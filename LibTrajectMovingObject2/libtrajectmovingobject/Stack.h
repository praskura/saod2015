#pragma once
#include "stdafx.h"

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
