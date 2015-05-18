//#include "stdafx.h"
#include "SETI.h"
#include <iostream>
using namespace std;

SETI::SETI(int ColObjects, int Width, int Height, int Coef)
{
	this->CurrentID=0;
	this->TimeIntervals=new RTreeTime(3);
	this->ColObjects=ColObjects;
	int ColRegions=Width/Coef * Height/Coef;
	this->Front=new ElemOfFront[ColObjects];
	this->ArrOfRegions=new ElemSETIArr[ColRegions];
	int ColX=Width/Coef;
	int ColY= Height/Coef;
	int ColNodes= ColX*ColY;
	this->ColRegions=ColNodes;
	int Top=Coef;
	int Left=0;
	int Bottom=0;
	int Right=Coef;
	for(int i=0; i<ColY; i++)
	{
		for(int j=0; j<ColX; j++)
		{
			if(i*ColX+j==80)
				int p=4;
			MBR *CurrentMBR=new MBR(Top,Bottom,Left,Right,-1,-1);
			this->ArrOfRegions[i*ColX+j].Mbr=CurrentMBR;
			Left+=Coef;
			Right+=Coef;

		}
		Left=0;
		Right=Coef;
		Top+=Coef;
		Bottom+=Coef;
	}

}

void SETI::InsertDot(int X,int Y, int T, int Number)
{
	ElemOfFront *PreDot=this->FindPreDot(Number);
	if(!PreDot)
		this->InsertDotInFront(X,Y,T,Number);
	else
	{
		Traject *Tj=new Traject(PreDot->X,PreDot->Y,X,Y,1,Number,PreDot->T0,T);
		PreDot->T0=T;
		PreDot->X=X;
		PreDot->Y=Y;
		this->InsertTrajectory(Tj);
	}
}

void SETI::InsertTrajectory(Traject *Trajectory)
{
	ElemSETIArr *Region=this->FindRegion(&MBR(Trajectory));
	if(Region->TimeOfCurrentLeaf==Trajectory->T1 || Region->TimeOfCurrentLeaf==-1)
	{
		Region->CurrentLeaf->InsertTrajectory(Trajectory);
		Region->TimeOfCurrentLeaf=Trajectory->T1;
		return;
	}
	else
	{
		if(Trajectory->X0 > 400 || Trajectory->X1 > 400 || Trajectory->Y0 > 400 || Trajectory->Y1 > 400)//для проверки траектории на возможность выхода за экран
			int p=4;
		LeafOfTraject *NewLeaf=new LeafOfTraject;
		NewLeaf->ID_Leaf=-1;
		Region->CurrentLeaf->ID_Leaf=this->GetID();
		Region->TrajectoriesTree->InsertLeaf(Region->CurrentLeaf);
		this->TimeIntervals->InsertID(Region->TimeOfCurrentLeaf,Region->CurrentLeaf->ID_Leaf);
		NewLeaf->InsertTrajectory(Trajectory);
		Region->CurrentLeaf=NewLeaf;
		Region->TimeOfCurrentLeaf=Trajectory->T1;
	}
}

TrajectObj* SETI::FindTrajectory(MBR *Mbr, int Number, int &Col)
{
	//сохраняем листы
	for(int i=0; i<this->ColRegions; i++)
	{
		if(this->ArrOfRegions[i].CurrentLeaf->Trajectories!=NULL)
		{
			LeafOfTraject *NewLeaf=new LeafOfTraject;
			NewLeaf->ID_Leaf=-1;
			this->ArrOfRegions[i].CurrentLeaf->ID_Leaf=this->GetID();
			this->ArrOfRegions[i].TrajectoriesTree->InsertLeaf(this->ArrOfRegions[i].CurrentLeaf);
			this->TimeIntervals->InsertID(this->ArrOfRegions[i].TimeOfCurrentLeaf,this->ArrOfRegions[i].CurrentLeaf->ID_Leaf);
			this->ArrOfRegions[i].CurrentLeaf=NewLeaf;
			this->ArrOfRegions[i].TimeOfCurrentLeaf=-1;
		}
	}
	//фильтр по времени
	TimeRange *TRange=new TimeRange;
	TRange->T0=Mbr->T0;
	TRange->T1=Mbr->T1;
	ListOfID* IDs=this->TimeIntervals->FindIDs(this->TimeIntervals->GetRoot(),TRange);
	ListOfLeafs *List=NULL;
	//фильтр по пространству
	for(int i=0; i<this->ColRegions; i++)
	{
		if(this->ArrOfRegions[i].Mbr->IntersectionSETI(*Mbr))
		{
			ListOfLeafs *NewList = this->ArrOfRegions[i].TrajectoriesTree->FindLeafs(this->ArrOfRegions[i].TrajectoriesTree->GetRoot(),IDs);
			if(NewList)
			{
				for(ListOfLeafs *Tmp=NewList; Tmp; Tmp=Tmp->Next)
				{
					if(Tmp->Next==NULL)
					{
						Tmp->Next=List;
						List=NewList;
						break;
					}
				}
			}
		}
	}
	TrajectList* TjList=NULL;
	for(ListOfLeafs *Tmp=List; Tmp; Tmp=Tmp->Next)
	{
		for(TrajectList *Temp=Tmp->Elem->Trajectories; Temp; Temp=Temp->Next)
		{
			MBR TempMbr(Temp->Trajetory);
			if(Temp->Trajetory->Number==Number && Mbr->IntersectionSETITime(TempMbr))
			{
				TrajectList *NewElem=new TrajectList;
				NewElem->Trajetory=Temp->Trajetory;
				NewElem->Next=TjList;
				TjList=NewElem;
			}
		}
	}
	int Count =TjList->Sort();
	Col=Count+1;
	TrajectObj *Arr=new TrajectObj[Count+1];
	int i=0;
	for(TrajectList *Tmp=TjList; Tmp; Tmp=Tmp->Next)
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
