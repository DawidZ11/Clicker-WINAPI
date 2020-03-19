#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>

// GLOBALS
#pragma region Window Stuff

const LPCWSTR lpszClassName = L"MojaKlasaOkna";
const LPCWSTR lpszWindowTitle = L"Window";

const int width = 525;
const int height = 275;
const int posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
const int posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

HWND hWnd;

#pragma endregion

#pragma region Controls

HWND hButton1;
HWND hButton2;

HWND hStatic1;

HWND hEdit1;

HWND hListBox1;

HWND hGroupBox1;

#pragma endregion

#pragma region Control Stuff

UINT Timer01Interval = 300; // click
UINT Timer02Interval = 10;

#pragma endregion

#pragma region Controls IDs

#define BUTTON_01 1
#define BUTTON_02 2

const UINT_PTR TIMER_01 = 101;
const UINT_PTR TIMER_02 = 102;

#define LISTBOX_01 201

#pragma endregion

COLORREF TestPanelColor;
POINT currMousePos;
std::vector<POINT> t_pos;
std::wstring ws;
int p = 0;

#pragma region Function Prototypes

bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow);
WPARAM Run();
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#pragma endregion

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!CreateMainWindow(hInstance, nCmdShow))
	{
		MessageBox(NULL, L"Could not create window", L"Error", MB_ICONERROR | MB_OK);
		return 0;
	}

	RegisterHotKey(hWnd, 1001, 0, VK_SPACE);
	RegisterHotKey(hWnd, 1002, 0, VK_TAB);

	hButton1 = CreateWindowEx(0, L"BUTTON", L"Start", WS_CHILD | WS_VISIBLE | WS_BORDER, 50, 50, 100, 25, hWnd, (HMENU)BUTTON_01, hInstance, 0);
	hButton2 = CreateWindowEx(0, L"BUTTON", L"Stop", WS_CHILD | WS_VISIBLE | WS_BORDER, 200, 50, 100, 25, hWnd, (HMENU)BUTTON_02, hInstance, 0);
	hStatic1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"STATIC", L"           Mouse Position(0, 0)", WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT, 50, 85, 250, 25, hWnd, NULL, hInstance, 0);
	hGroupBox1 = CreateWindowEx(0, L"BUTTON", L"Mouse Options", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_GROUPBOX, 25, 25, 450, 105, hWnd, NULL, hInstance, 0);
	hListBox1 = CreateWindowEx(0, L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL, 325, 50, 125, 75, hWnd, (HMENU)LISTBOX_01, hInstance, 0);
	hEdit1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT",  NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT, 25, 145, 450, 25, hWnd, NULL, hInstance, 0);

	SetTimer(hWnd, TIMER_02, Timer02Interval, (TIMERPROC)NULL);

	return Run();
}

bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = WndProc;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszClassName = lpszClassName;
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.lpszMenuName = NULL;

	if (!RegisterClassEx(&wcex))
		return false;

	hWnd = CreateWindowEx(WS_EX_CLIENTEDGE | WS_EX_TOPMOST, lpszClassName, lpszWindowTitle, WS_OVERLAPPEDWINDOW, posX, posY, width, height, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
		return false;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return true;
}

WPARAM Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT)
	{
		if (msg.wParam == 1001)
		{
				KillTimer(hWnd, TIMER_01);
		}
		if (msg.wParam == 1002)
		{
			t_pos.push_back(currMousePos);

			ws = L"Pos( " + std::to_wstring(currMousePos.x) + L", " + std::to_wstring(currMousePos.y) + L" )";

			SendMessage(hListBox1, LB_ADDSTRING, 0, (LPARAM)ws.c_str());
		}

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// render stuff
		}
	}


	KillTimer(hWnd, TIMER_02);
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CLOSE:
		{
			DestroyWindow(hWnd);
			return 0;
		} break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		} break;
		case WM_COMMAND:
		{
			switch (wParam)
			{
				case BUTTON_01:
				{
					SetTimer(hWnd, TIMER_01, Timer01Interval, (TIMERPROC)NULL);
				} break;
				case BUTTON_02:
				{
					KillTimer(hWnd, TIMER_01);
				} break;
			}
			return 0;
		} break;
		case WM_TIMER:
		{
			switch (wParam)
			{
				case TIMER_01:
				{
					srand(time(NULL));
					TestPanelColor = RGB(rand() % 255, rand() % 255, rand() % 255);
					InvalidateRect(hWnd, NULL, true);



					if (p < t_pos.size())
					{
						SetCursorPos(t_pos[p].x, t_pos[p].y);
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

						p++;
					}
					if (p == t_pos.size())
						p = 0;
				}
				case TIMER_02:
				{
					GetCursorPos(&currMousePos);

					std::wstring string = L"           Mouse Position( " + std::to_wstring(currMousePos.x) + L", " + std::to_wstring(currMousePos.y) + L" )";

					SetWindowText(hStatic1, string.c_str());
				}
			}
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);

			RECT rc;
			rc.left = 25;
			rc.top = 180;
			rc.right = rc.left + 450;
			rc.bottom = rc.top + 25;

			HBRUSH hbr = CreateSolidBrush(TestPanelColor);

			FillRect(ps.hdc, &rc, hbr);

			DeleteObject(hbr);

			EndPaint(hWnd, &ps);
		}
		default:
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		} break;
	}
}