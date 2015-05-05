#include "stdafx.h"
#include "Node.h"
#include "STR-TREE.h"
#include <math.h>

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
	//if(!Root->Leaf())
	//{
	//	for(NodeListElem *Tmp=Root->Childs; Tmp; Tmp=Tmp->Next)
	//	{
	//		MBR *RootMbr=Tmp->Elem->GetMBR();
	//		if(RootMbr->Intersection(*Mbr))
	//		{
	//			Node *TmpNode = this->FindNodeForSearch(Tmp->Elem,Mbr,Number);
	//			if(TmpNode)
	//				return TmpNode;
	//				
	//		}
	//	}
	//	return NULL;
	//}
	//else
	//{
	//	for(TrajectList *Tmp=Root->Trajectories; Tmp; Tmp=Tmp->Next)
	//	{
	//		if(Tmp->Trajetory->Intersection(Mbr->Left,Mbr->Top,Mbr->T0,Number))
	//			return Root;
	//	}
	//	return NULL;
	//}
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

	FirstNode=this->FindNodeForSearch(this->Root,Mbr,Number);
	if(!FirstNode)
		return NULL;

	while(true)
	{
		TrajectList *NewElem=new TrajectList;
		NewElem->Trajetory=FirstNode->FindTraject(Number,Mbr->T0);
		NewElem->Next=List;
		List=NewElem;
		Col++;
		int X=NewElem->Trajetory->X1;
		int Y=NewElem->Trajetory->Y1;
		int T=NewElem->Trajetory->T1;
		if( X>Mbr->Right || Y < Mbr->Bottom || Y > Mbr->Top || T > Mbr->T1)
			break;
		if( X >= FirstNode->GetMBR()->Left && X <= FirstNode->GetMBR()->Right && Y >= FirstNode->GetMBR()->Bottom && Y <= FirstNode->GetMBR()->Top )
			Mbr->T0=NewElem->Trajetory->T1;
		else
		{
			SearchMBR->Left=X;
			SearchMBR->Top=Y;
			SearchMBR->T0=T;
			FirstNode=this->FindNodeForSearch(this->Root,SearchMBR,Number);
		}
	}

	TrajectObj *Arr=new TrajectObj[Col+1];
	int i=Col;
	for(TrajectList *Tmp=List; Tmp; Tmp=Tmp->Next)
	{
		Arr[i].Number=Number;
		Arr[i].X=Tmp->Trajetory->X1;
		Arr[i].Y=Tmp->Trajetory->Y1;
		if(i==1)
		{
			i--;
			Arr[i].Number=Number;
			Arr[i].X=Tmp->Trajetory->X0;
			Arr[i].Y=Tmp->Trajetory->Y0;
		}
		i--;
	}
	Col++;
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


