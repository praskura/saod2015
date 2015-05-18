#pragma once
//#include "stdafx.h"
#include <math.h>
#ifndef STACK
#include "Stack.h"
#endif
#ifndef NODE
#include "Node.h"
#endif

class RTreeLeaf
{
private:
	RTreeLeafNodeStack* Stack;
	RTreeNodeLeaf *Root;
	int CoefExp;
	RTreeNodeLeaf* FindNode(RTreeNodeLeaf *Root, LeafOfTraject *Leaf);
	RTreeNodeLeaf* ChooseNode(RTreeNodeLeaf *Tmp, LeafOfTraject *Leaf, Node* CheckedNode);
	RTreeNodeLeaf* FindFirstNode(RTreeNodeLeaf *Root, Range* Rng, int Number);
	void RTreeLeaf::InsertNode(RTreeNodeLeaf *InsertedNode);
public:
	RTreeLeaf(int BranchingFactor);
	void InsertLeaf(LeafOfTraject *NewLeaf);
	
	void ExpCoefs()
	{
		RTreeNodeLeaf *CurrentNode=NULL;
		while(CurrentNode=this->Stack->Pop())
			CurrentNode->ExpRangeNode();
	}
	RTreeNodeLeaf* GetRoot()
	{
		return this->Root;
	}
	//ListOfID* FindRangeIDs(Range *IDRange);
	ListOfLeafs* FindLeafs(RTreeNodeLeaf *Root, ListOfID* IDs);
};
