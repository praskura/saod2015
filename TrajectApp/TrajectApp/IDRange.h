#pragma once
//#include "stdafx.h"

struct Range
{
	int ID0;
	int ID1;
	Range()
	{
		this->ID0=-1;
		this->ID1=-1;
	}

	void ExpRange(Range *IDRange)
	{
		if(this->ID0>IDRange->ID0 || this->ID0==-1)
			this->ID0=IDRange->ID0;
			
		if(this->ID1<IDRange->ID1 || this->ID1==-1)
			this->ID1=IDRange->ID1;
	}
	bool Intersection(Range *Rg)
	{
		if(this->ID0>Rg->ID1 || this->ID1<this->ID0)
			return false;
		return true;
	}
};

struct TimeRange
{
	int T0;
	int T1;
	TimeRange()
	{
		this->T0=-1;
		this->T1=-1;
	}

	void ExpRange(TimeRange *TRange)
	{
		if(this->T0>TRange->T0 || this->T0==-1)
			this->T0=TRange->T0;
			
		if(this->T1<TRange->T1 || this->T1==-1)
			this->T1=TRange->T1;
	}
	bool Intersection(TimeRange *Rg)
	{
		if(this->T0>Rg->T1 || this->T1<Rg->T0)
			return false;
		return true;
	}
};

struct IDRangeList
{
	Range *Elem;
	IDRangeList *Next;
};