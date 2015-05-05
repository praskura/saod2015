#pragma once
#include "stdafx.h"
#include "math.h"
#include"MBR.h"


#ifndef TRAJECT
#include "Trajectory.h"
#endif

struct NodeListElem;

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