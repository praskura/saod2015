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
	this->ColTraject=0;
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
	//разбивает криво, не знаю что делать, что по уыеличению площади,что по увеличению расстояния, получается одно и тоже
	//мб так и надо...
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
		/*NodeListElem *Elem=new NodeListElem;
		Elem->Elem=InsertNode;
		Elem->Next=this->Childs;
		this->Childs=Elem;*/
		NodeListElem *Childs=this->Childs;
		//NodeListElem *Pre=this->Childs;
		this->Childs=NULL;
		for(NodeListElem *Tmp = /*this->*/Childs; Tmp; Tmp=Tmp->Next)
		{
			double DistanseFromMbrToFirstCenter=FirstCenter->Distanse(Tmp->Elem->GetMBR());
			double DistanseFromMbrToSecondCenter=SecondCenter->Distanse(Tmp->Elem->GetMBR());
			if(DistanseFromMbrToFirstCenter < DistanseFromMbrToSecondCenter)
			//double ExpMbrFromMbrToFirstCenter=FirstCenter->ExpSOfMBR(Tmp->Elem->GetMBR());
			//double ExpMbrFromMbrToSecondCenter=SecondCenter->ExpSOfMBR(Tmp->Elem->GetMBR());
			//if(ExpMbrFromMbrToFirstCenter < ExpMbrFromMbrToSecondCenter)
			{
				NewNode->InsertNode(Tmp->Elem);
				NewNode->ExpMBRNode();
				//FirstCenter->SetMbr(NewNode->GetMBR());
				//if(Tmp==this->Childs)
				//{
					//this->Childs=this->Childs->Next;
					//delete Pre;
					//Pre=this->Childs;
				//}
				//else
				//{
					//Pre->Next=Pre->Next->Next;
					//delete Tmp;
					//Tmp=Pre->Next;
				//}
			}
			else
			{
				//this->Col++;
				//Pre=Tmp;
				//Tmp=Tmp->Next;
				this->InsertNode(Tmp->Elem);
				this->ExpMBRNode();
				//SecondCenter->SetMbr(this->GetMBR());
			}
		}
	}
	delete FirstCenter;
	delete SecondCenter;
	this->ExpMBRNode();
	NewNode->ExpMBRNode();
	return NewNode;
}

