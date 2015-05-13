#pragma once

#ifndef NODE
#include "Node.h"
#endif

struct StackElem
{
	Node *Elem;
	StackElem *Next;
};

class NodeStack
{
private:
	StackElem *Head;
public:
	NodeStack();
	void Push(Node *Tmp);
	Node* Pop();
	void Delete();
	Node* Look();
	~NodeStack();
};

struct TBStackElem
{
	TBNode *Elem;
	TBStackElem *Next;
};

class TBNodeStack
{
private:
	TBStackElem *Head;
public:
	TBNodeStack();
	void Push(TBNode *Tmp);
	TBNode* Pop();
	void Delete();
	TBNode* Look();
	~TBNodeStack();
};