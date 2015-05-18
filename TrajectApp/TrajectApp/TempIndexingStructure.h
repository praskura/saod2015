#include "Node.h"

struct TmpIndexingData
{
	int NumOfObjects;
	int IndexingInterval;
	int FieldSize;
	POINT *pnts1, *pnts2;
	TmpIndexingData()
	{
		NumOfObjects = IndexingInterval = 0;
		FieldSize = 20;
	}
};

struct QueryData
{

	TrajectObj *Arr[1000][1000];
	int* IDs;
	int NumOfQueryObjects;
	int TimeFrom;
	int TimeTo;
	int MBRx0;
	int MBRy0;
	int MBRx1;
	int MBRy1;
	int col;
	int StructureType;
	COLORREF* Colors;
	COLORREF currTextColor;
	int currRed, currGreen, currBlue;
	QueryData()
	{
		IDs = NULL;
		Colors = NULL;
		currTextColor = RGB(0,0,0);
		NumOfQueryObjects = TimeFrom = TimeTo = MBRx0 = 
			MBRy0 = MBRx1 = MBRy1 = col = 0;
	}
};

struct TmpQueryData : public QueryData
{
	COLORREF* Colors;
	COLORREF currTextColor;
	int currRed, currGreen, currBlue;
	TmpQueryData()
	{
		IDs = NULL;
		Colors = NULL;
		currTextColor = RGB(0,0,0);
		NumOfQueryObjects = TimeFrom = TimeTo = MBRx0 = 
			MBRy0 = MBRx1 = MBRy1 = 0;
	}
};