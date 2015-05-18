//#include "stdafx.h"
#include "Stack.h"

NodeStack::NodeStack()
{
	this->Head=NULL;
}

void NodeStack::Push(Node *Inserted)
{
	StackElem *Tmp=new StackElem;
	Tmp->Elem=Inserted;
	Tmp->Next=this->Head;
	this->Head=Tmp;
}

Node* NodeStack::Pop()
{
	if(this->Head==NULL)
		return NULL;
	StackElem* Elem=this->Head;
	this->Head=this->Head->Next;
	Node *TmpElem=Elem->Elem;
	delete Elem;
	return TmpElem;
}

void NodeStack::Delete()
{
	while(this->Pop());
}

Node* NodeStack::Look()
{
	return this->Head->Elem;
}

NodeStack:: ~NodeStack()
{
	this->Delete();
}

TBNodeStack::TBNodeStack()
{
	this->Head=NULL;
}

void TBNodeStack::Push(TBNode *Inserted)
{
	TBStackElem *Tmp=new TBStackElem;
	Tmp->Elem=Inserted;
	Tmp->Next=this->Head;
	this->Head=Tmp;
}

TBNode* TBNodeStack::Pop()
{
	if(this->Head==NULL)
		return NULL;
	TBStackElem* Elem=this->Head;
	this->Head=this->Head->Next;
	TBNode *TmpElem=Elem->Elem;
	delete Elem;
	return TmpElem;
}

void TBNodeStack::Delete()
{
	while(this->Pop());
}

TBNode* TBNodeStack::Look()
{
	return this->Head->Elem;
}

TBNodeStack:: ~TBNodeStack()
{
	this->Delete();
}

RTreeLeafNodeStack::RTreeLeafNodeStack()
{
	this->Head=NULL;
}

void RTreeLeafNodeStack::Push(RTreeNodeLeaf *Inserted)
{
	RTreeLeafElem *Tmp=new RTreeLeafElem;
	Tmp->Elem=Inserted;
	Tmp->Next=this->Head;
	this->Head=Tmp;
}

RTreeNodeLeaf* RTreeLeafNodeStack::Pop()
{
	if(this->Head==NULL)
		return NULL;
	RTreeLeafElem* Elem=this->Head;
	this->Head=this->Head->Next;
	RTreeNodeLeaf *TmpElem=Elem->Elem;
	delete Elem;
	return TmpElem;
}

void RTreeLeafNodeStack::Delete()
{
	while(this->Pop());
}

RTreeNodeLeaf* RTreeLeafNodeStack::Look()
{
	return this->Head->Elem;
}

RTreeLeafNodeStack:: ~RTreeLeafNodeStack()
{
	this->Delete();
}

RTreeTimeNodeStack::RTreeTimeNodeStack()
{
	this->Head=NULL;
}

void RTreeTimeNodeStack::Push(RTreeNodeTime *Inserted)
{
	RTreeTimeElem *Tmp=new RTreeTimeElem;
	Tmp->Elem=Inserted;
	Tmp->Next=this->Head;
	this->Head=Tmp;
}

RTreeNodeTime* RTreeTimeNodeStack::Pop()
{
	if(this->Head==NULL)
		return NULL;
	RTreeTimeElem* Elem=this->Head;
	this->Head=this->Head->Next;
	RTreeNodeTime *TmpElem=Elem->Elem;
	delete Elem;
	return TmpElem;
}

void RTreeTimeNodeStack::Delete()
{
	while(this->Pop());
}

RTreeNodeTime* RTreeTimeNodeStack::Look()
{
	return this->Head->Elem;
}

RTreeTimeNodeStack:: ~RTreeTimeNodeStack()
{
	this->Delete();
}
	