//#include "stdafx.h"
#include "Trajectory.h"
#include "Node.h"
#include"R_tree_time.h"

RTreeTime::RTreeTime(int BranchingFactor)
{
	this->Root=new RTreeNodeTime(BranchingFactor);
	this->Stack=new RTreeTimeNodeStack();
}

void RTreeTime::InsertID(int T,int ID)
{
	RTreeNodeTime *PlaceInsert=this->FindNode(this->Root,ID,T);
	if(PlaceInsert)
	{
		if(!PlaceInsert->FullTraject())
		{
			PlaceInsert->InsertID(ID);
			this->ExpCoefs(T);
			return;

		}
		else
		{
			RTreeNodeTime *NewNode=PlaceInsert->NodePartitionID(ID,T);
			this->InsertNode(NewNode);
		}
	}
	else
	{
		RTreeNodeTime* NewNode=this->ChooseNode(this->Root,ID,T, NULL);
		this->ExpCoefs(T);
	}
}

void RTreeTime::InsertNode(RTreeNodeTime *InsertedNode)
{
	RTreeNodeTime *CurrentNode=NULL;
	RTreeNodeTime *NodeFromPartition=InsertedNode;
	while(CurrentNode=this->Stack->Pop())
	{

		if(CurrentNode==this->Root)
		{
			if(this->Root->Leaf())
			{
				RTreeNodeTime *NewRoot=new RTreeNodeTime(this->Root->GetFactor());
				NewRoot->InsertNode(this->Root);
				NewRoot->InsertNode(InsertedNode);
				this->Root=NewRoot;
				this->Root->ExpTRangeNode(0);
				return;
			}
			else
			{
				if(!CurrentNode->InsertNode(NodeFromPartition))
				{
					NodeFromPartition=CurrentNode->NodePartitionNode(NodeFromPartition);
					RTreeNodeTime *NewRoot=new RTreeNodeTime(this->Root->GetFactor());
					NewRoot->InsertNode(this->Root);
					NewRoot->InsertNode(NodeFromPartition);
					this->Root=NewRoot;
					this->Root->ExpTRangeNode(0);
					return;
				}
				else
					return;
			}
		}
		if(!CurrentNode->InsertNode(NodeFromPartition))
		{
			NodeFromPartition=CurrentNode->NodePartitionNode(NodeFromPartition);
			CurrentNode->ExpTRangeNode(0);
		}
		else
		{
			CurrentNode->ExpTRangeNode(0);
			this->ExpCoefs(0);
			return;
		}
	}
}

RTreeNodeTime* RTreeTime::FindNode(RTreeNodeTime *Root, int ID, int T)
{
	if(!Root->Leaf())
	{
		this->Stack->Push(Root);
		for(RTreeNodeTimeList *Tmp=Root->Childs; Tmp; Tmp=Tmp->Next)
		{
			if(Root->IntersectionDot(T))
			{
				RTreeNodeTime *TmpNode=this->FindNode(Tmp->Elem,ID,T);
				if(TmpNode)
					return TmpNode;
				else
					this->Stack->Pop();
			}
		}
		return NULL;
	}
	else
	{
	/*	if(this->Root->Leaf())
			this->Stack->Push(Root);
		for(ListOfLeafs *Tmp=Root->Leafs; Tmp; Tmp=Tmp->Next)
		{
			if(Tmp->Elem->ID_Leaf==Leaf->ID_Leaf-1)
			{
				return Root;
			}
		}
		return NULL;*/
		if(this->Root->IDs==NULL)
		{
			this->Stack->Push(Root);
			return Root;
		}
		TimeRange *Temp=Root->GetRange();
		if(Temp->T0!=T)
			return NULL;
		else
			return Root;
	}
}

RTreeNodeTime* RTreeTime::ChooseNode(RTreeNodeTime *Tmp, int ID, int T, Node* CheckedNode)
{
	if(!this->Root->Leaf())
	{
		RTreeNodeTime *Leaf=Root;
		while(!Leaf->Leaf())
		{
			this->Stack->Push(Leaf);
			for(RTreeNodeTimeList *Tmp=Leaf->Childs; Tmp; Tmp=Tmp->Next)
			{
				if(Tmp->Next==NULL)
				{
					Leaf=Tmp->Elem;
					break;
				}
			}	
		}
		RTreeNodeTime* NewNode=new RTreeNodeTime(Root->GetFactor());
		
		NewNode->InsertID(ID);
		NewNode->ExpTRange(T);
		this->InsertNode(NewNode);
		return NewNode;
	}
	else
	{
		/*if(!Root->FullTraject())
		{
			Root->InsertID(ID);
			this->Stack->Push(Root);
			return Root;
		}
		else
		{*/
			this->Stack->Push(Root);
			RTreeNodeTime *NewNode = Root->NodePartitionID(ID,T);
			this->InsertNode(NewNode);
			return NewNode;
		//}
	}
}

ListOfID* RTreeTime::FindIDs(RTreeNodeTime *Root, TimeRange* TRange)
{
	ListOfID *CurrentList=NULL;
	if(!Root->Leaf())
	{
		this->Stack->Push(Root);
		for(RTreeNodeTimeList *Tmp=Root->Childs; Tmp; Tmp=Tmp->Next)
		{
			if(Tmp->Elem->IntersectionRange(TRange))
			{
				ListOfID *TmpList=this->FindIDs(Tmp->Elem,TRange);
				if(TmpList)
				{
					//for(ListOfID *Temp=TmpList; Temp; Temp=Temp->Next)
					if(!CurrentList)
						CurrentList=TmpList;
					else
					{
						for(ListOfID *Temp=CurrentList; Temp; Temp=Temp->Next)
						{
							if(Temp->Next==NULL)
							{
								//Temp->Next=CurrentList;
								//CurrentList=TmpList;
								Temp->Next=TmpList;
								break;
							}
						}
					}

				}
				//return CurrentList;
			}
		}
		return CurrentList;
	}
	else
	{
		if(Root->IntersectionRange(TRange))
		{
			for(ListOfID *Tmp=Root->IDs; Tmp; Tmp=Tmp->Next)
			{
				ListOfID* Temp=new ListOfID;
				Temp->Elem=Tmp->Elem;
				Temp->Next=CurrentList;
				CurrentList=Temp;
			}
		}
		return CurrentList;
	}
}