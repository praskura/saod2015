#pragma once
//#include "stdafx.h"
#include "math.h"
#include"MBR.h"
#include"IDRange.h"

#ifndef TRAJECT
#include "Trajectory.h"
#endif

struct TrajectList
{
	Traject *Trajetory;
	TrajectList *Next;
	
	Traject* MaxTimeTraject()
	{
		TrajectList *MaxElem=this;
		int MaxTime=0;
		for(TrajectList *Tmp=this; Tmp; Tmp=Tmp->Next)
		{
			if(Tmp->Trajetory->T1>MaxTime)
			{
				MaxElem=Tmp;
				MaxTime=Tmp->Trajetory->T1;
			}
		}
		return MaxElem->Trajetory;
	}

	int Sort()
	{
		TrajectList* Pointer=this;
		TrajectList* MinElem=this;
		int col=0;
		int MinTime=10000000000;
		while(true)
		{
			for(TrajectList *Tmp=Pointer; Tmp; Tmp=Tmp->Next)
			{
				if(Tmp->Trajetory->T1<MinTime)
				{
					MinTime=Tmp->Trajetory->T1;
					MinElem=Tmp;
				}
			}
			if(Pointer==NULL)
				return 0;
			Traject* Tmp=Pointer->Trajetory;
			Pointer->Trajetory=MinElem->Trajetory;
			MinElem->Trajetory=Tmp;
			Pointer=Pointer->Next;
			MinElem=Pointer;
			col++;
			if(Pointer==NULL)
				break;
			MinTime=10000000000;
		}
		return col;
	}
};

struct ListOfID
{
	int Elem;
	ListOfID *Next;
};

struct LeafOfTraject
{
	int ID_Leaf;
	TrajectList *Trajectories;
	
	LeafOfTraject()
	{
		this->ID_Leaf=0;
		this->Trajectories=NULL;
	}

	void InsertTrajectory(Traject *Trajectory)
	{
		TrajectList* Elem=new TrajectList;
		if(this->Trajectories==NULL)
		{
			Elem->Trajetory=Trajectory;
			Elem->Next=this->Trajectories;
			this->Trajectories=Elem;
			return;
		}
		for(TrajectList *Tmp = this->Trajectories; Tmp; Tmp=Tmp->Next)
		{
			if(Tmp->Next==NULL)
			{
				Elem->Trajetory=Trajectory;
				Elem->Next=Tmp->Next;
				Tmp->Next=Elem;
				return;
			}
		}
	}

};

struct ListOfLeafs
{
	LeafOfTraject *Elem;
	ListOfLeafs *Next;
	bool IncludedInList(ListOfID* IDs)
	{
		for(ListOfID *Tmp=IDs; Tmp; Tmp=Tmp->Next)
		{
			if(this->Elem->ID_Leaf==Tmp->Elem)
				return true;
		}
		return false;
	}
};

struct TrajectObj
{
	int Number;
	int X;
	int Y;
};

struct NodeListElem;

class Node
{
private:
	MBR *Mbr;
	int DecExtend;
	int BranchingFactor;
	int Col;
	int ColTraject;
public:
	NodeListElem *Childs;
	TrajectList *Trajectories;
	Node();
	Node(int BranchingFactor, MBR &Mbr, Traject* Trajectory,int DecExtend);
	Node(int BranchingFactor, MBR &Mbr, int DecExtend);
	bool Full();
	bool InsertNode(Node*Tmp);
	Node* NodePartitionTraject(Traject *Trajectory);
	Node* NodePartitionNode(Node *NewNode);
	MBR *GetMBR(){return this->Mbr;}
	void InsertTraject(Traject *Trajectory)
	{
		TrajectList *NewElem=new TrajectList;
		NewElem->Trajetory=Trajectory;
		NewElem->Next=this->Trajectories;
		this->Trajectories=NewElem;
		this->ColTraject++;
		this->ExpMBRTraject();
	}
	bool NodeIntersection(Node *Tmp) { return this->Mbr->Intersection(*Tmp->GetMBR()); }
	bool NodeIntersection(MBR *Tmp) { return this->Mbr->Intersection(*Tmp); }
	int GetFactor() { return this->BranchingFactor; }
	bool Leaf()
	{
		if(this->Childs!=NULL)
			return false;
		return true;
	}
	int GetVol()
	{
		MBR *Mbr=this->Mbr;
		return abs(Mbr->Bottom - Mbr->Top)*abs(Mbr->Right - Mbr->Left)*abs(Mbr->T1 - Mbr->T0);
	}
	void SetMbr(MBR *Mbr)
	{
		if(this->Mbr==Mbr)
			this->Mbr=Mbr;
		else
		{
			if(this->Mbr!=NULL)
				delete this->Mbr;
			this->Mbr=Mbr;
		}
	}
	bool FullTraject()
	{
		if(this->ColTraject<this->BranchingFactor)
			return false;
		return true;
	}
	int GetDec() {return this->DecExtend;}
	void ReduceDec(int a) {this->DecExtend-=a;}
	void IncDec(int a) {this->DecExtend*=a;}
	void ExpTime(MBR *Mbr)
	{
		if(this->Mbr->T0 > Mbr->T0 || this->Mbr->T0==-1)
			this->Mbr->T0=Mbr->T0;
		if(this->Mbr->T1 < Mbr->T1 || this->Mbr->T1==-1)
			this->Mbr->T1=Mbr->T1;
	}
	void ExpColTraject() {this->ColTraject++;}
	void IncCol(){this->Col++;}
	void ZeroCol(){this->Col=0;}
	void IncColTraject(){this->ColTraject++;}
	void ZeroColTraject(){this->ColTraject=0;}
	Traject* FindTraject(int Number, int T0, int T1)
	{
		for(TrajectList *Tmp=this->Trajectories; Tmp; Tmp=Tmp->Next)
		{
			if(Tmp->Trajetory->Number==Number && (Tmp->Trajetory->T0 >= T0 && Tmp->Trajetory->T1 <= T1))
				return Tmp->Trajetory;
		}
		return NULL;
	}
	void ExpMBRNode();
	void ExpMBRTraject()
	{
		MBR *CurrentMBR=this->GetMBR();
		CurrentMBR->SetMbr(&MBR(-1,-1,-1,-1,-1,-1));
		MBR *TmpMbr=new MBR(this->Trajectories->Trajetory);
		for(TrajectList *Tmp = this->Trajectories; Tmp; Tmp=Tmp->Next)
		{
			TmpMbr->SetMbrTraject(Tmp->Trajetory);
			CurrentMBR->ExpMbr(TmpMbr);
		}
	}
};

struct NodeListElem
{
	Node *Elem;
	NodeListElem *Next;
};

struct TBNodeListElem;

class TBNode:public Node
{
private:
	TBNode* NextNodeWithTraject;
	TBNode* PreNodeWithTraject;
public:
	TBNodeListElem* Childs;
	TBNode(int BranchingFactor, MBR &Mbr, int DecExtend);
	virtual TBNode* NodePartitionTraject(Traject *Trajectory);
	virtual TBNode* NodePartitionNode(TBNode *NewNode);
	virtual void InsertTraject(Traject *Trajectory);
	virtual bool InsertNode(TBNode*Tmp);
	void ExpMBRNode();
	bool Leaf()
	{
		if(this->Childs!=NULL)
			return false;
		return true;
	}
	void SetPreNode(TBNode *Node)
	{
		this->PreNodeWithTraject=Node;
	}
	void SetNextNode(TBNode *Node)
	{
		this->NextNodeWithTraject=Node;
	}
	TBNode* GetPreNode()
	{
		return this->PreNodeWithTraject;
	}
	TBNode* GetNextNode()
	{
		return this->NextNodeWithTraject;
	}
};

struct TBNodeListElem
{
	TBNode *Elem;
	TBNodeListElem *Next;
};

struct RTreeNodeLeafList;


class RTreeNodeLeaf
{
private:
	int ColLeaf;
	int BranchingFactor;
	Range *IDRange;
	int Col;
public:
	ListOfLeafs *Leafs;
	RTreeNodeLeafList *Childs;
	RTreeNodeLeaf(int BranchingFactor);
	RTreeNodeLeaf* NodePartitionLeaf(LeafOfTraject *NewLeaf);
	RTreeNodeLeaf* NodePartitionNode(RTreeNodeLeaf *NewNode);
	void InsertLeaf(LeafOfTraject *NewLeaf);
	bool InsertNode(RTreeNodeLeaf *NewNode);
	bool Intersection(RTreeNodeLeaf *Node);
	void ExpRangeNode();
	bool Leaf()
	{
		if(this->Childs!=NULL)
			return false;
		return true;
	}
	bool FullTraject()
	{
		if(this->ColLeaf<this->BranchingFactor)
			return false;
		return true;
	}
	int GetFactor()
	{
		return this->BranchingFactor;
	}
	void IncColLeafs()
	{
		this->ColLeaf++;
	}
	bool Full()
	{
		if(this->Col>=this->BranchingFactor)
			return true;
		return false;
	}
	void IncCol()
	{
		this->Col++;
	}
	Range *GetRange()
	{
		return this->IDRange;
	}
	void ExpRangeLeafs()
	{
		Range *CurrentRange=this->GetRange();
		CurrentRange->ID0=CurrentRange->ID1=-1;
		for(ListOfLeafs *Tmp = this->Leafs; Tmp; Tmp=Tmp->Next)
		{
			if(Tmp->Elem->ID_Leaf > CurrentRange->ID1 || CurrentRange->ID1==-1)
				CurrentRange->ID1=Tmp->Elem->ID_Leaf;
			if(Tmp->Elem->ID_Leaf < CurrentRange->ID0 || CurrentRange->ID0==-1)
				CurrentRange->ID0=Tmp->Elem->ID_Leaf;
		}
	}
	bool IntersectionDot(int ID)
	{
		if(this->IDRange->ID0<=ID && this->IDRange->ID1>=ID)
			return true;
		return false;
	}
	bool IncludedInList(ListOfID* IDs)
	{
		for(ListOfID *Tmp=IDs; Tmp; Tmp=Tmp->Next)
		{
			if(this->IntersectionDot(Tmp->Elem))
				return true;
		}
		return false;
	}
	
};

struct RTreeNodeLeafList
{
	RTreeNodeLeaf *Elem;
	RTreeNodeLeafList *Next;
};

struct RTreeNodeTimeList;

class RTreeNodeTime
{
private:
	int ColLeaf;
	int BranchingFactor;
	TimeRange *TRange;
	int Col;
public:
	ListOfID *IDs;
	RTreeNodeTimeList *Childs;
	RTreeNodeTime(int BranchingFactor);
	RTreeNodeTime* NodePartitionID(int ID, int T);
	RTreeNodeTime* NodePartitionNode(RTreeNodeTime *NewNode);
	void InsertID(int ID);
	bool InsertNode(RTreeNodeTime *NewNode);
	bool Intersection(RTreeNodeTime *Node);
	void ExpTRangeNode(int T);
	bool Leaf()
	{
		if(this->Childs!=NULL)
			return false;
		return true;
	}
	bool FullTraject()
	{
		if(this->ColLeaf<this->BranchingFactor)
			return false;
		return true;
	}
	int GetFactor()
	{
		return this->BranchingFactor;
	}
	void IncColLeafs()
	{
		this->ColLeaf++;
	}
	bool Full()
	{
		if(this->Col>=this->BranchingFactor)
			return true;
		return false;
	}
	void IncCol()
	{
		this->Col++;
	}
	TimeRange *GetRange()
	{
		return this->TRange;
	}
	void ExpTRange(int T)
	{
		TimeRange *CurrentRange=this->GetRange();
		//CurrentRange->T0=CurrentRange->T1=T;
		if(this->ColLeaf==1)
			CurrentRange->T0=CurrentRange->T1=T;
		if(CurrentRange->T1<T)
			CurrentRange->T1=T;
	}
	bool IntersectionDot(int T)
	{
		if(this->TRange->T0>=T && this->TRange->T1<=T)
			return true;
		return false;
	}
	bool IntersectionRange(TimeRange *TR)
	{
		if(this->TRange->T0 > TR->T1 || this->TRange->T1 < TR->T0)
			return false;
		return true;
	}
};

struct RTreeNodeTimeList
{
	RTreeNodeTime *Elem;
	RTreeNodeTimeList *Next;
};