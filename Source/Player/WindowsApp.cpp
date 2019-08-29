
// WindowsApp.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "WindowsPrecompiled.h"
#include "Res/resource.h"

static HWND hMainWnd;
static HMENU hMenu;
static HMENU hSettingMenu;
static HMENU hSubRenderMenu;
static HACCEL hAccelTable;

static const TCHAR * className = _T("SOFTRENDERER_PLAYER");
static TCHAR szTitle[100];
static TCHAR szFullTitle[100];
#define SIZEOF(a) ((int)(sizeof(a) / sizeof((a)[0])))

static void InitInstance(HINSTANCE hInstance, LPCTSTR lpCmdLine, int nCmdShow);
static HWND CreateAppWindow(const TCHAR *title, const TCHAR *classname, int width, int height, bool fullscreen);
static void DestroyAppWindow(HWND hwnd);
static void ShutdownInstance();
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static bool LoopInstance();
static void UpdateMenuStatus();

void UpdateMenuStatus()
{
	//CheckMenuRadioItem(hSubRenderMenu, IDM_2D, IDM_3DPERSP, IDM_2D, MF_BYCOMMAND);
	//CheckMenuRadioItem(hSubRenderMenu, IDM_2D, IDM_3DPERSP, IDM_3DPERSP, MF_BYCOMMAND);
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) 
{
	//const HCURSOR hcurSave = ::SetCursor(LoadCursor(0, IDC_WAIT));
	::LoadString(hInstance, IDS_APP_TITLE, szTitle, SIZEOF(szTitle));

	InitInstance(hInstance, lpCmdLine, nCmdShow);
	hAccelTable = ::LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOFTRENDERER));
	::SetFocus(hMainWnd);

	hMenu = ::GetMenu(hMainWnd);
	hSettingMenu = ::GetSubMenu(hMenu, 1);
	hSubRenderMenu = ::GetSubMenu(hSettingMenu, 0);

	UpdateMenuStatus();

	double prevShowTitleSec = 0;

	while (1)
	{
		if (!LoopInstance()) 
		{
			break;
		}

		float frameFPS = 0.f;
		float averageFPS = 0.f;
		_stprintf_s(szFullTitle, "%s Current : %.2fFPS Average : %.2fFPS", szTitle, frameFPS, averageFPS);
		::SetWindowText(hMainWnd, szFullTitle);
	}

	ShutdownInstance();
	return 0;
}

static void InitInstance(HINSTANCE hInstance, LPCTSTR lpCmdLine, int nCmdShow) 
{
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOFTRENDERER));
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.hbrBackground = (HBRUSH)(COLOR_GRAYTEXT + 1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_SOFTRENDERER);
	wcex.lpszClassName = className;
	RegisterClassEx(&wcex);

	hMainWnd = CreateAppWindow(szTitle, className, 800, 600, false);
}

static HWND CreateAppWindow(const TCHAR *title, const TCHAR *classname, int width, int height, bool fullscreen) 
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = 800 - 1;
	rect.bottom = 600 - 1;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	int WindowWidth = rect.right - rect.left + 1;
	int WindowHeight = rect.bottom - rect.top + 1;
	
	HWND hwnd = CreateWindow(className, szTitle, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
		(GetSystemMetrics(SM_CXFULLSCREEN) - WindowWidth) / 2, (GetSystemMetrics(SM_CYFULLSCREEN) - WindowHeight) / 2, WindowWidth, WindowHeight,
		nullptr, nullptr, (HINSTANCE)GetModuleHandle(NULL), nullptr);
	
	::ShowWindow(hwnd, SW_SHOW);
	::SetForegroundWindow(hwnd);
	::SetFocus(hwnd);

	return hwnd;
}

static void DestroyAppWindow(HWND hwnd) 
{
	::DestroyWindow(hwnd);
}

static void ShutdownInstance() 
{
	DestroyAppWindow(hMainWnd);
}

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	int wmId, wmEvent;

	switch (message) 
	{
	case WM_CREATE:
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_DPICHANGED:
		break;
	case WM_SYSCOMMAND:
		if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER) 
		{
			return 0;
		}
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId) 
		{
		case IDM_ABOUT:
			DialogBox((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
			break;
		case IDM_2D:
			UpdateMenuStatus();
			break;
		case IDM_3DPERSP:
			UpdateMenuStatus();
			break;
		case IDM_EXIT:
			DestroyWindow(hwnd);
			break;
		}
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

static bool LoopInstance() 
{
	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
	{
		if (msg.message == WM_QUIT) 
		{
			return false;
		}

		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// Set Update Login Here.
	Sleep(10);

	return true;
}

