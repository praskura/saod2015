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