#pragma once
//#include "stdafx.h"
#include "R_tree_leaf.h"
#include "R_tree_time.h"
#include "MBR.h"
#include "Node.h"

struct ElemSETIArr
{
	MBR *Mbr;
	RTreeLeaf* TrajectoriesTree;
	LeafOfTraject *CurrentLeaf;
	int TimeOfCurrentLeaf;
	ElemSETIArr()
	{
		this->Mbr=NULL;
		this->TrajectoriesTree=new RTreeLeaf(3);
		this->CurrentLeaf=new LeafOfTraject;
		this->CurrentLeaf->ID_Leaf=-1;
		this->CurrentLeaf->Trajectories=NULL;
		this->TimeOfCurrentLeaf=-1;
	}
};

struct ElemOfFront
{
	int X;
	int Y;
	int T0;
	int Number;
	ElemOfFront()
	{
		this->X=this->Y=this->T0=this->Number=-1;
	}
};

class SETI
{
private:
	int testint;
	ElemSETIArr *ArrOfRegions;
	ElemOfFront *Front;
	RTreeTime *TimeIntervals;
	int ColObjects;
	int ColRegions;
	int CurrentID;
	int GetID()
	{
		int ID=this->CurrentID;
		this->CurrentID++;
		return ID;
	}
	ElemOfFront* FindPreDot(int Number)
	{
		for(int i=0; i<this->ColObjects; i++)
		{
			if(this->Front[i].Number==-1)
				return NULL;
			if(this->Front[i].Number==Number)
				return &this->Front[i];

		}

	}
	void InsertDotInFront(int X,int Y,int T,int Number)
	{
		for(int i=0; i<this->ColObjects; i++)
		{
			if(this->Front[i].Number==-1)
			{
				this->Front[i].Number=Number;
				this->Front[i].T0=T;
				this->Front[i].X=X;
				this->Front[i].Y=Y;
				return;
			}
		}
	}
	ElemSETIArr* FindRegion(MBR* MbrTj)
	{
		for(int i=0; i<this->ColRegions; i++)
		{
			if(this->ArrOfRegions[i].Mbr->IntersectionSETI(*MbrTj))
			{
				this->testint=i;
				return &this->ArrOfRegions[i];
			}
		}
	}

public:
	SETI(int ColObjects, int Width, int Height, int Coef);
	void InsertDot(int X,int Y, int T, int Number);
	void InsertTrajectory(Traject *Trajectory);
	TrajectObj* FindTrajectory(MBR *Mbr, int Number, int &Col);
};