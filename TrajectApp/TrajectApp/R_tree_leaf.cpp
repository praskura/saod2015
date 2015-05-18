//#include "stdafx.h"
#include "Trajectory.h"
#include "Node.h"
#include"R_tree_leaf.h"

RTreeLeaf::RTreeLeaf(int BranchingFactor)
{
	this->Root=new RTreeNodeLeaf(BranchingFactor);
	this->Stack=new RTreeLeafNodeStack();
}

void RTreeLeaf::InsertLeaf(LeafOfTraject *Leaf)
{
	RTreeNodeLeaf *PlaceInsert=this->FindNode(this->Root,Leaf);
	if(PlaceInsert)
	{
		if(!PlaceInsert->FullTraject())
		{
			PlaceInsert->InsertLeaf(Leaf);
			this->ExpCoefs();
			return;

		}
		else
		{
			RTreeNodeLeaf *NewNode=PlaceInsert->NodePartitionLeaf(Leaf);
			this->InsertNode(NewNode);
		}
	}
	else
	{
		RTreeNodeLeaf* NewNode=this->ChooseNode(this->Root,Leaf, NULL);
		this->ExpCoefs();
	}
}

void RTreeLeaf::InsertNode(RTreeNodeLeaf *InsertedNode)
{
	RTreeNodeLeaf *CurrentNode=NULL;
	RTreeNodeLeaf *NodeFromPartition=InsertedNode;
	while(CurrentNode=this->Stack->Pop())
	{

		if(CurrentNode==this->Root)
		{
			if(this->Root->Leaf())
			{
				RTreeNodeLeaf *NewRoot=new RTreeNodeLeaf(this->Root->GetFactor());
				NewRoot->InsertNode(this->Root);
				NewRoot->InsertNode(InsertedNode);
				this->Root=NewRoot;
				this->Root->ExpRangeNode();
				return;
			}
			else
			{
				if(!CurrentNode->InsertNode(NodeFromPartition))
				{
					NodeFromPartition=CurrentNode->NodePartitionNode(NodeFromPartition);
					RTreeNodeLeaf *NewRoot=new RTreeNodeLeaf(this->Root->GetFactor());
					NewRoot->InsertNode(this->Root);
					NewRoot->InsertNode(NodeFromPartition);
					this->Root=NewRoot;
					this->Root->ExpRangeNode();
					return;
				}
				else
					return;
			}
		}
		if(!CurrentNode->InsertNode(NodeFromPartition))
		{
			NodeFromPartition=CurrentNode->NodePartitionNode(NodeFromPartition);
			CurrentNode->ExpRangeNode();
		}
		else
		{
			CurrentNode->ExpRangeNode();
			this->ExpCoefs();
			return;
		}
	}
}

RTreeNodeLeaf* RTreeLeaf::FindNode(RTreeNodeLeaf *Root, LeafOfTraject *Leaf)
{
	if(!Root->Leaf())
	{
		this->Stack->Push(Root);
		for(RTreeNodeLeafList *Tmp=Root->Childs; Tmp; Tmp=Tmp->Next)
		{
			if(Root->IntersectionDot(Leaf->ID_Leaf))
			{
				RTreeNodeLeaf *TmpNode=this->FindNode(Tmp->Elem,Leaf);
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
		if(this->Root->Leaf())
			this->Stack->Push(Root);
		for(ListOfLeafs *Tmp=Root->Leafs; Tmp; Tmp=Tmp->Next)
		{
			if(Tmp->Elem->ID_Leaf==Leaf->ID_Leaf-1)
			{
				return Root;
			}
		}
		return NULL;
	}
}

RTreeNodeLeaf* RTreeLeaf::ChooseNode(RTreeNodeLeaf *Tmp, LeafOfTraject *NewLeaf, Node* CheckedNode)
{
	if(!this->Root->Leaf())
	{
		RTreeNodeLeaf *Leaf=Root;
		while(!Leaf->Leaf())
		{
			this->Stack->Push(Leaf);
			for(RTreeNodeLeafList *Tmp=Leaf->Childs; Tmp; Tmp=Tmp->Next)
			{
				if(Tmp->Next==NULL)
				{
					Leaf=Tmp->Elem;
					break;
				}
			}	
		}
		RTreeNodeLeaf* NewNode=new RTreeNodeLeaf(Root->GetFactor());
		
		NewNode->InsertLeaf(NewLeaf);
		this->InsertNode(NewNode);
		return NewNode;
	}
	else
	{
		if(!Root->FullTraject())
		{
			Root->InsertLeaf(NewLeaf);
			this->Stack->Push(Root);
			return Root;
		}
		else
		{
			RTreeNodeLeaf *NewNode = Root->NodePartitionLeaf(NewLeaf);
			this->InsertNode(NewNode);
			return NewNode;
		}
	}
}

ListOfLeafs* RTreeLeaf::FindLeafs(RTreeNodeLeaf *Root, ListOfID* IDs)
{
	/*ListOfLeafs *TempList=NULL;
	ListOfID *TmpID=IDs;
	int CurrentID=NULL;
	if(!Root->Leaf())
	{
		CurrentID=TmpID->Elem;
		this->Stack->Push(Root);
		for(RTreeNodeLeafList *Tmp=Root->Childs; Tmp; Tmp=Tmp->Next)
		{
			if(TmpID==NULL)
				return TempList;
			else
				CurrentID=TmpID->Elem;
			if(Tmp->Elem->IntersectionDot(CurrentID))
			{
				ListOfLeafs* NewElem=this->FindLeafs(Tmp->Elem,TmpID);
				if(NewElem)	
				{
					int Col=0;
					for(ListOfLeafs *Tmp=NewElem; Tmp; Tmp=Tmp->Next)
					{
						Col++;
						if(Tmp->Next==NULL)
						{
							Tmp->Next=TempList;
							TempList=NewElem;
							break;
						}
					}
					for(int i=0; i<Col; i++)
						TmpID=TmpID->Next;
				}
			}
		}
		return TempList;
	}
	else
	{
		CurrentID=TmpID->Elem;
		if(this->Root->Leaf())
			this->Stack->Push(Root);
		for(ListOfLeafs *Tmp=Root->Leafs; Tmp; Tmp=Tmp->Next)
		{
			if(Tmp->Elem->ID_Leaf==CurrentID)
			{
				ListOfLeafs* NewElem=new ListOfLeafs;
				NewElem->Elem=Tmp->Elem;
				NewElem->Next=NULL;
				TempList=NewElem;
				TmpID=TmpID->Next;
				if(TmpID!=NULL)
					CurrentID=TmpID->Elem;
				else
					break;
			}
		}
		return TempList;
	}*/
	ListOfLeafs *TempList=NULL;
	if(!Root->Leaf())
	{
		this->Stack->Push(Root);
		for(RTreeNodeLeafList *Tmp=Root->Childs; Tmp; Tmp=Tmp->Next)
		{
			if(Tmp->Elem->IncludedInList(IDs))
			{
				ListOfLeafs* NewElem=this->FindLeafs(Tmp->Elem,IDs);
				if(NewElem)	
				{
					int Col=0;
					for(ListOfLeafs *Tmp=NewElem; Tmp; Tmp=Tmp->Next)
					{
						Col++;
						if(Tmp->Next==NULL)
						{
							Tmp->Next=TempList;
							TempList=NewElem;
							break;
						}
					}
				}
			}
		}
		return TempList;
	}
	else
	{
		if(this->Root->Leaf())
			this->Stack->Push(Root);

		for(ListOfLeafs *Tmp=Root->Leafs; Tmp; Tmp=Tmp->Next)
		{
			if(Tmp->IncludedInList(IDs))
			{
				ListOfLeafs* NewElem=new ListOfLeafs;
				NewElem->Elem=Tmp->Elem;
				NewElem->Next=TempList;
				TempList=NewElem;
			}
		}
		return TempList;
	}
}
