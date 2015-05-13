#include "Node.h"
#include "STR-TREE.h"
#include <math.h>
#include <iostream>
using namespace std;

StrTree::StrTree(int BranchingFactor,int CoefExp)
{
	this->CoefExp=CoefExp;
	this->Levels=0;
	this->Root=new Node(BranchingFactor,*new MBR(-1,-1,-1,-1,-1,-1),0);
	this->Stack=new NodeStack();
}

void StrTree::InsertTrajectory(Traject* NewTraject)
{
	Node *PlaceInsert=this->FindNode(this->Root,NewTraject);
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
			Node *NewNode=PlaceInsert->NodePartitionTraject(NewTraject);
			this->InsertNode(NewNode);
		}
	}
	else
	{
		Node* NewNode=this->ChooseNode(this->Root, NewTraject, NULL);
		if(NewNode==NULL)
			this->InsertNewNodeDependOfDistanse(this->Root, NewTraject);
		this->ExpCoefs();
	}
}

Node* StrTree::FindNode(Node *Root, Traject *Trajectory)
{
	if(!Root->Leaf())
	{
		this->Stack->Push(Root);
		MBR *InsrtMbr=new MBR(Trajectory);
		for(NodeListElem *Tmp=Root->Childs; Tmp; Tmp=Tmp->Next)
		{
			MBR *RootMbr=Tmp->Elem->GetMBR();
			if(RootMbr->Intersection(*InsrtMbr))
			{
				Node *TmpNode=this->FindNode(Tmp->Elem,Trajectory);
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

Node* StrTree::FindNodeForSearch(Node* Root, MBR *Mbr, int Number)
{
	if(!Root->Leaf())
	{
		this->Stack->Push(Root);
		//MBR *InsrtMbr=new MBR(Trajectory);
		for(NodeListElem *Tmp=Root->Childs; Tmp; Tmp=Tmp->Next)
		{
			MBR *RootMbr=Tmp->Elem->GetMBR();
			if(RootMbr->Intersection(*Mbr))
			{
				Node *TmpNode=this->FindNodeForSearch(Tmp->Elem,Mbr,Number);
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
			if(Tmp->Trajetory->Intersection(Mbr->Left,Mbr->T0,Number))
			{
				//this->Stack->Push(Root);
				return Root;
			}
		}
		return NULL;
	}
}

Node* StrTree::ChooseNode(Node *Root, Traject *Trajectory, Node* CheckedNode)
{
	if(!Root->Leaf())
	{
		this->Stack->Push(Root);
		MBR *InsrtMbr=new MBR(Trajectory);
		for(NodeListElem *Tmp=Root->Childs; Tmp; Tmp=Tmp->Next)
		{
			MBR *RootMbr=Tmp->Elem->GetMBR();
			if(RootMbr->IntersectionR(*InsrtMbr))
			{
				Node *TmpNode=this->ChooseNode(Tmp->Elem,Trajectory,NULL);
				if(TmpNode)
					return TmpNode;
				else
					this->Stack->Pop();
			}
		}
		return  NULL;
	}
	else
	{
		if(!Root->FullTraject())
		{
			Root->InsertTraject(Trajectory);
			this->Stack->Push(Root);
			return Root;
		}
		else
		{
			Node *NewNode = Root->NodePartitionTraject(Trajectory);
			this->InsertNode(NewNode);
			return NewNode;
		}
	}
}

void StrTree::ExtendRoot(Traject *NewTraject, int T0, int T1)
{
	MBR *RootMbr=this->Root->GetMBR();
	MBR *MbrNewRoot=new MBR(NewTraject);
	int RootXC=(RootMbr->Right-RootMbr->Left)/2;
	int RootYC=(RootMbr->Top-RootMbr->Bottom)/2;
	int NewRootXC=(MbrNewRoot->Right-MbrNewRoot->Left)/2 - RootXC;
	int NewRootYC=(MbrNewRoot->Top-MbrNewRoot->Bottom)/2 - RootYC;
	int Lenth=pow((NewRootXC*NewRootXC)+(NewRootYC*NewRootYC),0.5);
	int NewLevels=Lenth/this->CoefExp;
	for(int i=0; i<NewLevels; i++)
	{
		MBR *NewMbr=new MBR(*this->Root->GetMBR());
		
		if(i==NewLevels-1)
			NewMbr->ExpMbr(MbrNewRoot);
		else
		{
			NewMbr->ExpMbrOnKoef(MbrNewRoot,this->CoefExp);
		}
		Node *InsertedNode=new Node(this->Root->GetFactor(),*NewMbr,this->CoefExp);
		InsertedNode->InsertNode(this->Root);
		this->Root=InsertedNode;
		this->Levels++;
		this->Root->IncDec(i+1);
	}
	this->ChooseNode(this->Root,NewTraject,NULL);
}

TrajectObj* StrTree::FindTrajectory(MBR *Mbr, int Number, int &Col)
{
	TrajectList* List= NULL;
	MBR *SearchMBR=new MBR(Mbr->Left,Mbr->Top,Mbr->Left,Mbr->Top,Mbr->T0,Mbr->T1);
	Node *FirstNode=NULL;
	List=this->FindTrajectories(this->Root,Mbr,Number);
	int Count = List->Sort();
	Col=Count+1;
	TrajectObj *Arr=new TrajectObj[Count+1];
	int i=0;
	for(TrajectList *Tmp=List; Tmp; Tmp=Tmp->Next)
	{
		Arr[i].Number=Tmp->Trajetory->Number;
		Arr[i].X=Tmp->Trajetory->X0;
		Arr[i].Y=Tmp->Trajetory->Y0;
		if(i==Count-1)
		{
			Arr[i+1].Number=Tmp->Trajetory->Number;
			Arr[i+1].X=Tmp->Trajetory->X1;
			Arr[i+1].Y=Tmp->Trajetory->Y1;
			break;		
		}
		i++;
	}
	return Arr;
}

Node* StrTree::InsertNewNodeDependOfDistanse(Node *Root, Traject *Trajectory)
{
	if(!Root->Leaf())
	{
		this->Stack->Push(Root);
		MBR *InsrtMbr=new MBR(Trajectory);
		double Distanse=0;
		double MinDistanse=100000;
		MBR *CurrentMBR=new MBR(Trajectory);
		Node *CurrentNode=NULL;
		MBR *TmpMBR=new MBR(-1,-1,-1,-1,-1,-1);
		for(NodeListElem *Tmp=Root->Childs; Tmp; Tmp=Tmp->Next)
		{
			TmpMBR->SetMbr(Tmp->Elem->GetMBR());
			Distanse=TmpMBR->Distanse(CurrentMBR);
			if(MinDistanse > Distanse)
			{
				MinDistanse=Distanse;
				CurrentNode=Tmp->Elem;
			}
		}
		return this->InsertNewNodeDependOfDistanse(CurrentNode,Trajectory);
	}
	else
	{
		if(!Root->FullTraject())
		{
			Root->InsertTraject(Trajectory);
			this->Stack->Push(Root);
			return Root;
		}
		else
		{
			Node *NewNode = Root->NodePartitionTraject(Trajectory);
			this->InsertNode(NewNode);
			return NewNode;
		}
	}
}

TrajectList* StrTree::FindTrajectories(Node *Root, MBR* Mbr,int Number)
{
	TrajectList* NewElems=NULL;
	TrajectList* ReturnedList=NULL;
	if(!Root->Leaf())
	{
		this->Stack->Push(Root);
		for(NodeListElem *Tmp=Root->Childs; Tmp; Tmp=Tmp->Next)
		{
			MBR *RootMbr=Tmp->Elem->GetMBR();
			if(RootMbr->Intersection(*Mbr))
			{
				if(NewElems=this->FindTrajectories(Tmp->Elem,Mbr,Number))
				{
					for(TrajectList *Tmp=NewElems; Tmp; Tmp=Tmp->Next)
					{
						if(Tmp->Next==NULL)
						{
							Tmp->Next=ReturnedList;
							ReturnedList=NewElems;
							break;
						}
					}
				}
			}
		}
		return ReturnedList;
	}
	else
	{
		if(this->Root->Leaf())
			this->Stack->Push(Root);
		TrajectList* List=NULL;
		for(TrajectList *Tmp=Root->Trajectories; Tmp; Tmp=Tmp->Next)
		{
			if(Tmp->Trajetory->MbrIncludedTraject(Mbr->Left,Mbr->Right,Mbr->Bottom, Mbr->Top, Mbr->T0, Mbr->T1,Number))
			{
				//this->Stack->Push(Root);
				TrajectList *NewElem=new TrajectList;
				NewElem->Trajetory=Tmp->Trajetory;
				NewElem->Next=List;
				List=NewElem;
			}
		}
		return List;
	}




}

