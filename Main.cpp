/*
Copiright 1999-2002,Sasha Paholkov 
email: Sasha4685847@mail.ru
*/
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include "main.h"
#include <windowsx.h>
#include <math.h>
#include <mmsystem.h>

#include <strmif.h>
//#include "map.h"
#include "printf.h"
extern class printf1 Printf;
#include "spirit.h"
extern class spirit1 Spirit[SPIRIT_COUNT];
//#pragma comment(lib,"user32.lib")

//#pragma comment(linker,"/nodefaultlib")//disable libs

//#pragma comment(linker,"/defaultlib:user32.lib")
//#pragma comment(linker,"/defaultlib:gdi32.lib")
//#pragma comment(linker,"/defaultlib:winmm.lib")
//#pragma comment(linker,"/defaultlib:ole32.lib")
//#pragma comment(linker,"/defaultlib:user32.lib")
//#pragma comment(linker,"/defaultlib:shellapi.lib")
//#pragma comment(linker,"/defaultlib:user32.lib")





//#include "stdwin.h"
//#include <evcode.h>
//#include <windows.h>
//#include <commdlg.h>
//#include <string.h>

#include <control.h>



LONG      evCode;
  LONG      evParam1;
  LONG      evParam2;
 extern IMediaEventEx *pME;
extern IMediaControl *pMC;
extern 	IMediaPosition *pMP;
	char str[10];

HWND hWnd;
char szErrorMessage[255];	
//Обработчики сообщений Windows
BOOL DX_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void DX_OnDestroy(HWND hwnd);
void DX_OnTimer(HWND hwnd, UINT id);
void DX_OnActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized);
void DX_OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
void DX_OnIdle(HWND hwnd);

DWORD dwGlobPrevTime;
DWORD dwGlobCurTime;

//Оконная процедура
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Характеристики таймера
#define TIMER_ID	1
unsigned int TimerId;
//Флаг активности нашего приложения
BOOL	bActive;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASSEX wndClass;
	//HWND hWnd;
	MSG msg;

	//Регистрация оконного класса
	wndClass.cbSize       =sizeof(wndClass);
	wndClass.style        =CS_HREDRAW|CS_VREDRAW;
	wndClass.lpfnWndProc  =WndProc;
	wndClass.cbClsExtra   =0;
	wndClass.cbWndExtra   =0;
	wndClass.hInstance    =hInst;
	wndClass.hIcon        =LoadIcon(NULL,IDI_WINLOGO);
	wndClass.hCursor      =LoadCursor(NULL,IDC_ARROW);
	wndClass.hbrBackground=NULL;//Обратите внимание!!!
	wndClass.lpszMenuName =NULL;
	wndClass.lpszClassName=ClassName;
	wndClass.hIconSm      =LoadIcon(NULL,IDI_WINLOGO);
	RegisterClassEx(&wndClass);

	//Создание окна на основе класса
	hWnd=CreateWindowEx(
		0,//WS_EX_TOPMOST,//Дополнительный стиль окна
		ClassName,	//Класс окна
		AppName,	//Текст заголовка
		WS_POPUP|
		WS_MAXIMIZE,	//Стиль окна
		0,0,		//Координаты X и Y
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),//Ширина и высота
		NULL,		//Дескриптор родительского окна
		NULL,		//Дескриптор меню
		hInst,		//Описатель экземпляра
		NULL);		//Дополнительные данные
	ShowCursor(FALSE);
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	SYSTEMTIME sm;
	GetLocalTime(&sm);
//	if(sm.wDay == 6)
//	return FALSE;
/* if(!sm.wMonth%2)
  day = 31;
 else
  day = 30;
 if(sm.wMonth == 2)
  day = 28;
 for(int i=0; i<10; i++){
  if(sm.wYear-4*i == 2000){
   day = 29;
   break;
  }
 }
 if(sm.wMonth == 1){
  month = 12;
  sm.wYear--;
 }
 else{
  sm.wMonth--;
  if(sm.wMonth < 10)
   wsprintf(month, "0%d", sm.wMonth);
 }
*/
	LoadSettings();
Printf.Init();
	if (!InitDirectDraw(hWnd))
		return FALSE;
	if (!InitDirectInput(hWnd))
	{
		wsprintf(szErrorMessage, "Direct input error");
		ShowCursor(TRUE);
		MessageBox(hWnd, szErrorMessage, AppName, MB_OK);
		ShowCursor(FALSE);
		return FALSE;
	}

	if(DS)
	if (!InitDirectSound(hWnd))
	{		
		wsprintf(szErrorMessage, "Sound Disabled");
		ShowCursor(TRUE);
		MessageBox(hWnd, szErrorMessage, AppName, MB_OK);
		ShowCursor(FALSE);
		DS=0;
		SOUND=0;
	}
//	else
	//	SOUND=1;
#ifndef NOMUSIC
	if(DM)
		if (!InitDirectMusic(hWnd))
		{
			wsprintf(szErrorMessage, "Music Disabled");
			ShowCursor(TRUE);
			MessageBox(hWnd, szErrorMessage, AppName, MB_OK);
			ShowCursor(FALSE);
			DM=0;
			MUSIC=0;
			}
	//	else
	//	 MUSIC=0;
#endif
	  if(DSH)
		  InitMP3()	;
	//CoInitialize(NULL);

	//SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);


if (!PrepareGame())
return FALSE;
//	if (!InitGame())
//		return FALSE;
	//LoadSettings();
	//Главный цикл программы

//	MSG msg;
    //HANDLE  ahObjects[1];       // Handles that need to be waited on
    //const int cObjects = 1;     // Number of objects that we have

    // Message loop lasts until we get a WM_QUIT message
    // Upon which we shall return from the function

   
	while (TRUE)
	{
		

		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (msg.message==WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			//if (bActive)//Только если приложение активно!
			{
			;//
			DX_OnIdle(hWnd);
				//OnGraphNotify();
			}
	}
	
	return (msg.wParam);
}
//---------------------------------------------------------
//	Оконная процедура
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//char str[10];
	HDC hdc;
	switch(msg)
	{
		HANDLE_MSG(hWnd,WM_CREATE,DX_OnCreate);
		HANDLE_MSG(hWnd,WM_DESTROY,DX_OnDestroy);
		HANDLE_MSG(hWnd,WM_TIMER,DX_OnTimer);
		HANDLE_MSG(hWnd,WM_ACTIVATE,DX_OnActivate);
		HANDLE_MSG(hWnd,WM_KEYDOWN,DX_OnKey);
		//HANDLE_MSG(hWnd,WM_SETCURSOR,Set_Cursor);
		   case WM_SETCURSOR:
            SetCursor(NULL);
			return (0);
			break;

		
			case WM_CHAR: /* обработка нажатия клавиши */ 
			hdc= GetDC (hWnd) ; /*получить контекст устройства */ 
			//TextOut (hdc, 1, 1, " ", 2); /* стереть старый символ */ 
			wsprintf (str, "%c", (char)wParam); /* записать новый символ в выходной буфер */ 
			//TextOut (hdc, 1, 1, str, strlen (str) ) ; 
			ReleaseDC (hWnd, hdc) ; 
			return (0);
			break; 

		   case	WM_PALETTECHANGED:
			   	return (0);
			   break;
			case WM_GRAPHNOTIFY:

            while (SUCCEEDED(pME->GetEvent(&evCode, &evParam1, &evParam2, 0)))

              { // Spin through the events

                hr = pME->FreeEventParams(evCode, evParam1, evParam2);

                if ((EC_COMPLETE == evCode) )
				{
					pMP->put_CurrentPosition( 0); 		
				 hr = pMC->Run();
				}
                  //{ // Finished

                    //pivw->put_Visible(OAFALSE);

                   // HELPER_RELEASE(pivw);
                   // HELPER_RELEASE(pif);
                   // HELPER_RELEASE(pigb);
                   // HELPER_RELEASE(pimc);
                  //  HELPER_RELEASE(pimex);

                   // EnableMenuItem(ghMenu, ID_FILE_OPENCLIP, MF_ENABLED);
			}	return (0);
                    break;

	default:
		return DefWindowProc(hWnd,msg,wParam,lParam);
	}
}
		/* Обработчики сообщений */
//---------------------------------------------------------

BOOL DX_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
//Установка таймера
	if (!SetTimer(hwnd,TIMER_ID,TIMER_RATE,NULL))
		return (FALSE);
	return (TRUE);
}
//---------------------------------------------------------
void DX_OnDestroy(HWND hwnd)
{
	bActive=FALSE;
	KillTimer(hwnd,TIMER_ID);
	 
	StopMidi();
	RemoveMP3();
	RemoveDirectMusic();	
	RemoveDirectSound();
	RemoveDirectInput();
	 
	RemoveDirectDraw();
	ShowCursor(TRUE);
	PostQuitMessage(0);
}
//---------------------------------------------------------
void DX_OnTimer(HWND hwnd, UINT id)
{
	extern sec;
	if (bActive)
	//	NextTick();
	sec++;
}
//---------------------------------------------------------
void DX_OnIdle(HWND hwnd)
{
	extern Quit;//
	if(	Quit==1)
		DestroyWindow(hwnd);//
	else
	{
	dwGlobCurTime=timeGetTime();
	ProcessMouse();
	if((dwGlobCurTime-dwGlobPrevTime)>=30)
		{			
		dwGlobPrevTime=dwGlobCurTime;
		NextTick();
		
		DrawFrame();
		
		}
	//ProcessMouse();
	//DrawFrame();
	
	}
}
//---------------------------------------------------------
void DX_OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	int static temp=0;
extern Hands;	
extern nMudrost,game;
extern void Change_laguage();
extern void NewGame();
extern Map_Numer;
extern UserSelect;
//extern double AddAngle;
extern world;
if(mode==SELECT)
{
if (vk==VK_F1)
{

Map_Numer=0;
world=0;
UserSelect=0;
mode=BASE_MENU;
NewGame();
}

if (vk==VK_F2)
{

Map_Numer=1;
world=1;
UserSelect=20;
#ifndef DEBUG

mode=BASE_MENU;
#endif

NewGame();
}

if (vk==VK_F3)
{

Map_Numer=2;
world=2;
UserSelect=10;
mode=BASE_MENU;
NewGame();
}

if (vk==VK_F4)
{
Map_Numer=3;
world=1;
UserSelect=30;
mode=BASE_MENU;
NewGame();
}

return;
}
//--------------
if (vk==VK_F9)
if(world>0)world--;

if (vk==VK_F8)
if(world<2)world++;
//-------------------
if(mode==SPLASH){PlayMovie(7); mode=BASE_MENU;}

//if (vk==VK_LEFT)AddAngle-=0.01;
//if (vk==VK_RIGHT)AddAngle+=0.01;

	if (vk==VK_SPACE)
	{	
		SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);

		//world++;
		//mission=0; 
		DX_OnIdle(hWnd);
	//	Change_laguage();
	 //nK_asurov+=10;
	//nK_nebes-=10;
	//nMudrost+=10;
	 //nK_nebes,nK_asurov,nK_ludei,nK_pretov,nK_givotn,nK_ada;
		//Hands=0;
		NextTick();
		INVENTORY=0;
		INVENTORY2=0;
		QUEST=0;
		//MAGIA=0;
		OPIT=0;
		MAP=0;

	//	nLife--;
//VolumeMP3();
	}
	if (vk==VK_ESCAPE)		
		switch(mode)
		{
		//case SPLASH:
		//	mode=BASE_MENU;
		//	break;
		case BASE_MENU:
			DestroyWindow(hwnd);
			break;
		case DEATH:
			{MENU=MENU^1;
			game=game^1;}
			break;
		case 25:
			mode=SHOP;
			break;

		case TALK_D_OBJ:
		case SHOP:
		case BAR:
		case RESTAURANT:
		case ASK_PEOPLE:
		case TALK_D_OBJ0:
			
			mode=0;
		//	modenew=0;
		//	GoOut=1;
		//	LocalXq=XqNew=Xp-2;
		//	PersXNew=PersX-30*2;
		//	dialog=0;
			break;

		case BAY:
			mode=SHOP;
			break;
		case 20:
			mode=S_OBJ;//
			break;
		case S_OBJ:
			mode=0;
			break;
		case PEOPLE:
			mode=0;
			break;
		case D_OBJ:
			mode=0;
			break;
		case 0:
		default:
			//MENU++;
			//if(MENU>1)
			//	MENU=0;
			//MENU=MENU^1;
			MENU=MENU^1;
			game=game^1;
			if(OPTIONS==1)
				{
				OPTIONS=0;
				MENU=0;
				game=1;
				}
			break;
		
		}
	if (vk==VK_F1)
		nDEBUG=1;
	if (vk==VK_F2)
		nDEBUG=0;
	if (vk==VK_F3)
	{
		if(world==0)
		if(UserSelect<9)
		{
			Spirit[UserSelect].user=0;
			UserSelect++;
			Spirit[UserSelect].user=1;
		
		}
		if(world==1)
		if(UserSelect<39)
		{
			Spirit[UserSelect].user=0;
			UserSelect++;
			Spirit[UserSelect].user=1;
		
		}
		if(world==2)
		if(UserSelect<19)
		{
			Spirit[UserSelect].user=0;
			UserSelect++;
			Spirit[UserSelect].user=1;
		
		}
		//if(world==3)
		//	if(UserSelect<4)UserSelect++;
	}
	if (vk==VK_F4)
	{
		if(world==0)
			if(UserSelect>0)
			{
			Spirit[UserSelect].user=0;
			UserSelect--;
			Spirit[UserSelect].user=1;
		
			}
			if(world==1)
			if(UserSelect>20)
			{
			Spirit[UserSelect].user=0;
			UserSelect--;
			Spirit[UserSelect].user=1;
		
			}
			if(world==2)
			if(UserSelect>10)
			{
			Spirit[UserSelect].user=0;
			UserSelect--;
			Spirit[UserSelect].user=1;
		
			}
	}
	//PANEL_UPDATE=1;

	if (vk==VK_F5)
		Spirit[UserSelect].user=Spirit[UserSelect].user^1;
	if (vk==VK_F6)
		modeinfo++;
	if (vk==VK_F7)
		modeinfo--;
		
	//if (vk==VK_F8)
	//		nUstalost-=5;
//if (vk==VK_F9)
//		if(nLife<65)nLife+=5;

	//if (vk==VK_F10)
	//	  VolumeMP3();


	if (vk==VK_ADD)
		SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);
  
	
//	if (vk==VK_SUBTRACT)
//			;
}
//---------------------------------------------------------
void DX_OnActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized)
{
	//Обновить флаг состояния активности
	if (state==WA_INACTIVE)
		bActive=FALSE;
	else
		bActive=TRUE;
}
