#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
#pragma comment(lib, "Shlwapi.lib")

//МЭЙН ЛЁШИН
/*
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
}*/
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include <CommCtrl.h>
#include "TempIndexingStructure.h"
#include <Shlwapi.h>
#include <time.h>
#include "STR-TREE.h"

HINSTANCE hInst;
static int dTimerCommon, nTimerCommon, dTimerSec, nTimerSec, TimerVal;
int ProgressBarPos;
bool IsRecordComplited, IsIndexingComplited;
HWND ProgressBar;
HWND MainDlg;
HWND ComboBox;
HWND ButtonQuery;
TmpIndexingData CurrentIndexingData;
TmpQueryData CurrentQueryData;
StrTree STR_tree(4, 10);
int x1;
int x2;
int y;
int y2;
int t1;
int t2;


BOOL CALLBACK MainDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK IndexingDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK QueryDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void PaintField(HWND hWnd, HDC hDC, int FieldSize);
 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	hInst = hInstance;
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, MainDlgProc);
	return 0;
}

void PaintField(HWND hWnd, HDC hDC, int FieldSize)
{
	HPEN BackGround = CreatePen(PS_INSIDEFRAME, 300, RGB(0,0,0));
	SelectPen(hDC, BackGround);
	Rectangle(hDC, 11, 32, 411, 436);
	HPEN GreenLine = CreatePen(PS_SOLID, 1, RGB(0,125,0));
	SelectPen(hDC, GreenLine);
	int x;
	int y = 32;
	MoveToEx(hDC, 11, 32, NULL);
	for (x = 11; x < 411; x+=400/FieldSize)
	{
		MoveToEx(hDC, x, y, NULL);
		LineTo(hDC, x, y+403);
	}
	x = 11;
	for (; y < 432; y+=400/FieldSize)
	{
		MoveToEx(hDC, x, y, NULL);
		LineTo(hDC, x+400, y);
	}
}

BOOL CALLBACK MainDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			MainDlg = hWnd;
			ButtonQuery = GetDlgItem(hWnd, IDC_BUTTON_QUERY);
			EnableWindow(ButtonQuery, false);
		}; break;
		case WM_PAINT: 
		{
			InvalidateRect(hWnd, NULL, true);
			HDC hDC;
			PAINTSTRUCT ps;
			hDC = BeginPaint(hWnd, &ps);
			//Отрисовка сетки
			PaintField(hWnd, hDC, 20);
			EndPaint(hWnd, &ps);
		}; break;
		case WM_COMMAND:
		{	
			switch(LOWORD(wParam))
			{
				case IDC_BUTTON_INDEXING:
					{
						DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_INDEXING), hWnd, IndexingDlgProc);
					}; break;
				case IDC_BUTTON_QUERY:
					{
						DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_QUERY), hWnd, QueryDlgProc);
					}; break;
			}
		}; break;
	case WM_CLOSE:
		{
			PostQuitMessage(0);
		}; break;
	default:
		return FALSE;
	}
}
int costyl = 0;
BOOL CALLBACK IndexingDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			nTimerCommon = 1;
			nTimerSec = 2;
			TimerVal = 0;
			ProgressBarPos = 0;
			IsRecordComplited = true;
			IsIndexingComplited = false;
			ComboBox = GetDlgItem(hWnd, IDC_COMBOFIELDSIZE);
			ProgressBar = GetDlgItem(hWnd, IDC_PROGRESS1);
			SendMessage(ComboBox, CB_ADDSTRING, 0, LPARAM(L"10"));
			SendMessage(ComboBox, CB_ADDSTRING, 0, LPARAM(L"20"));
			SendMessage(ComboBox, CB_ADDSTRING, 0, LPARAM(L"30"));
			SendMessage(ComboBox, CB_ADDSTRING, 0, LPARAM(L"40"));
			SendMessage(ComboBox, CB_ADDSTRING, 0, LPARAM(L"50"));
			
		}; break;
	case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case IDOK:
				{
					srand(time(0));
					CurrentIndexingData.IndexingInterval = GetDlgItemInt(hWnd, IDC_EDITTIME, false, false);
					CurrentIndexingData.FieldSize = GetDlgItemInt(hWnd, IDC_COMBOFIELDSIZE, false, false);
					CurrentIndexingData.NumOfObjects = GetDlgItemInt(hWnd, IDC_EDITNUMBER, false, false);
					CurrentIndexingData.pnts1 = new POINT[CurrentIndexingData.NumOfObjects];
					CurrentIndexingData.pnts2 = new POINT[CurrentIndexingData.NumOfObjects];
					for (int i = 0; i < CurrentIndexingData.NumOfObjects; i++)
					{
						CurrentIndexingData.pnts1[i].x = rand()%400;
						CurrentIndexingData.pnts1[i].y = rand()%400;

						//SETI.Insert(CurrentIndexingData.pnts1[i].x, CurrentIndexingData.pnts1[i].y);
					}
					SendMessage(ProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, CurrentIndexingData.IndexingInterval));
					InvalidateRect(hWnd, NULL, true);
					HDC tDC = GetDC(MainDlg);
					PaintField(MainDlg, tDC, CurrentIndexingData.FieldSize);
					ReleaseDC(hWnd, tDC);
					SetTimer(hWnd, nTimerCommon, CurrentIndexingData.IndexingInterval*1000, NULL);
					SetTimer(hWnd, nTimerSec, 800, NULL);
				}; break;
			}
		}; break;
	case WM_TIMER:
		{
			if (wParam == nTimerCommon && IsIndexingComplited)
			{
				KillTimer(hWnd, nTimerCommon);
				KillTimer(hWnd, nTimerSec);
				MessageBox(hWnd, (LPCWSTR)L"Индексирование завершено!", (LPCWSTR)L"Готово", MB_OK | MB_ICONASTERISK);
				EndDialog(hWnd, NULL);
				EnableWindow(ButtonQuery, true);
			}
			else if (wParam == nTimerSec && IsRecordComplited)
			{
				for (int i = 0; i < CurrentIndexingData.NumOfObjects; i++)
				{
					IsRecordComplited = false;

					srand(i*time(0));
					if ((CurrentIndexingData.pnts1[i].x) % 2)
					{
						CurrentIndexingData.pnts2[i].x = CurrentIndexingData.pnts1[i].x + rand()%20;
						if (CurrentIndexingData.pnts2[i].x > 400)
							CurrentIndexingData.pnts2[i].x -= 5;
					}
					else
						CurrentIndexingData.pnts2[i].x = CurrentIndexingData.pnts1[i].x - rand()%20;

					if ((CurrentIndexingData.pnts1[i].y) % 2)
					{
						CurrentIndexingData.pnts2[i].y = CurrentIndexingData.pnts1[i].y - rand()%20;
						if (CurrentIndexingData.pnts2[i].y > 400)
							CurrentIndexingData.pnts2[i].y -= 5;
					}
					else
						CurrentIndexingData.pnts2[i].y = CurrentIndexingData.pnts1[i].y + rand()%20;

					HDC hDC = GetDC(MainDlg);
					HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255,0,0));
					SelectPen(hDC, hPen);
					MoveToEx(hDC, CurrentIndexingData.pnts1[i].x, CurrentIndexingData.pnts1[i].y, NULL);
					LineTo(hDC, CurrentIndexingData.pnts2[i].x, CurrentIndexingData.pnts2[i].y);
					DeletePen(hPen);
					ReleaseDC(MainDlg, hDC);

					Traject *NewTraject = new Traject(CurrentIndexingData.pnts1[i].x, CurrentIndexingData.pnts1[i].y,
					CurrentIndexingData.pnts2[i].x, CurrentIndexingData.pnts2[i].y, 1, i, TimerVal, TimerVal+1);
					if (costyl == 1)
						int a=3;
					STR_tree.InsertTrajectory(NewTraject);
					costyl++;
					/*TB_tree.InsertTrajectory(Traject* NewTraject);
					SETI.Insert(CurrentIndexingData.pnts2[i].x, CurrentIndexingData.pnts2[i].y);*/
					CurrentIndexingData.pnts1[i].x = CurrentIndexingData.pnts2[i].x;
					CurrentIndexingData.pnts1[i].y = CurrentIndexingData.pnts2[i].y;

					IsRecordComplited = true;
				}
				IsIndexingComplited = true;
				ProgressBarPos++;
				SendMessage(ProgressBar, PBM_SETPOS, ProgressBarPos, NULL);
				TimerVal++;
			}
		}; break;
	case WM_CLOSE:
		{
			EndDialog(hWnd, NULL);
		}; break;
	default:
		return FALSE;
	}
}



BOOL CALLBACK QueryDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			CurrentQueryData.IDs = new int[CurrentIndexingData.NumOfObjects];
			for (int i = 0; i < CurrentIndexingData.NumOfObjects; i++)
				CurrentQueryData.IDs[i] = i;
		}; break;
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDOK:
					{
						CurrentQueryData.NumOfQueryObjects = 0;
						HWND IDsControl = GetDlgItem(hWnd, IDC_EDITIDS);
						TCHAR ObjectsIDs[10240];
						GetWindowText(IDsControl, ObjectsIDs, 10240);
						TCHAR tmpstr[10] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
						int IDsint[1000];
						int j = 0;
						int k = 0;

						for (int i = 0; i < 10240; i++)
						{
							if (ObjectsIDs[i] == 44 || ObjectsIDs[i] == 0)
							{
								int tmp = StrToInt(tmpstr);
								CurrentQueryData.NumOfQueryObjects++;
								IDsint[k] = tmp;
								j = 0;
								k++;
								for (int i = 0; i < 10; i++)
									tmpstr[i] = ' ';
								continue;
							}
							if ((ObjectsIDs[i] < 48 || ObjectsIDs[i] > 57) && ObjectsIDs[i] != 44)
								break;
							tmpstr[j] = ObjectsIDs[i];
							j++;
						}

						CurrentQueryData.IDs = new int[CurrentQueryData.NumOfQueryObjects];
						for (int i = 0; i < CurrentQueryData.NumOfQueryObjects; i++)
							CurrentQueryData.IDs[i] = IDsint[i];
						
						CurrentQueryData.TimeFrom = GetDlgItemInt(hWnd, IDC_EDIT2, false, false);
						CurrentQueryData.TimeTo = GetDlgItemInt(hWnd, IDC_EDIT3, false, false);
						CurrentQueryData.MBRx0 = GetDlgItemInt(hWnd, IDC_EDIT4, false, false);
						CurrentQueryData.MBRy0 = GetDlgItemInt(hWnd, IDC_EDIT5, false, false);
						CurrentQueryData.MBRx1 = GetDlgItemInt(hWnd, IDC_EDIT6, false, false);
						CurrentQueryData.MBRy1 = GetDlgItemInt(hWnd, IDC_EDIT8, false, false);

						int col = 0;
						/*TrajectObj * Arr = STR_tree.FindTrajectory(&MBR(CurrentQueryData.MBRx0, CurrentQueryData.MBRy0,
							CurrentQueryData.MBRx1, CurrentQueryData.MBRy1, CurrentQueryData.TimeFrom, CurrentQueryData.TimeTo),
							5, col);*/
						//читай сигнатуры методов, кривая последовательность параметров при передаче в конструктор MBR
						for (int i = 0; i < CurrentQueryData.NumOfQueryObjects; i++)
						{
							TrajectObj * Arr = STR_tree.FindTrajectory(&MBR(CurrentQueryData.MBRy1, CurrentQueryData.MBRy0,
								CurrentQueryData.MBRx0, CurrentQueryData.MBRx1, CurrentQueryData.TimeFrom, CurrentQueryData.TimeTo),
								CurrentQueryData.IDs[i], col);
							HDC hDC = GetDC(MainDlg);
							MoveToEx(hDC, Arr[0].X, Arr[0].Y, NULL);
							HPEN hPen = CreatePen(PS_SOLID, 3, RGB(0,255,0));
							SelectPen(hDC, hPen);
							for (int i = 1; i < col; i++)
							{
								LineTo(hDC, Arr[i].X, Arr[i].Y);
								MoveToEx(hDC, Arr[i].X, Arr[i].Y, NULL);
							}
							ReleaseDC(MainDlg, hDC);
						}
					}; break;
			}
		}; break;
		case WM_CLOSE:
			{
				EndDialog(hWnd, 0);
			}; break;
		default:
			return FALSE;
	}
}