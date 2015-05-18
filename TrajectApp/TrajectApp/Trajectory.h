#pragma once
//#include "stdafx.h"
//#include "Node.h"
#define NULL 0
//struct MBR;

struct Traject
{
	int X0;
	int Y0;
	int X1;
	int Y1;
	int Direction;
	int Number;
	int T0;
	int T1;
	Traject()
	{
		this->Direction=0;
		this->X0=this->X1=this->Y0=this->Y1=this->Number=this->T0=this->T1=0;
	}
	Traject(int X0,int Y0, int X1, int Y1, int Direction, int Number, int T0, int T1)
	{
		this->Direction=Direction;
		this->X0=X0;
		this->Y0=Y0;
		this->X1=X1;
		this->Y1=Y1;
		this->Number=Number;
		this->T0=T0;
		this->T1=T1;
	}
	bool Connected(Traject *Trajectory)
	{
		if(Trajectory->X0==this->X1 && Trajectory->Y0==this->Y1 && Trajectory->T0==this->T1)
			return true;
		return false;
	}

	bool ConnectedLeft(int X, int T, int Number)
	{
		if(X==this->X0 && T==this->T0 && this->Number==Number)
			return true;
		return false;
	}

	bool Intersection(int X, /*int Y,*/ int T, int Number)
	{
		if(/*(*/(this->X0 <= X && this->X1 > X) /*|| (this->X0 >= X))*/ && (this->T0==T) && (this->Number==Number))
			return true;
		return false;
	}

	bool MbrIncludedTraject(int X0, int X1, int Y0, int Y1, int T0, int T1, int Number)
	{
		if(X0<=this->X0 && X1>=this->X1 && Y0<=this->Y0 && Y1>=this->Y1 && T0<=this->T0 && T1>=this->T1 && this->Number==Number)
			return true;
		return false;
	}

	bool NotIncludedRight(int X0, int X1, int Y0, int Y1, int T0, int T1)
	{
		if(this->X0 >= X1 || this->X1 > X1 ||  this->Y0 >= Y1 || this->Y1 > Y1 || this->T0 >=T1 || this->T1 > T1)
			return true;
		return false;
	}

	bool NotIncludedLeft(int X0, int X1, int Y0, int Y1, int T0, int T1)
	{
		if(this->X0 < X0 || this->X1 <= X0 ||  this->Y0 < Y0 || this->Y1 <= Y0 || this->T0 < T0 || this->T1 <= T0)
			return true;
		return false;
	}

};