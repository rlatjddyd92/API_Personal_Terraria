// DefaultWindow.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "DefaultWindow.h"
#include "MainGame.h"

/*
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup/subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup/subsystem:console")
#endif
*/

#define MAX_LOADSTRING 100

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND	g_hWnd;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);	// 생성할 창의 각종 스타일 지정 함수
BOOL                InitInstance(HINSTANCE, int);			// 창을 생성하고 출력하는 기능의 함수
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);	// 윈도우 메세지 처리 함수
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//핸들 : 운영체제가 부여하는 고유 식별 번호(16진수 형태의 정수)
//
//리소스
//
//1. 프로세스		: 현재 구동하고 있는 프로그램(램->레지스터)
//2. 쓰레드			: 구동하는 프로그램의 코드적 흐름
//3. 파일(파이프)	: 파일

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,				// 메모리에 할당되는 실체, 즉 객체
                     _In_opt_ HINSTANCE hPrevInstance,		// 전에 실행되었던 인스턴스의 핸들이 넘어옴, 없을 경우 자동 NULL로 채워짐
                     _In_ LPWSTR    lpCmdLine,				// 유니코드 기반 문자 타입
                     _In_ int       nCmdShow)				// 콘솔 창이 아닌 윈도우 창 실행을 의미(실행할 창의 스타일 옵션 지정)
{
	//_crtBreakAlloc = 1163;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DEFAULTWINDOW, szWindowClass, MAX_LOADSTRING);
   
	MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEFAULTWINDOW));

    MSG msg;
	msg.message = WM_NULL;

    // 기본 메시지 루프입니다.
  //  while (GetMessage(&msg, nullptr, 0, 0))
  //  {
		//// 메뉴 기능의 단축키가 제대로 작동하도록 검사하는 함수
  //      if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
  //      {
		//	// TranslateMessage : 키보드 메시지를 가공하여 프로그램에서 쉽게 사용할 수 있도록 하는 함수
  //          TranslateMessage(&msg);

		//	// DispatchMessage : 시스템 큐에서 꺼낸 메시지를 프로그램의 메시지 처리 함수(WndProc)에게 전달
  //          DispatchMessage(&msg);
  //      }
  //  }

	// GetTickCount : 운영 체제가 동작된 시점으로부터 흐른 시간을 정수 형태로 반환
	// 1 / 1000초의 정수 값을 반환하는 함수

	srand(unsigned int(time(NULL)));

	DWORD	dwTime = GetTickCount();	// 500

	CMainGame		MainGame;
	MainGame.Initialize();

	while (true)
	{
		// PM_REMOVE   : 메세지를 읽어옴과 동시에 메세지 큐에서 메세지를 제거
		// PM_NOREMOVE : 메세지 큐에 메세지가 존재하는지만 파악, 메세지를 읽어오려면 GetMessage함수를 다시 호출해야 함

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (dwTime + 10 < GetTickCount())	
			{
				MainGame.Update();
				MainGame.Late_Update();
				MainGame.Render();

				dwTime = GetTickCount();
			}
		}
		
	}

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);	// 창 사이즈 

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
						// 가로를 다시 그리기 | 세로를 다시 그리기
						// 윈도우 창의 수직, 수평의 길이가 변화할 경우 윈도우 창을 다시 그린다는 옵션

    wcex.lpfnWndProc    = WndProc;	// 저장해둔 함수가 호출되어 메세지를 처리


    wcex.cbClsExtra     = 0;	//윈도우가 특수한 목적으로 사용하는 여분의 공간
    wcex.cbWndExtra     = 0;
	
    wcex.hInstance      = hInstance;	// 프로그램의 고유 식별 번호

    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEFAULTWINDOW));
						// 윈도우 창이 사용할 아이콘 지정

    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
						// 창에 출력된 커서 모양 지정
	
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
						// 배경색

	wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_DEFAULTWINDOW);
						// 창 상단에 있는 메뉴를 설정
    wcex.lpszClassName  = szWindowClass;
							// 실행파일 이름 저장, 가급적 프로젝트 이름과 일치시켜 작성
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
							// 창 상단에 있는 작은 아이콘 모양

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   // CreateWindowW : 등록해 놓은 정보들을 토대로 윈도우(창)를 만드는 함수
   // szWindowClass : 앞서 정의한 윈도우 클래스의 전달한 클래스 이름을 그대로 넣어줌
   // szTitle : 윈도우 상단에 이름을 나타낼 문자열
   // WS_OVERLAPPEDWINDOW : 만들고자 하는 찬 스타일(기본 값으로 사용하는 흔한 창 상태)
   // CW_USEDEFAULT, 0 : 창의 시작점 X, Y 좌표
   // CW_USEDEFAULT, 0 : 창의 가로, 세로 사이즈
   // nullptr : 부모 윈도우가 있다면 부모 윈도우 핸들을 지정, 없으면 NULL
   // nullptr : 윈도우에서 사용할 메뉴의 핸들을 지정, 레지스터에 등록한 메뉴 형태를 사용하려면 NULL
   // hInstance : 윈도우를 만드는 주체, 프로그램의 핸들을 지정
   // nullptr : 운영체제가 특수한 목적으로 사용하므로 사용할 일 없음

   RECT	rc{ 0, 0, WINCX, WINCY };

   // 최종적인 RC = rc 크기 + 기본 윈도우 창 스타일 + 메뉴 바 사이즈 크기 고려 여부
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
   
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 
	  rc.right - rc.left,
	  rc.bottom - rc.top,
	   nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//

// window의 기본 라이브러리

//1. kernel : 메모리를 관리하고 프로그램을 실행
//2. user : 유저 인터페이스와 윈도우 창 관리
//3. gdi : 화면 처리와 그래픽을 담당

//RECT		rc{ 100, 100, 200, 200 };
//list<RECT>	BulletList;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	// HDC : 출력에 관한 정보를 갖고 있는 데이터 구조체
	// GDI가 관리하며 어떤 포인트(점), 선(색상, 굵기, 무늬)에 대한 정보를 DC가 관리

 //   case WM_PAINT:
 //       {
 //           PAINTSTRUCT ps;
 //          
	//		HDC hdc = BeginPaint(hWnd, &ps);		// GetDC : 화면 dc를 얻어오는 함수
	//				
	//		Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
	//		
	//		for (auto& iter : BulletList)
	//		{
	//			Ellipse(hdc, iter.left, iter.top, iter.right, iter.bottom);

	//			iter.top	-= 10;
	//			iter.bottom -= 10;
	//		}
	//		
 //           EndPaint(hWnd, &ps);
 //       }
 //       break;

	//case WM_CREATE:

	//	SetTimer(hWnd, 0, 0, 0);
	//	// 2 : 타이머 id번호
	//	// 3 : 타이머 주기 0인 경우 지정되어있는 기본 값 동작(1 /1000초)
	//	// 4 : null인 경우 3번에 설정한 주기대로 WM_TIMER 메세지를 발생
	//	break;

	//case WM_TIMER:

	//	InvalidateRect(hWnd, 0, TRUE);
	//	// 윈도우 화면 갱신 함수 : WM_PAINT 메세지를 발생시킴

	//	// 2 : 윈도우에서 갱신할 범위를 지정, RECT 구조체 주소값을 받음, NULL을 넣으면 전체 화면 영역을 의미
	//	// 3 : TRUE : 그림이 그려져 있지 않는 부분도 갱신
	//	//	   FALSE : 그림이 그려진 부분만 갱신
	//	break;

	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;

	/*	case VK_RIGHT:
			rc.left  += 10;
			rc.right += 10;
			break;

		case VK_LEFT :
			rc.left -= 10;
			rc.right -= 10;
			break;

		case VK_UP:
			rc.top	  -= 10;
			rc.bottom -= 10;
			break;

		case VK_DOWN:
			rc.top += 10;
			rc.bottom += 10;
			break;

		case VK_SPACE:
			BulletList.push_back(rc);
			break;*/
		}
		break;

    case WM_DESTROY:

		//KillTimer(hWnd, 0);
	    PostQuitMessage(0);		 
	break;
	
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

