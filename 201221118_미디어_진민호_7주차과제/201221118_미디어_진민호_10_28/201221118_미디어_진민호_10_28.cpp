// 201221118_�̵��_����ȣ_10_28.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "201221118_�̵��_����ȣ_10_28.h"
#include "egl/egl.h"
#include "vg/openvg.h"
#include "vgLoadImage.h"

#define MAX_LOADSTRING 100

int leftup = 0;
int leftdown = 1;
int rightup = 2;
int rightdown = 3;

EGLDisplay display;
EGLSurface surface;
EGLContext context;

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.
VGImage ballImage;


typedef struct stateball{
	int x, y;
	int dir;
};

stateball ball;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void timerProc(void);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY201221118___10_28, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY201221118___10_28));

	// �⺻ �޽��� �����Դϴ�.
		DWORD lastTime = GetTickCount();
		while (1)
			{
				if( PeekMessage(&msg, NULL, 0, 0, 1) )
				{
					if(!TranslateAccelerator(msg.hwnd, NULL, &msg))
					{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
					}
					if( msg.message == WM_QUIT ) break;
				}
				DWORD curTime = GetTickCount();
				if( curTime-lastTime>32 ) // 30 frame per second
				{
				lastTime = lastTime+33;
				timerProc();
			}
		}

	return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
//  ����:
//
//    Windows 95���� �߰��� 'RegisterClassEx' �Լ����� ����
//    �ش� �ڵ尡 Win32 �ý��۰� ȣȯ�ǵ���
//    �Ϸ��� ��쿡�� �� �Լ��� ����մϴ�. �� �Լ��� ȣ���ؾ�
//    �ش� ���� ���α׷��� �����
//    '�ùٸ� ������' ���� �������� ������ �� �ֽ��ϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY201221118___10_28));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY201221118___10_28);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 400, 400, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
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
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		ball.x = 257;
		ball.y = 200;
		ball.dir = rightdown;
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
		EndPaint(hWnd, &ps);
		break;
	case WM_CREATE:
		{
			display = eglGetDisplay( GetDC(hWnd));
			eglInitialize( display, NULL, NULL );
			eglBindAPI( EGL_OPENVG_API );
			EGLint conf_list[] = { 
					EGL_RED_SIZE, 8,
					EGL_GREEN_SIZE, 8,
					EGL_BLUE_SIZE, 8,
					EGL_ALPHA_SIZE, 8,
					EGL_NONE 
					};
			EGLConfig config;
			EGLint num_config;
			eglChooseConfig( display, conf_list, &config, 1, &num_config );
			surface = eglCreateWindowSurface( display, config, hWnd, NULL );
			context = eglCreateContext( display, 0, NULL, NULL );
			ballImage = vgLoadImage(TEXT("ball-r.png"));
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
void draw(void){
	
	eglMakeCurrent(display,surface,surface,context);
	
	vgSeti(VG_MATRIX_MODE,VG_MATRIX_IMAGE_USER_TO_SURFACE);
	VGfloat clearColor[4]={2,2,0,2};
	vgSetfv(VG_CLEAR_COLOR,4,clearColor);
	vgClear(0,0,400,400);
	vgLoadIdentity();
	vgTranslate(ball.x,ball.y);
	vgDrawImage(ballImage);

	eglSwapBuffers(display,surface);
	
}

void timerProc(){
	
	if(ball.dir == rightup){
		ball.x +=5; ball.y+=5;
			if(ball.x>400-50) ball.dir = leftup;
			else if(ball.y>400-105) ball.dir = rightdown;
	}
	else if(ball.dir == rightdown){
		ball.x +=5; ball.y-=5;
			if(ball.x>400-50) ball.dir = leftdown;
			else if(ball.y<0) ball.dir = rightup;
	}
	else if(ball.dir == leftdown){
		ball.x -=5; ball.y-=5;
			if(ball.y<0) ball.dir = leftup;
			else if(ball.x<0) ball.dir = rightdown;
	}
	else if(ball.dir == leftup){
		ball.x -=5; ball.y+=5;
			if(ball.x<0) ball.dir = rightup;
			else if(ball.y>400-105) ball.dir = leftdown;
	}
	
		
	draw();
}
