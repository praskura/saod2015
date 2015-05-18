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

struct RTreeLeafElem
{
	RTreeNodeLeaf *Elem;
	RTreeLeafElem *Next;
};

class RTreeLeafNodeStack
{
private:
	RTreeLeafElem *Head;
public:
	RTreeLeafNodeStack();
	void Push(RTreeNodeLeaf *Tmp);
	RTreeNodeLeaf* Pop();
	void Delete();
	RTreeNodeLeaf* Look();
	~RTreeLeafNodeStack();
};

struct RTreeTimeElem
{
	RTreeNodeTime *Elem;
	RTreeTimeElem *Next;
};

class RTreeTimeNodeStack
{
private:
	RTreeTimeElem *Head;
public:
	RTreeTimeNodeStack();
	void Push(RTreeNodeTime *Tmp);
	RTreeNodeTime* Pop();
	void Delete();
	RTreeNodeTime* Look();
	~RTreeTimeNodeStack();
};