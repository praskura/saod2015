#pragma once
#include "stdafx.h"
#include <math.h>
#ifndef STACK
#include "Stack.h"
#endif
#ifndef NODE
#include "Node.h"
#endif

class Object
{
private:
	MBR *Mbr;
	int X;
	int Y;
	int Number;
public:
	Object();
	Object(int X, int Y, MBR &Mbr,int Number);
	~Object();
};

struct TrajectObj
{
	int Number;
	int X;
	int Y;
};


class StrTree
{
private:
	Node *Root;
	int CoefExp;
	int Levels;
	NodeStack* Stack;
	Node* FindNode(Node *Root, Traject *Trajectory);
	Node* ChooseNode(Node *Tmp, Traject *Trajectory, Node* CheckedNode);
	Node* FindNodeForSearch(Node* Root, MBR *Mbr, int Number);
	void ExtendRoot(Traject *Trajectory, int T0, int T1);
	Node* InsertNewNodeDependOfDistanse(Node *Root, Traject *Trajectory);
	TrajectList* FindTrajectories(Node *Root, MBR* Mbr,int Number);

public:
	StrTree(int BranchingFactor,int CoefExp);
	//~StrTree();
	void InsertTrajectory(Traject* NewTraject);

	int Extend(Node* Tmp, Node* Tmp2)
	{
		MBR *RootMbr=Tmp->GetMBR();
		MBR *MbrNewRoot=Tmp2->GetMBR();
		double RootXC=abs(RootMbr->Right + (RootMbr->Right-RootMbr->Left)/2);
		double RootYC=abs(RootMbr->Top + (RootMbr->Top-RootMbr->Bottom)/2);
		double NewRootXC=abs(MbrNewRoot->Right + (MbrNewRoot->Right-MbrNewRoot->Left)/2);
		double NewRootYC=abs(MbrNewRoot->Top + (MbrNewRoot->Top-MbrNewRoot->Bottom)/2);
		double X=abs(RootXC-NewRootXC);
		double Y=abs(RootYC-NewRootYC);
		double Lenth=pow((X*X)+(Y*Y),0.5);
		if(Lenth > Tmp->GetDec())
			return 0;
		return Lenth+1;
	}

	void InsertNode(Node *InsertedNode)
	{
		Node *CurrentNode=NULL;
		Node *NodeFromPartition=InsertedNode;
		while(CurrentNode=this->Stack->Pop())
		{

			if(CurrentNode==this->Root)
			{
				if(this->Root->Leaf())
				{
					Node *NewRoot=new Node(this->Root->GetFactor(),*new MBR(0,0,0,0,0,0),this->CoefExp);
					NewRoot->InsertNode(this->Root);
					NewRoot->InsertNode(InsertedNode);
					this->Root=NewRoot;
					this->Root->ExpMBRNode();
					return;
				}
				else
				{
					if(!CurrentNode->InsertNode(NodeFromPartition))
					{
						NodeFromPartition=CurrentNode->NodePartitionNode(NodeFromPartition);
						Node *NewRoot=new Node(this->Root->GetFactor(),*new MBR(-1,-1,-1,-1,-1,-1),this->CoefExp);
						NewRoot->InsertNode(this->Root);
						NewRoot->InsertNode(NodeFromPartition);
						this->Root=NewRoot;
						this->Root->ExpMBRNode();
						return;
					}
					else
						return;
				}
			}
			if(!CurrentNode->InsertNode(NodeFromPartition))
			{
				NodeFromPartition=CurrentNode->NodePartitionNode(NodeFromPartition);
				CurrentNode->ExpMBRNode();
			}
			else
			{
				CurrentNode->ExpMBRNode();
				this->ExpCoefs();
				return;
			}
		}

	}

	void ExpCoefs()
	{
		Node *CurrentNode=NULL;
		while(CurrentNode=this->Stack->Pop())
			CurrentNode->ExpMBRNode();
	}
	TrajectObj* FindTrajectory(MBR *Mbr, int Number, int &Col);
	
};