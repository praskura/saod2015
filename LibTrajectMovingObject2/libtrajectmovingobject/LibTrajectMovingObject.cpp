
#include "stdafx.h"
#include "STR-TREE.h"
#include <iostream>
#include <time.h>
#include<conio.h>


using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	StrTree a(4,10);
	clock_t t=clock();
	int x1=1;
	int x2=2;
	int y1=1;
	int y2=2;
	int t1=1;
	int t2=2;
	for(int i=0; i<1000000; i++)
	{
		Traject *Tr1=new Traject(x1,y1,x2,y2,1,5,t1,t2);
		x1=x2;
		x2++;
		y1=y2;
		y2++;
		t1=t2;
		t2++;
		a.InsertTrajectory(Tr1);
		if(i%1000000==0)
			cout<<i<<" ";
	}
	clock_t t3=clock();
	cout<<"time index"<<t3-t;
	int col=0;
	clock_t t4=clock();
	TrajectObj * Arr = a.FindTrajectory(&MBR(10000,1000,100,100000,100,100000),5,col);
	clock_t t5=clock();
	cout<<endl<<"time zapr"<<t5-t4<<endl<<endl<<endl;
	for(int i=0; i<col; i++)
		cout<<Arr[i].X<<" "<<Arr[i].Y<<endl;
	getch();
	return 0;
}

