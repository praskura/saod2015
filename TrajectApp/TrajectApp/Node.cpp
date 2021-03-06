//#include "stdafx.h"
#include "Node.h"

void Node::ExpMBRNode()
{
	MBR *CurrentMBR=this->GetMBR();
	CurrentMBR->SetMbr(&MBR(-1,-1,-1,-1,-1,-1));
	if(!this->Leaf())
	{
		for(NodeListElem *Tmp = this->Childs; Tmp; Tmp=Tmp->Next)
			CurrentMBR->ExpMbr(Tmp->Elem->GetMBR());
	}
	else
		this->ExpMBRTraject();
}

Node::Node()
{
	this->BranchingFactor=0;
	this->Childs=NULL;
	this->Col=0;
	this->Mbr=NULL;
	this->Trajectories=NULL;
}

Node::Node(int BranchingFactor, MBR &Mbr, Traject* Trajectory, int DecExtend)
{
	this->BranchingFactor=BranchingFactor;
	this->Childs=NULL;
	this->Col=0;
	this->Mbr=&Mbr;
	this->Trajectories=NULL;
	this->InsertTraject(Trajectory);
	this->ColTraject=1;
	this->DecExtend=DecExtend;
}

Node::Node(int BranchingFactor, MBR &Mbr, int DecExtend)
{
	this->BranchingFactor=BranchingFactor;
	this->Childs=NULL;
	this->Col=0;
	this->Mbr=&Mbr;
	this->Trajectories=NULL;
	this->DecExtend=DecExtend;
	this->ColTraject=0;
}

bool Node::Full()
{
	if(this->Col>=this->BranchingFactor)
		return true;
	return false;
}

bool Node::InsertNode(Node *Tmp)
{
	if(!this->Full())
	{
		/*NodeListElem *NewElem=new NodeListElem;
		if(this->Childs==NULL)
		{
			NewElem->Elem=Tmp;
			this->Childs=NewElem;
			NewElem->Next=NULL;
			this->Col++;
			return true;
		}
		if(this->Childs->Elem->GetVol() >= Tmp->GetVol())
		{
			NewElem->Elem=Tmp;
			NewElem->Next=this->Childs;
			this->Childs=NewElem;
			this->Col++;
			return true;
		}
		if(this->Col==1)
		{
			NewElem->Elem=Tmp;
			NewElem->Next=NULL;
			this->Childs->Next=NewElem;
			this->Col++;
			return true;
		}
		NodeListElem *Pre=this->Childs;
		for(NodeListElem *Temp=this->Childs->Next; Temp; Temp=Temp->Next)
		{
			if(Temp->Elem->GetVol() >=  Tmp->GetVol())
			{
				NewElem->Next=Pre->Next;
				Pre->Next=NewElem;
				break;
			}
			Pre=Temp;
		}*/
		NodeListElem *NewElem=new NodeListElem;
		NewElem->Elem=Tmp;
		NewElem->Next=this->Childs;
		this->Childs=NewElem;
		this->Col++;
		return true;
	}
	return false;
}

Node *Node::NodePartitionTraject(Traject *Trajectory)
{
	this->InsertTraject(Trajectory);
	Node *NewNode=new Node(this->BranchingFactor,*new MBR(-1,-1,-1,-1,-1,-1),this->DecExtend);
	MBR *FirstCenter=new MBR(this->Trajectories->Trajetory);
	MBR *SecondCenter=new MBR(this->Trajectories->Trajetory);
	MBR *FirstMBR=new MBR(this->Trajectories->Trajetory);
	MBR *TmpMBR=new MBR(this->Trajectories->Trajetory);
	double Distanse=0;
	double MaxDistanse=0;
	if(this->Leaf())
	{
		for(TrajectList *Tmp = this->Trajectories; Tmp; Tmp=Tmp->Next)
		{
			TmpMBR->SetMbrTraject(Tmp->Trajetory);
			Distanse=TmpMBR->Distanse(FirstMBR);
			if(MaxDistanse < Distanse)
			{
				MaxDistanse=Distanse;
				FirstCenter->SetMbr(TmpMBR);
			}
		}
		MaxDistanse=0;
		for(TrajectList *Tmp = this->Trajectories; Tmp; Tmp=Tmp->Next)
		{
			TmpMBR->SetMbrTraject(Tmp->Trajetory);
			Distanse=TmpMBR->Distanse(FirstCenter);
			if(MaxDistanse < Distanse)
			{
				MaxDistanse=Distanse;
				SecondCenter->SetMbr(TmpMBR);
			}
		}
		delete FirstMBR;
		delete TmpMBR;
		this->ColTraject=0;
		TrajectList *Pre=this->Trajectories;
		for(TrajectList *Tmp = this->Trajectories; Tmp; /*Tmp=Tmp->Next*/)
		{
			double DistanseFromMbrToFirstCenter=FirstCenter->Distanse(&MBR(Tmp->Trajetory));
			double DistanseFromMbrToSecondCenter=SecondCenter->Distanse(&MBR(Tmp->Trajetory));
			if(DistanseFromMbrToFirstCenter < DistanseFromMbrToSecondCenter)
			{
				NewNode->InsertTraject(Tmp->Trajetory);
				if(Tmp==this->Trajectories)
				{
					this->Trajectories=this->Trajectories->Next;
					delete Pre;
					Pre=this->Trajectories;
				}
				else
				{
					Pre->Next=Pre->Next->Next;
					delete Tmp;
					Tmp=Pre->Next;
				}
			}
			else
			{
				this->ColTraject++;
				Pre=Tmp;
				Tmp=Tmp->Next;
			}
		}
	}
	NewNode->ExpMBRTraject();
	this->ExpMBRTraject();
	delete FirstCenter;
	delete SecondCenter;
	return NewNode;
}

Node* Node::NodePartitionNode(Node *InsertNode)
{
	NodeListElem *Elem=new NodeListElem();
	Elem->Elem=InsertNode;
	Elem->Next=this->Childs;
	this->Childs=Elem;
	Node *NewNode=new Node(this->BranchingFactor,*new MBR(-1,-1,-1,-1,-1,-1),this->DecExtend);
	MBR *FirstCenter=new MBR(-1,-1,-1,-1,-1,-1);
	MBR *SecondCenter=new MBR(-1,-1,-1,-1,-1,-1);
	MBR *FirstMBR=new MBR(-1,-1,-1,-1,-1,-1);
	MBR *TmpMBR=new MBR(-1,-1,-1,-1,-1,-1);
	double Distanse=0;
	double MaxDistanse=0;
	if(!this->Leaf())
	{
		FirstMBR->SetMbr(this->Childs->Elem->GetMBR());
		for(NodeListElem *Tmp = this->Childs; Tmp; Tmp=Tmp->Next)
		{
			TmpMBR->SetMbr(Tmp->Elem->GetMBR());
			Distanse=TmpMBR->Distanse(FirstMBR);
			if(MaxDistanse < Distanse)
			{
				MaxDistanse=Distanse;
				FirstCenter->SetMbr(TmpMBR);
			}
		}
		Distanse=0;
		MaxDistanse=0;
		for(NodeListElem *Tmp = this->Childs; Tmp; Tmp=Tmp->Next)
		{
			TmpMBR->SetMbr(Tmp->Elem->GetMBR());
			Distanse=TmpMBR->Distanse(FirstCenter);
			if(MaxDistanse < Distanse)
			{
				MaxDistanse=Distanse;
				SecondCenter->SetMbr(TmpMBR);
			}
		}
		delete FirstMBR;
		delete TmpMBR;
		this->Col=0;
		NodeListElem *Childs=this->Childs;
		this->Childs=NULL;
		for(NodeListElem *Tmp = /*this->*/Childs; Tmp; Tmp=Tmp->Next)
		{
			double DistanseFromMbrToFirstCenter=FirstCenter->Distanse(Tmp->Elem->GetMBR());
			double DistanseFromMbrToSecondCenter=SecondCenter->Distanse(Tmp->Elem->GetMBR());
			if(DistanseFromMbrToFirstCenter < DistanseFromMbrToSecondCenter)
			{
				NewNode->InsertNode(Tmp->Elem);
				NewNode->ExpMBRNode();
			}
			else
			{
				this->InsertNode(Tmp->Elem);
				this->ExpMBRNode();
			}
		}
	}
	delete FirstCenter;
	delete SecondCenter;
	this->ExpMBRNode();
	NewNode->ExpMBRNode();
	return NewNode;
}


TBNode::TBNode(int BranchingFactor, MBR &Mbr, int DecExtend):Node(BranchingFactor,Mbr,DecExtend)
{
	this->NextNodeWithTraject=NULL;
	this->Childs=NULL;
	this->PreNodeWithTraject=NULL;
}

TBNode* TBNode::NodePartitionTraject(Traject *Trajectory)
{
	TBNode *NewNode=new TBNode(this->GetFactor(),*new MBR(-1,-1,-1,-1,-1,-1),this->GetDec());
	NewNode->InsertTraject(Trajectory);
	NewNode->ExpMBRTraject();
	return NewNode;
}

TBNode* TBNode::NodePartitionNode(TBNode *NewNode)
{
	TBNode *NewTBNode=new TBNode(this->GetFactor(),*new MBR(-1,-1,-1,-1,-1,-1),this->GetDec());
	NewTBNode->InsertNode(NewNode);
	NewTBNode->ExpMBRNode();
	return NewTBNode;
}

void TBNode::InsertTraject(Traject *Trajectory)
{
	TrajectList* Elem=new TrajectList;
	if(this->Trajectories==NULL)
	{
		Elem->Trajetory=Trajectory;
		Elem->Next=this->Trajectories;
		this->Trajectories=Elem;
		this->IncColTraject();
		this->ExpMBRTraject();
		return;
	}
	for(TrajectList *Tmp = this->Trajectories; Tmp; Tmp=Tmp->Next)
	{
		if(Tmp->Next==NULL)
		{
			Elem->Trajetory=Trajectory;
			Elem->Next=Tmp->Next;
			Tmp->Next=Elem;
			this->IncColTraject();
			this->ExpMBRTraject();
			return;
		}
	}
	
}
	
bool TBNode::InsertNode(TBNode*Tmp)
{
	if(!this->Full())
	{
		TBNodeListElem* Elem=new TBNodeListElem;
		if(this->Childs==NULL)
		{
			Elem->Elem=Tmp;
			Elem->Next=this->Childs;
			this->Childs=Elem;
			this->IncCol();
			return true;
		}
		for(TBNodeListElem *Temp=this->Childs; Temp; Temp=Temp->Next)
		{
			if(Temp->Next==NULL)
			{
				Elem->Elem=Tmp;
				Elem->Next=Temp->Next;
				Temp->Next=Elem;
				this->IncCol();
				return true;
			}
		}
	}

	return false;
}

void TBNode::ExpMBRNode()
{
	MBR *CurrentMBR=this->GetMBR();
	CurrentMBR->SetMbr(&MBR(-1,-1,-1,-1,-1,-1));
	if(!this->Leaf())
	{
		for(TBNodeListElem *Tmp = this->Childs; Tmp; Tmp=Tmp->Next)
			CurrentMBR->ExpMbr(Tmp->Elem->GetMBR());
	}
	else
		this->ExpMBRTraject();
}


RTreeNodeLeaf::RTreeNodeLeaf(int BranchingFactor)
{
	this->BranchingFactor=BranchingFactor;
	this->Childs=NULL;
	this->ColLeaf=0;
	this->IDRange=new Range;
	this->Leafs=NULL;
	this->Col=0;
}

RTreeNodeLeaf* RTreeNodeLeaf::NodePartitionLeaf(LeafOfTraject *NewLeaf)
{
	RTreeNodeLeaf *NewNode=new RTreeNodeLeaf(this->GetFactor());
	NewNode->InsertLeaf(NewLeaf);
	NewNode->ExpRangeNode();
	return NewNode;
}

RTreeNodeLeaf* RTreeNodeLeaf::NodePartitionNode(RTreeNodeLeaf *NewNode)
{
	RTreeNodeLeaf *NewRTreeLeafNode=new RTreeNodeLeaf(this->GetFactor());
	NewRTreeLeafNode->InsertNode(NewNode);
	NewRTreeLeafNode->ExpRangeNode();
	return NewRTreeLeafNode;
}

void RTreeNodeLeaf::InsertLeaf(LeafOfTraject *NewLeaf)
{
	ListOfLeafs* Elem=new ListOfLeafs;
	if(this->Leafs==NULL)
	{
		Elem->Elem=NewLeaf;
		Elem->Next=this->Leafs;
		this->Leafs=Elem;
		this->IncColLeafs();
		//this->ExpMBRTraject();
		this->ExpRangeLeafs();
		return;
	}
	for(ListOfLeafs *Tmp = this->Leafs; Tmp; Tmp=Tmp->Next)
	{
		if(Tmp->Next==NULL)
		{
			Elem->Elem=NewLeaf;
			Elem->Next=NULL;
			Tmp->Next=Elem;
			this->IncColLeafs();
			//this->ExpMBRTraject();
			this->ExpRangeLeafs();
			return;
		}
	}
}

bool RTreeNodeLeaf::InsertNode(RTreeNodeLeaf *NewNode)
{
	if(!this->Full())
	{
		RTreeNodeLeafList* Elem=new RTreeNodeLeafList;
		if(this->Childs==NULL)
		{
			Elem->Elem=NewNode;
			Elem->Next=this->Childs;
			this->Childs=Elem;
			this->IncCol();
			return true;
		}
		for(RTreeNodeLeafList *Temp=this->Childs; Temp; Temp=Temp->Next)
		{
			if(Temp->Next==NULL)
			{
				Elem->Elem=NewNode;
				Elem->Next=Temp->Next;
				Temp->Next=Elem;
				this->IncCol();
				return true;
			}
		}
	}
	return false;
}

bool RTreeNodeLeaf::Intersection(RTreeNodeLeaf *Node)
{
	Range *Tmp=Node->GetRange();
	return this->IDRange->Intersection(Tmp);
}

void RTreeNodeLeaf::ExpRangeNode()
{
	
	Range *CurrentRange=this->GetRange();
	CurrentRange->ID0=CurrentRange->ID1=-1;
	if(!this->Leaf())
	{
		for(RTreeNodeLeafList *Tmp = this->Childs; Tmp; Tmp=Tmp->Next)
			CurrentRange->ExpRange(Tmp->Elem->GetRange());
	}
	else
		this->ExpRangeLeafs();

}


RTreeNodeTime::RTreeNodeTime(int BranchingFactor)
{
	this->BranchingFactor=BranchingFactor;
	this->Childs=NULL;
	this->ColLeaf=0;
	this->TRange=new TimeRange;
	this->IDs=NULL;
	this->Col=0;
}

RTreeNodeTime* RTreeNodeTime::NodePartitionID(int ID,int T)
{
	RTreeNodeTime *NewNode=new RTreeNodeTime(this->GetFactor());
	NewNode->InsertID(ID);

	NewNode->ExpTRangeNode(T);
	return NewNode;
}

RTreeNodeTime* RTreeNodeTime::NodePartitionNode(RTreeNodeTime *NewNode)
{
	RTreeNodeTime *NewRTreeLeafNode=new RTreeNodeTime(this->GetFactor());
	NewRTreeLeafNode->InsertNode(NewNode);
	NewRTreeLeafNode->ExpTRangeNode(0);
	return NewRTreeLeafNode;
}

void RTreeNodeTime::InsertID(int ID)
{
	ListOfID* Elem=new ListOfID;
	if(this->IDs==NULL)
	{
		Elem->Elem=ID;
		Elem->Next=this->IDs;
		this->IDs=Elem;
		this->IncColLeafs();
		//this->ExpMBRTraject();
		return;
	}
	for(ListOfID *Tmp = this->IDs; Tmp; Tmp=Tmp->Next)
	{
		if(Tmp->Next==NULL)
		{
			Elem->Elem=ID;
			Elem->Next=this->IDs;
			this->IDs=Elem;
			this->IncColLeafs();
			//this->ExpMBRTraject();
			return;
		}
	}
}

bool RTreeNodeTime::InsertNode(RTreeNodeTime *NewNode)
{
	if(!this->Full())
	{
		RTreeNodeTimeList* Elem=new RTreeNodeTimeList;
		if(this->Childs==NULL)
		{
			Elem->Elem=NewNode;
			Elem->Next=this->Childs;
			this->Childs=Elem;
			this->IncCol();
			return true;
		}
		for(RTreeNodeTimeList *Temp=this->Childs; Temp; Temp=Temp->Next)
		{
			if(Temp->Next==NULL)
			{
				Elem->Elem=NewNode;
				Elem->Next=Temp->Next;
				Temp->Next=Elem;
				this->IncCol();
				return true;
			}
		}
	}
	return false;
}

bool RTreeNodeTime::Intersection(RTreeNodeTime *Node)
{
	TimeRange *Tmp=Node->GetRange();
	return this->TRange->Intersection(Tmp);
}

void RTreeNodeTime::ExpTRangeNode(int T)
{
	
	TimeRange *CurrentRange=this->GetRange();
	//CurrentRange->T0=CurrentRange->T1=-1;
	if(!this->Leaf())
	{
		for(RTreeNodeTimeList *Tmp = this->Childs; Tmp; Tmp=Tmp->Next)
			CurrentRange->ExpRange(Tmp->Elem->GetRange());
	}
	else
		this->ExpTRange(T);

}
