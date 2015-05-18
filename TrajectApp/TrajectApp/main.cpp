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
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include <CommCtrl.h>
#include "TempIndexingStructure.h"
#include <Shlwapi.h>
#include <time.h>
#include "STR-TREE.h"
#include "TB-tree.h"
#include "SETI.h"

HINSTANCE hInst;
static int dTimerCommon, nTimerCommon, dTimerSec, nTimerSec, TimerVal;
int ProgressBarPos;
int NumOfQueries = 0;
bool IsRecordComplited, IsIndexingComplited;
HWND ProgressBar;
HWND MainDlg;
HWND ComboBox;
HWND ButtonQuery;
HWND ListBox;
HWND ButtonStartIndexing;
HWND EditIDs;
HWND ComboNumQ;
HDC MainDC;
TmpIndexingData CurrentIndexingData;
TmpQueryData CurrentQueryData;
WCHAR bufNumOfQueries[10];
QueryData QueryHistory[100];

StrTree STR_tree(4, 10);
TBtree TB_tree(3, 10);
SETI* seti;

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
void PaintKoordLines(HWND hWnd, HDC hDC);
 

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

void PaintKoordLines(HWND hWnd, HDC hDC)
{
	HPEN KoordLinePen = CreatePen(PS_SOLID, 3, RGB(0,0,0));
	SelectPen(hDC, KoordLinePen);
	MoveToEx(hDC, 11, 13, NULL);
	LineTo(hDC, 11, 436);
	LineTo(hDC, 430, 436);
	LineTo(hDC, 420, 430);
	MoveToEx(hDC, 430, 436, NULL);
	LineTo(hDC, 420, 442);
	MoveToEx(hDC, 11, 13, NULL);
	LineTo(hDC, 5, 28);
	MoveToEx(hDC, 11, 13, NULL);
	LineTo(hDC, 17, 28);
	//Y и X
	MoveToEx(hDC, 37, 5, NULL);
	LineTo(hDC, 25, 25);
	MoveToEx(hDC, 31, 14, NULL);
	LineTo(hDC, 25, 5);

	MoveToEx(hDC, 400, 450, NULL);
	LineTo(hDC, 412, 470);
	MoveToEx(hDC, 400, 470, NULL);
	LineTo(hDC, 412, 450);
}
int nItem = 0;
BOOL CALLBACK MainDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			ListBox = GetDlgItem(hWnd, IDC_LIST1);
			MainDlg = hWnd;
			ButtonQuery = GetDlgItem(hWnd, IDC_BUTTON_QUERY);
			EnableWindow(ButtonQuery, false);
		}; break;
		case WM_MEASUREITEM:
		{
			int p = 0;
		}; break;
		case WM_PAINT: 
		{
			InvalidateRect(hWnd, NULL, true);
			HDC hDC;
			PAINTSTRUCT ps;
			hDC = BeginPaint(hWnd, &ps);
			//Отрисовка сетки
			PaintField(hWnd, hDC, 20);
			PaintKoordLines(hWnd, hDC);
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
				case IDC_LIST1:
					{
						if (HIWORD(wParam) == LBN_SELCHANGE)
						{
							int p = SendMessage(ListBox, LB_GETCURSEL, 0, 0);
							HDC hDC = GetDC(hWnd);
							HPEN hPen = CreatePen(PS_SOLID | PS_INSIDEFRAME, 30, CurrentQueryData.Colors[p]);
							SelectPen(hDC, hPen);
							Rectangle(hDC, 545, 32, 595, 82);
							ReleaseDC(hWnd, hDC);
						}
						
					}; break;
					case IDC_LIST2:
					{
						if (HIWORD(wParam) == LBN_SELCHANGE)
						{
							SendMessage(ListBox, LB_RESETCONTENT, 0, 0);
							nItem = 0;
							WCHAR tmpID[500];
							int col = 0;
							int qn = SendMessage(ComboNumQ, LB_GETCURSEL, 0, 0);
							int kp = 0;
							for (int i = 0; i < QueryHistory[qn].NumOfQueryObjects; i++)
							{
								HDC hDC = GetDC(MainDlg);
								MoveToEx(hDC, QueryHistory[qn].Arr[i][0]->X, QueryHistory[qn].Arr[i][0]->Y, NULL);
								int red = CurrentQueryData.currRed = rand()%210+30;
								int green = CurrentQueryData.currGreen = rand()%210+30;
								int blue = CurrentQueryData.currBlue = rand()%210+30;
								HPEN hPen = CreatePen(PS_SOLID, 3, RGB(red,green,blue));
								SelectPen(hDC, hPen);
								for (int j = 1; j < QueryHistory[qn].Arr[i][0]->Number; j++)
									LineTo(hDC, QueryHistory[qn].Arr[i][j]->X, QueryHistory[qn].Arr[i][j]->Y);
								
								CurrentQueryData.currTextColor = RGB(red, green, blue);
								CurrentQueryData.Colors[i] = CurrentQueryData.currTextColor;
								wsprintf(tmpID, L"");
								wsprintf(tmpID, L"%d", QueryHistory[qn].IDs[i]);
								nItem = SendMessage(ListBox, LB_ADDSTRING, 0, (LPARAM)tmpID);
								SendMessage(ListBox, LB_SETITEMDATA, nItem, (LPARAM)CurrentQueryData.currTextColor);
								ReleaseDC(MainDlg, hDC);
								kp++;
							}																														
							int p = SendMessage(ListBox, LB_GETCURSEL, 0, 0);
							HDC hDC = GetDC(hWnd);
							HPEN hPen = CreatePen(PS_SOLID | PS_INSIDEFRAME, 30, CurrentQueryData.Colors[p]);
							SelectPen(hDC, hPen);
							Rectangle(hDC, 545, 32, 595, 82);
							ReleaseDC(hWnd, hDC);
						}
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

BOOL CALLBACK IndexingDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			ButtonStartIndexing = GetDlgItem(hWnd, IDOK);
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
					CurrentIndexingData.NumOfObjects = GetDlgItemInt(hWnd, IDC_EDITNUMBER, false, false);
					if (CurrentIndexingData.NumOfObjects == 0)
					{
						MessageBox(hWnd, L"Укажите количество объектов!", L"Ошибка",
							MB_ICONINFORMATION);
						break;
					}
					CurrentIndexingData.IndexingInterval = GetDlgItemInt(hWnd, IDC_EDITTIME, false, false);
					if (CurrentIndexingData.IndexingInterval == 0)
					{
						MessageBox(hWnd, L"Укажите время индексирования!", L"Ошибка",
							MB_ICONINFORMATION);
						break;
					}
					CurrentIndexingData.FieldSize = GetDlgItemInt(hWnd, IDC_COMBOFIELDSIZE, false, false);
					if (CurrentIndexingData.FieldSize == 0)
					{
						MessageBox(hWnd, L"Укажите размер поля!", L"Ошибка",
							MB_ICONINFORMATION);
						break;
					}
					seti = new SETI(CurrentIndexingData.NumOfObjects, 480, 480, 40);
					CurrentIndexingData.pnts1 = new POINT[CurrentIndexingData.NumOfObjects];
					CurrentIndexingData.pnts2 = new POINT[CurrentIndexingData.NumOfObjects];
					for (int i = 0; i < CurrentIndexingData.NumOfObjects; i++)
					{
						CurrentIndexingData.pnts1[i].x = rand()%400+11;
						CurrentIndexingData.pnts1[i].y = rand()%402+32;

						seti->InsertDot(CurrentIndexingData.pnts1[i].x, CurrentIndexingData.pnts1[i].y, 0, i);
					}
					SendMessage(ProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, CurrentIndexingData.IndexingInterval));
					InvalidateRect(hWnd, NULL, true);
					HDC tDC = GetDC(MainDlg);
					PaintField(MainDlg, tDC, CurrentIndexingData.FieldSize);
					ReleaseDC(hWnd, tDC);
					SetTimer(hWnd, nTimerCommon, CurrentIndexingData.IndexingInterval*1000, NULL);
					SetTimer(hWnd, nTimerSec, 800, NULL);

					EnableWindow(ButtonStartIndexing, FALSE);
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
				EnableWindow(ButtonQuery, TRUE);
				EnableWindow(ButtonStartIndexing, FALSE);
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
						if (CurrentIndexingData.pnts2[i].x > 390)
							CurrentIndexingData.pnts2[i].x -= 20;
						if (CurrentIndexingData.pnts2[i].x < 12)
							CurrentIndexingData.pnts2[i].x += 20;
					}
					else
					{
						CurrentIndexingData.pnts2[i].x = CurrentIndexingData.pnts1[i].x - rand()%20;
						if (CurrentIndexingData.pnts2[i].x > 410)
							CurrentIndexingData.pnts2[i].x -= 20;
						if (CurrentIndexingData.pnts2[i].x < 12)
							CurrentIndexingData.pnts2[i].x += 20;
					}

					if ((CurrentIndexingData.pnts1[i].y) % 2)
					{
						CurrentIndexingData.pnts2[i].y = CurrentIndexingData.pnts1[i].y - rand()%20;
						if (CurrentIndexingData.pnts2[i].y > 415)
							CurrentIndexingData.pnts2[i].y -= 20;
						if (CurrentIndexingData.pnts2[i].y < 33)
							CurrentIndexingData.pnts2[i].y += 20;
					}
					else
					{
						CurrentIndexingData.pnts2[i].y = CurrentIndexingData.pnts1[i].y + rand()%20;
						if (CurrentIndexingData.pnts2[i].y > 435)
							CurrentIndexingData.pnts2[i].y -= 20;
						if (CurrentIndexingData.pnts2[i].y < 33)
							CurrentIndexingData.pnts2[i].y += 20;
					}

					HDC hDC = GetDC(MainDlg);
					HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255,0,0));
					SelectPen(hDC, hPen);
					MoveToEx(hDC, CurrentIndexingData.pnts1[i].x, CurrentIndexingData.pnts1[i].y, NULL);
					LineTo(hDC, CurrentIndexingData.pnts2[i].x, CurrentIndexingData.pnts2[i].y);
					DeletePen(hPen);
					ReleaseDC(MainDlg, hDC);

					Traject *NewTraject = new Traject(CurrentIndexingData.pnts1[i].x, CurrentIndexingData.pnts1[i].y,
					CurrentIndexingData.pnts2[i].x, CurrentIndexingData.pnts2[i].y, 1, i, TimerVal, TimerVal+1);
					STR_tree.InsertTrajectory(NewTraject);
					TB_tree.InsertTrajectory(NewTraject);
					seti->InsertDot(CurrentIndexingData.pnts2[i].x, CurrentIndexingData.pnts2[i].y, TimerVal+1, i);
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
			delete CurrentIndexingData.pnts1;
			delete CurrentIndexingData.pnts2;
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
			ComboNumQ = GetDlgItem(MainDlg, IDC_LIST2);
			EditIDs = GetDlgItem(hWnd, IDC_EDITIDS);
			CurrentQueryData.IDs = new int[CurrentIndexingData.NumOfObjects];
			for (int i = 0; i < CurrentIndexingData.NumOfObjects; i++)
				CurrentQueryData.IDs[i] = i;
		}; break;
		case WM_COMMAND:
		{
			if (HIWORD(wParam) == EN_CHANGE)
			{
				if (lParam == (LPARAM)EditIDs)
				{
					WCHAR textbuf[1000];
					GetDlgItemText(hWnd, IDC_EDITIDS, textbuf, 1000);
					int len = lstrlen(textbuf);
					for (int i = 0; i < len; i++)
					{
						if ((textbuf[i] != ',') && ((textbuf[i] < '0') || (textbuf[i] > '9')))
						{
							MessageBox(hWnd, L"Перечислите индексы объектов\nпо порядку через запятую", L"Некорректный ввод",
								MB_ICONINFORMATION);
							textbuf[i] = '\0';
							SetDlgItemText(hWnd, IDC_EDITIDS, textbuf);
							Edit_SetSel(EditIDs, i, i);
							break;
						}
					}
				}
			}
			switch (LOWORD(wParam))
			{
				case IDOK:
					{
						SendMessage(ListBox, LB_RESETCONTENT, 0, 0);
						CurrentQueryData.NumOfQueryObjects = 0;
						HWND IDsControl = GetDlgItem(hWnd, IDC_EDITIDS);
						TCHAR ObjectsIDs[10240];
						GetWindowText(IDsControl, ObjectsIDs, 10240);
						TCHAR tmpstr[10] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
						int IDsint[1000];
						int j = 0;
						int k = 0;

						for (int i = 0; i < 1000; i++)
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
						QueryHistory[NumOfQueries].IDs = new int[CurrentQueryData.NumOfQueryObjects];
						for (int i = 0; i < CurrentQueryData.NumOfQueryObjects; i++)
							CurrentQueryData.IDs[i] = QueryHistory[NumOfQueries].IDs[i] = IDsint[i];

						CurrentQueryData.Colors = new COLORREF[CurrentQueryData.NumOfQueryObjects];
						
						CurrentQueryData.TimeFrom = GetDlgItemInt(hWnd, IDC_EDIT2, false, false);
						CurrentQueryData.TimeTo = GetDlgItemInt(hWnd, IDC_EDIT3, false, false);
						CurrentQueryData.MBRx0 = 11 + GetDlgItemInt(hWnd, IDC_EDIT4, false, false);
						CurrentQueryData.MBRy0 = 432 - GetDlgItemInt(hWnd, IDC_EDIT8, false, false);
						CurrentQueryData.MBRx1 = 11 + GetDlgItemInt(hWnd, IDC_EDIT6, false, false);
						CurrentQueryData.MBRy1 = 436 - GetDlgItemInt(hWnd, IDC_EDIT5, false, false);

						WCHAR tmpID[5];

						int col = 0;
						//читай сигнатуры методов, кривая последовательность параметров при передаче в конструктор MBR
						if (IsDlgButtonChecked(hWnd, IDC_RADIOSTR) == BST_CHECKED)
						{
							for (int k = 0; k < 100; k++)
								for (int h = 0; h < 100; h++)
										QueryHistory[NumOfQueries].Arr[k][h] = new TrajectObj;
							TrajectObj* ArrSTR;
							for (int i = 0; i < CurrentQueryData.NumOfQueryObjects; i++)
							{
								ArrSTR = STR_tree.FindTrajectory(&MBR(CurrentQueryData.MBRy1, CurrentQueryData.MBRy0,
									CurrentQueryData.MBRx0, CurrentQueryData.MBRx1, CurrentQueryData.TimeFrom, CurrentQueryData.TimeTo),
									CurrentQueryData.IDs[i], col);
								
								HDC hDC = GetDC(MainDlg);
								MoveToEx(hDC, ArrSTR[0].X, ArrSTR[0].Y, NULL);
								QueryHistory[NumOfQueries].Arr[i][0]->X = ArrSTR[0].X;
								QueryHistory[NumOfQueries].Arr[i][0]->Y = ArrSTR[0].Y;
								QueryHistory[NumOfQueries].Arr[i][0]->Number = col;
								
								int red = CurrentQueryData.currRed = rand()%210+30;
								int green = CurrentQueryData.currGreen = rand()%210+30;
								int blue = CurrentQueryData.currBlue = rand()%210+30;
								HPEN hPen = CreatePen(PS_SOLID, 3, RGB(red,green,blue));
								SelectPen(hDC, hPen);
								if (col == 1)
								{
									MessageBox(hWnd, L"В указанной области\nв указанное время\nэтих объектов нет", L"Результат",
										MB_ICONINFORMATION);
									return FALSE;
								}
								for (int j = 1; j < col; j++)
								{
									LineTo(hDC, ArrSTR[j].X, ArrSTR[j].Y);
									QueryHistory[NumOfQueries].Arr[i][j]->X = ArrSTR[j].X;
									QueryHistory[NumOfQueries].Arr[i][j]->Y = ArrSTR[j].Y;
								}
								
								CurrentQueryData.currTextColor = RGB(red, green, blue);
								CurrentQueryData.Colors[i] = CurrentQueryData.currTextColor;
								wsprintf(tmpID, L"%d", CurrentQueryData.IDs[i]);
								nItem = SendMessage(ListBox, LB_ADDSTRING, 0, (LPARAM)tmpID);
								SendMessage(ListBox, LB_SETITEMDATA, nItem, (LPARAM)CurrentQueryData.currTextColor);
								ReleaseDC(MainDlg, hDC);
								

							}
							QueryHistory[NumOfQueries].StructureType = IDC_RADIOSTR;
							QueryHistory[NumOfQueries].MBRx0 = CurrentQueryData.MBRx0;
							QueryHistory[NumOfQueries].MBRy0 = CurrentQueryData.MBRy0;
							QueryHistory[NumOfQueries].MBRx1 = CurrentQueryData.MBRx1;
							QueryHistory[NumOfQueries].MBRy1 = CurrentQueryData.MBRy1;
							QueryHistory[NumOfQueries].NumOfQueryObjects = CurrentQueryData.NumOfQueryObjects;
							QueryHistory[NumOfQueries].TimeFrom = CurrentQueryData.TimeFrom;
							QueryHistory[NumOfQueries].TimeTo = CurrentQueryData.TimeTo;
							wsprintf(bufNumOfQueries, L"%d", NumOfQueries);	
							SendMessage(ComboNumQ, LB_ADDSTRING, 0, (LPARAM)bufNumOfQueries);
							NumOfQueries++;
						}
						else if (IsDlgButtonChecked(hWnd, IDC_RADIOTB) == BST_CHECKED)
						{
							for (int k = 0; k < 100; k++)
									for (int h = 0; h < 100; h++)
										QueryHistory[NumOfQueries].Arr[k][h] = new TrajectObj;
							TrajectObj* ArrTB;
							for (int i = 0; i < CurrentQueryData.NumOfQueryObjects; i++)
							{
								ArrTB = TB_tree.FindTrajectory(&MBR(CurrentQueryData.MBRy1, CurrentQueryData.MBRy0,
									CurrentQueryData.MBRx0, CurrentQueryData.MBRx1, CurrentQueryData.TimeFrom, CurrentQueryData.TimeTo),
									CurrentQueryData.IDs[i], col);
								
								HDC hDC = GetDC(MainDlg);
								MoveToEx(hDC, ArrTB[0].X, ArrTB[0].Y, NULL);
								QueryHistory[NumOfQueries].Arr[i][0]->X = ArrTB[0].X;
								QueryHistory[NumOfQueries].Arr[i][0]->Y = ArrTB[0].Y;
								QueryHistory[NumOfQueries].Arr[i][0]->Number = col;
								
								int red = CurrentQueryData.currRed = rand()%210+30;
								int green = CurrentQueryData.currGreen = rand()%210+30;
								int blue = CurrentQueryData.currBlue = rand()%210+30;
								HPEN hPen = CreatePen(PS_SOLID, 3, RGB(red,green,blue));
								SelectPen(hDC, hPen);
								if (col == 1)
								{
									MessageBox(hWnd, L"В указанной области\nв указанное время\nэтих объектов нет", L"Результат",
										MB_ICONINFORMATION);
									return FALSE;
								}
								for (int j = 1; j < col; j++)
								{
									LineTo(hDC, ArrTB[j].X, ArrTB[j].Y);
									QueryHistory[NumOfQueries].Arr[i][j]->X = ArrTB[j].X;
									QueryHistory[NumOfQueries].Arr[i][j]->Y = ArrTB[j].Y;
								}
								
								CurrentQueryData.currTextColor = RGB(red, green, blue);
								CurrentQueryData.Colors[i] = CurrentQueryData.currTextColor;
								wsprintf(tmpID, L"%d", CurrentQueryData.IDs[i]);
								nItem = SendMessage(ListBox, LB_ADDSTRING, 0, (LPARAM)tmpID);
								SendMessage(ListBox, LB_SETITEMDATA, nItem, (LPARAM)CurrentQueryData.currTextColor);
								ReleaseDC(MainDlg, hDC);
								

							}
							QueryHistory[NumOfQueries].StructureType = IDC_RADIOSTR;
							QueryHistory[NumOfQueries].MBRx0 = CurrentQueryData.MBRx0;
							QueryHistory[NumOfQueries].MBRy0 = CurrentQueryData.MBRy0;
							QueryHistory[NumOfQueries].MBRx1 = CurrentQueryData.MBRx1;
							QueryHistory[NumOfQueries].MBRy1 = CurrentQueryData.MBRy1;
							QueryHistory[NumOfQueries].NumOfQueryObjects = CurrentQueryData.NumOfQueryObjects;
							QueryHistory[NumOfQueries].TimeFrom = CurrentQueryData.TimeFrom;
							QueryHistory[NumOfQueries].TimeTo = CurrentQueryData.TimeTo;
							wsprintf(bufNumOfQueries, L"%d", NumOfQueries);	
							SendMessage(ComboNumQ, LB_ADDSTRING, 0, (LPARAM)bufNumOfQueries);
							NumOfQueries++;
						}
						else if (IsDlgButtonChecked(hWnd, IDC_RADIOSETI) == BST_CHECKED)
						{
							for (int k = 0; k < 100; k++)
									for (int h = 0; h < 100; h++)
										QueryHistory[NumOfQueries].Arr[k][h] = new TrajectObj;
							TrajectObj* ArrSETI;
							for (int i = 0; i < CurrentQueryData.NumOfQueryObjects; i++)
							{
								ArrSETI = seti->FindTrajectory(&MBR(CurrentQueryData.MBRy1, CurrentQueryData.MBRy0,
									CurrentQueryData.MBRx0, CurrentQueryData.MBRx1, CurrentQueryData.TimeFrom, CurrentQueryData.TimeTo),
									CurrentQueryData.IDs[i], col);
								
								HDC hDC = GetDC(MainDlg);
								MoveToEx(hDC, ArrSETI[0].X, ArrSETI[0].Y, NULL);
								QueryHistory[NumOfQueries].Arr[i][0]->X = ArrSETI[0].X;
								QueryHistory[NumOfQueries].Arr[i][0]->Y = ArrSETI[0].Y;
								QueryHistory[NumOfQueries].Arr[i][0]->Number = col;
								
								int red = CurrentQueryData.currRed = rand()%210+30;
								int green = CurrentQueryData.currGreen = rand()%210+30;
								int blue = CurrentQueryData.currBlue = rand()%210+30;
								HPEN hPen = CreatePen(PS_SOLID, 3, RGB(red,green,blue));
								SelectPen(hDC, hPen);
								if (col == 1)
								{
									MessageBox(hWnd, L"В указанной области\nв указанное время\nэтих объектов нет", L"Результат",
										MB_ICONINFORMATION);
									return FALSE;
								}
								for (int j = 1; j < col; j++)
								{
									LineTo(hDC, ArrSETI[j].X, ArrSETI[j].Y);
									QueryHistory[NumOfQueries].Arr[i][j]->X = ArrSETI[j].X;
									QueryHistory[NumOfQueries].Arr[i][j]->Y = ArrSETI[j].Y;
								}
								
								CurrentQueryData.currTextColor = RGB(red, green, blue);
								CurrentQueryData.Colors[i] = CurrentQueryData.currTextColor;
								wsprintf(tmpID, L"%d", CurrentQueryData.IDs[i]);
								nItem = SendMessage(ListBox, LB_ADDSTRING, 0, (LPARAM)tmpID);
								SendMessage(ListBox, LB_SETITEMDATA, nItem, (LPARAM)CurrentQueryData.currTextColor);
								ReleaseDC(MainDlg, hDC);

							}
							QueryHistory[NumOfQueries].StructureType = IDC_RADIOSTR;
							QueryHistory[NumOfQueries].MBRx0 = CurrentQueryData.MBRx0;
							QueryHistory[NumOfQueries].MBRy0 = CurrentQueryData.MBRy0;
							QueryHistory[NumOfQueries].MBRx1 = CurrentQueryData.MBRx1;
							QueryHistory[NumOfQueries].MBRy1 = CurrentQueryData.MBRy1;
							QueryHistory[NumOfQueries].NumOfQueryObjects = CurrentQueryData.NumOfQueryObjects;
							QueryHistory[NumOfQueries].TimeFrom = CurrentQueryData.TimeFrom;
							QueryHistory[NumOfQueries].TimeTo = CurrentQueryData.TimeTo;
							wsprintf(bufNumOfQueries, L"%d", NumOfQueries);	
							SendMessage(ComboNumQ, LB_ADDSTRING, 0, (LPARAM)bufNumOfQueries);
							NumOfQueries++;
						}
						else //ничего не выбрано
						{
							MessageBox(hWnd, L"Выберите индесную структуру", L"Ошибка",
								MB_ICONINFORMATION);
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