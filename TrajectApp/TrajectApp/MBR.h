#pragma once
#include "Trajectory.h"

struct MBR
{
//private:
public:
	int Top;
	int Left;
	int Right;
	int Bottom;
	int T0;
	int T1;
//public:
	MBR(){ this->Bottom=this->Left=this->Right=this->T0=this->T1=this->Top=0; }
	MBR(Traject *Trajectory)
	{
		if(Trajectory->X0>Trajectory->X1)
		{
			this->Right=Trajectory->X0;
			this->Left=Trajectory->X1;
		}
		else
		{
			this->Right=Trajectory->X1;
			this->Left=Trajectory->X0;
		}
		if(Trajectory->Y0>Trajectory->Y1)
		{
			this->Top=Trajectory->Y0;
			this->Bottom=Trajectory->Y1;
		}
		else
		{
			this->Top=Trajectory->Y1;
			this->Bottom=Trajectory->Y0;
		}
		this->T0=Trajectory->T0;
		this->T1=Trajectory->T1;
	}

	MBR(int Top, int Bottom, int Left, int Right, int T0, int T1)
	{
		this->Bottom=Bottom;
		this->Top=Top;
		this->Left=Left;
		this->Right=Right;
		this->T0=T0;
		this->T1=T1;
	}

	MBR(MBR &Mbr)
	{
		this->Bottom=Mbr.Bottom;
		this->Top=Mbr.Top;
		this->Left=Mbr.Left;
		this->Right=Mbr.Right;
		this->T0=Mbr.T0;
		this->T1=Mbr.T1;
	}

	bool Intersection(MBR &Tmp)
	{
		if(Tmp.Left>this->Right || Tmp.Right<this->Left ||  Tmp.Top< this->Bottom || Tmp.Bottom>this->Top || Tmp.T0>this->T1 || Tmp.T1<this->T0)
			return false;
		return true;
	}

	bool IntersectionR(MBR &Tmp)
	{
		if(Tmp.Left>this->Right || Tmp.Right<this->Left ||  Tmp.Top< this->Bottom || Tmp.Bottom>this->Top)
			return false;
		return true;
	}

	void ExpansionH(int Top, int Bottom){ this->Bottom=Bottom; this->Top=Top;}
	void ExpansionW(int Left, int Right){ this->Left=Left; this->Right=Right;}
	void ExpansionT(int T0, int T1){ this->T0=T0; this->T1=T1;}
	void ExpMbr(MBR *Mbr)
	{
		if(this->Bottom > Mbr->Bottom || this->Bottom==-1)
			this->Bottom=Mbr->Bottom;

		if(this->Top < Mbr->Top || this->Top==-1)
			this->Top=Mbr->Top;

		if(this->Left > Mbr->Left || this->Left==-1)
			this->Left=Mbr->Left;

		if(this->Right < Mbr->Right || this->Right==-1)
			this->Right=Mbr->Right;

		if(this->T0 > Mbr->T0 || this->T0==-1)
			this->T0=Mbr->T0;

		if(this->T1 < Mbr->T1 || this->T1==-1)
			this->T1=Mbr->T1;
	}

	void ExpMbrOnKoef(MBR *Mbr, int Koef)
	{
		if(this->Bottom > Mbr->Bottom)
			this->Bottom+=Koef;

		if(this->Top > Mbr->Top)
			this->Top+=Koef;

		if(this->Left > Mbr->Left)
			this->Left+=Koef;

		if(this->Right < Mbr->Right)
			this->Right+=Koef;
	}

	double Distanse(MBR *Tmp)
	{
		double X0=(this->Right-this->Left)/2.0+this->Left;
		double Y0=(this->Top-this->Bottom)/2.0+this->Bottom;
		double X1=(Tmp->Right-Tmp->Left)/2.0+Tmp->Left;
		double Y1=(Tmp->Top-Tmp->Bottom)/2.0+Tmp->Bottom;
		double X=abs(X0-X1);
		double Y=abs(Y0-Y1);
		double Distanse=pow(X*X+Y*Y,0.5);
		return Distanse;
	}

	void Center(int &X,int &Y)
	{
		X=(this->Right-this->Left)/2;
		Y=(this->Top-this->Bottom)/2;
	}
	
	void SetMbr(MBR *Tmp)
	{
		this->Bottom=Tmp->Bottom;
		this->Left=Tmp->Left;
		this->Right=Tmp->Right;
		this->Top=Tmp->Top;
		this->T0=Tmp->T0;
		this->T1=Tmp->T1;
	}

	void SetMbrTraject(Traject *Trajectory)
	{
		if(Trajectory->X0>Trajectory->X1)
		{
			this->Right=Trajectory->X0;
			this->Left=Trajectory->X1;
		}
		else
		{
			this->Right=Trajectory->X1;
			this->Left=Trajectory->X0;
		}
		if(Trajectory->Y0>Trajectory->Y1)
		{
			this->Top=Trajectory->Y0;
			this->Bottom=Trajectory->Y1;
		}
		else
		{
			this->Top=Trajectory->Y1;
			this->Bottom=Trajectory->Y0;
		}
		this->T0=Trajectory->T0;
		this->T1=Trajectory->T1;
	}

	int S()
	{
		return abs(this->Bottom - this->Top)*abs(this->Right - this->Left);
	}

	double ExpSOfMBR(MBR *Tmp)
	{
		MBR *First=new MBR(*this);
		MBR *Second=new MBR(*Tmp);
		double S1=First->S();
		First->ExpMbr(Second);
		double S2=First->S();
		return S2-S1;
	}

};


