#pragma once
//#include "stdafx.h"
#include <math.h>
#ifndef STACK
#include "Stack.h"
#endif
#ifndef NODE
#include "Node.h"
#endif

class RTreeTime
{
private:
	RTreeTimeNodeStack* Stack;
	RTreeNodeTime *Root;
	int CoefExp;
	RTreeNodeTime* FindNode(RTreeNodeTime *Root, int ID, int T);
	RTreeNodeTime* ChooseNode(RTreeNodeTime *Tmp, int ID, int T, Node* CheckedNode);
	//RTreeNodeTime* FindFirstNode(RTreeNodeTime *Root, Range* Rng, int Number);
	void RTreeTime::InsertNode(RTreeNodeTime *InsertedNode);

public:
	RTreeTime(int BranchingFactor);
	void InsertID(int T,int ID);
	RTreeNodeTime* GetRoot()
	{
		return this->Root;
	}
	void ExpCoefs(int T)
	{
		RTreeNodeTime *CurrentNode=NULL;
		while(CurrentNode=this->Stack->Pop())
			CurrentNode->ExpTRangeNode(T);
	}

	ListOfID* FindIDs(RTreeNodeTime *Root, TimeRange* TRange);
};