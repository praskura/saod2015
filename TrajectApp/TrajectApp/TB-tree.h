#pragma once
#include <math.h>
#ifndef STACK
#include "Stack.h"
#endif
#ifndef NODE
#include "Node.h"
#endif


//struct TrajectObj
//{
//	int Number;
//	int X;
//	int Y;
//};

class TBtree
{
private:
	TBNodeStack* Stack;
	TBNode *Root;
	int CoefExp;
	TBNode* FindNode(TBNode *Root, Traject *Trajectory);
	TBNode* ChooseNode(TBNode *Tmp, Traject *Trajectory, Node* CheckedNode);
	TBNode* FindFirstNode(TBNode *Root, MBR* Mbr,int Number);
	void TBtree::InsertNode(TBNode *InsertedNode);
	//void InsertNewNodeDependOfDistanse(TBNode *InsertedNode, Traject *Trajectory);
public:
	TBtree(int BranchingFactor,int CoefExp);
	void InsertTrajectory(Traject* NewTraject);
	
	void ExpCoefs()
	{
		TBNode *CurrentNode=NULL;
		while(CurrentNode=this->Stack->Pop())
			CurrentNode->ExpMBRNode();
	}

	TrajectObj* FindTrajectory(MBR *Mbr, int Number, int &Col);
};