// H_Tower.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
//int c = 0;

int HT(int from, int to, int tmp, int n)
{
	int m = 0;
	if (n == 1)
	{
		printf("%d -> %d\n", from, to);
		return 1;
	}
	m += HT(from, tmp, to, n-1);
	m++; printf("%d -> %d\n", from, to);
	m += HT(tmp, to, from, n-1);
	return m;
}
int _tmain(int argc, _TCHAR* argv[])
{
	int n = 3;
	printf("%d", HT(1,2,3, n));
	getch();
	return 0;
}

