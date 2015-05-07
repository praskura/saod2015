struct TmpIndexingData
{
	int NumOfObjects;
	int IndexingInterval;
	int FieldSize;
	POINT *pnts1, *pnts2;
	TmpIndexingData()
	{
		NumOfObjects = IndexingInterval = FieldSize = 0;
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
	TmpQueryData()
	{
		IDsQ = IDs = NULL;
		NumOfQueryObjects = TimeFrom = TimeTo = MBRx0 = 
			MBRy0 = MBRx1 = MBRy1 = 0;
	}
};