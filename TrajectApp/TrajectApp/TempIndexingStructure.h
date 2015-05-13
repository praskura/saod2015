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

struct TmpQueryData
{
	int* IDs;
	int* IDsQ;
	int NumOfQueryObjects;
	int TimeFrom;
	int TimeTo;
	int MBRx0;
	int MBRy0;
	int MBRx1;
	int MBRy1;
	COLORREF* Colors;
	COLORREF currTextColor;
	int currRed, currGreen, currBlue;
	TmpQueryData()
	{
		IDsQ = IDs = NULL;
		Colors = NULL;
		currTextColor = RGB(0,0,0);
		NumOfQueryObjects = TimeFrom = TimeTo = MBRx0 = 
			MBRy0 = MBRx1 = MBRy1 = 0;
	}
};