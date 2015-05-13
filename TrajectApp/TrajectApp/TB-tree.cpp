//#include "stdafx.h"
#include "Node.h"
#include "TB-tree.h"
#include <math.h>
#include <iostream>
using namespace std;

TBtree::TBtree(int BranchingFactor,int CoefExp)
{
	this->Root=new TBNode(BranchingFactor,*new MBR(-1,-1,-1,-1,-1,-1),CoefExp);
	this->Stack=new TBNodeStack();
}

void TBtree::InsertTrajectory(Traject* NewTraject)
{
	TBNode *PlaceInsert=this->FindNode(this->Root,NewTraject);
	if(PlaceInsert)
	{
		if(!PlaceInsert->FullTraject())
		{
			PlaceInsert->InsertTraject(NewTraject);
			this->ExpCoefs();
			return;

		}
		else
		{
			TBNode *NewNode=PlaceInsert->NodePartitionTraject(NewTraject);
			PlaceInsert->SetNextNode(NewNode);
			NewNode->SetPreNode(PlaceInsert);
			this->InsertNode(NewNode);
		}
	}
	else
	{
		TBNode* NewNode=this->ChooseNode(this->Root, NewTraject, NULL);
		/*if(NewNode==NULL)
			this->InsertNewNodeDependOfDistanse(this->Root, NewTraject);*/
		this->ExpCoefs();
	}
}

void TBtree::InsertNode(TBNode *InsertedNode)
{
	TBNode *CurrentNode=NULL;
	TBNode *NodeFromPartition=InsertedNode;
	while(CurrentNode=this->Stack->Pop())
	{

		if(CurrentNode==this->Root)
		{
			if(this->Root->Leaf())
			{
				TBNode *NewRoot=new TBNode(this->Root->GetFactor(),*new MBR(0,0,0,0,0,0),this->CoefExp);
				NewRoot->InsertNode(this->Root);
				NewRoot->InsertNode(InsertedNode);
				this->Root=NewRoot;
				this->Root->ExpMBRNode();
				return;
			}
			else
			{
				if(!CurrentNode->InsertNode(NodeFromPartition))
				{
					NodeFromPartition=CurrentNode->NodePartitionNode(NodeFromPartition);
					TBNode *NewRoot=new TBNode(this->Root->GetFactor(),*new MBR(-1,-1,-1,-1,-1,-1),this->CoefExp);
					NewRoot->InsertNode(this->Root);
					NewRoot->InsertNode(NodeFromPartition);
					this->Root=NewRoot;
					this->Root->ExpMBRNode();
					return;
				}
				else
					return;
			}
		}
		if(!CurrentNode->InsertNode(NodeFromPartition))
		{
			NodeFromPartition=CurrentNode->NodePartitionNode(NodeFromPartition);
			CurrentNode->ExpMBRNode();
		}
		else
		{
			CurrentNode->ExpMBRNode();
			this->ExpCoefs();
			return;
		}
	}
}

TBNode* TBtree::FindNode(TBNode *Root, Traject *Trajectory)
{
	if(!Root->Leaf())
	{
		this->Stack->Push(Root);
		MBR *InsrtMbr=new MBR(Trajectory);
		for(TBNodeListElem *Tmp=Root->Childs; Tmp; Tmp=Tmp->Next)
		{
			MBR *RootMbr=Tmp->Elem->GetMBR();
			if(RootMbr->Intersection(*InsrtMbr))
			{
				TBNode *TmpNode=this->FindNode(Tmp->Elem,Trajectory);
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
		for(TrajectList *Tmp=Root->Trajectories; Tmp; Tmp=Tmp->Next)
		{
			if(Tmp->Trajetory->Connected(Trajectory))
			{
				//this->Stack->Push(Root);
				return Root;
			}
		}
		return NULL;
	}
}

TBNode* TBtree::ChooseNode(TBNode *Tmp, Traject *Trajectory, Node* CheckedNode)
{
	/*if(!Root->Leaf())
	{
		this->Stack->Push(Root);
		MBR *InsrtMbr=new MBR(Trajectory);
		for(TBNodeListElem *Tmp=Root->Childs; Tmp; Tmp=Tmp->Next)
		{
			MBR *RootMbr=Tmp->Elem->GetMBR();
			if(RootMbr->IntersectionR(*InsrtMbr))
			{
				TBNode *TmpNode=this->ChooseNode(Tmp->Elem,Trajectory,NULL);
				if(TmpNode)
					return TmpNode;
				else
					this->Stack->Pop();
			}
		}
		return  NULL;
	}
	else
	{*/
		/*if(!Root->FullTraject())
		{
			Root->InsertTraject(Trajectory);
			this->Stack->Push(Root);
			return Root;
		}
		else
		{
			TBNode *NewNode = Root->NodePartitionTraject(Trajectory);
			this->InsertNode(NewNode);
			return NewNode;
		}*/
		/*TBNode* Parent=this->Stack->Pop;*/

	//}
	
	if(!this->Root->Leaf())
	{
		TBNode *Leaf=Root;
		while(!Leaf->Leaf())
		{
			this->Stack->Push(Leaf);
			for(TBNodeListElem *Tmp=Leaf->Childs; Tmp; Tmp=Tmp->Next)
			{
				if(Tmp->Next==NULL)
				{
					Leaf=Tmp->Elem;
					break;
				}
			}	
		}
		TBNode* NewNode=new TBNode(Root->GetFactor(),*new MBR(-1,-1,-1,-1,-1,-1),Root->GetDec());
		
		NewNode->InsertTraject(Trajectory);
		this->InsertNode(NewNode);
		return NewNode;
	}
	else
	{
	/*	if(!Root->FullTraject())
		{
			Root->InsertTraject(Trajectory);
			this->Stack->Push(Root);
			return Root;
		}
		else
		{*/
			TBNode *NewNode = Root->NodePartitionTraject(Trajectory);
			this->InsertNode(NewNode);
			return NewNode;
		//}
	}
}

TrajectObj* TBtree::FindTrajectory(MBR *Mbr, int Number, int &Col)
{
	Col=0;
	TBNode *FirstNode=this->FindFirstNode(this->Root,Mbr,Number);
	TrajectList* List=NULL;
	for(TBNode* Tmp=FirstNode; Tmp; Tmp=Tmp->GetNextNode())
	{
		for(TrajectList *Temp=Tmp->Trajectories; Temp; Temp=Temp->Next)
		{
			if(Temp->Trajetory->NotIncludedLeft(Mbr->Left,Mbr->Right,Mbr->Bottom, Mbr->Top, Mbr->T0, Mbr->T1))
				continue;
			if(Temp->Trajetory->NotIncludedRight(Mbr->Left,Mbr->Right,Mbr->Bottom, Mbr->Top, Mbr->T0, Mbr->T1))
			{
				Col++;
				TrajectObj *Arr=new TrajectObj[Col];
				int i=0;
				for(TrajectList *Tmp=List; Tmp; Tmp=Tmp->Next)
				{
					Arr[i].Number=Tmp->Trajetory->Number;
					Arr[i].X=Tmp->Trajetory->X1;
					Arr[i].Y=Tmp->Trajetory->Y1;
					//if(i==Col-1)
					if(Tmp->Next==NULL)
					{
						Arr[i+1].Number=Tmp->Trajetory->Number;
						Arr[i+1].X=Tmp->Trajetory->X0;
						Arr[i+1].Y=Tmp->Trajetory->Y0;
						break;		
					}
					i++;
				}
				TrajectObj *ResultArr=new TrajectObj[Col];
				for(int p=0,z=Col-1; p<Col; p++)
				{
					ResultArr[p]=Arr[z];
					z--;
				}
				return ResultArr;
			}
			TrajectList* NewElem=new TrajectList();
			NewElem->Trajetory=Temp->Trajetory;
			NewElem->Next=List;
			List=NewElem;
			Col++;
		}
	}
	Col++;
	TrajectObj *Arr=new TrajectObj[Col];
	int i=0;
	for(TrajectList *Tmp=List; Tmp; Tmp=Tmp->Next)
	{
		Arr[i].Number=Tmp->Trajetory->Number;
		Arr[i].X=Tmp->Trajetory->X1;
		Arr[i].Y=Tmp->Trajetory->Y1;
		if(Tmp->Next==NULL)
		{
			Arr[i+1].Number=Tmp->Trajetory->Number;
			Arr[i+1].X=Tmp->Trajetory->X0;
			Arr[i+1].Y=Tmp->Trajetory->Y0;
			break;		
		}
		i++;
	}
	//return Arr;
	TrajectObj *ResultArr=new TrajectObj[Col];
	for(int p=0,z=Col-1; p<Col; p++)
	{
		ResultArr[p]=Arr[z];
		z--;
	}
	return ResultArr;
}

TBNode* TBtree::FindFirstNode(TBNode *Root, MBR* Mbr,int Number)
{
	if(!Root->Leaf())
	{
		for(TBNodeListElem *Tmp=Root->Childs; Tmp; Tmp=Tmp->Next)
		{
			MBR *RootMbr=Tmp->Elem->GetMBR();
			if(RootMbr->Intersection(*Mbr))
			{
				TBNode *TmpNode=this->FindFirstNode(Tmp->Elem,Mbr,Number);
				if(TmpNode)
					return TmpNode;
			}
		}
		return NULL;
	}
	else
	{
		if(this->Root->Leaf())
			this->Stack->Push(Root);
		for(TrajectList *Tmp=Root->Trajectories; Tmp; Tmp=Tmp->Next)
		{
			if(Tmp->Trajetory->MbrIncludedTraject(Mbr->Left,Mbr->Right,Mbr->Bottom, Mbr->Top, Mbr->T0, Mbr->T1,Number))
			{
				return Root;
			}
		}
		return NULL;
	}
}