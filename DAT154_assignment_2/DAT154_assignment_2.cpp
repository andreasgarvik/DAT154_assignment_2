// DAT154_assignment_2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "DAT154_assignment_2.h"
#include "Car.cpp"
#include <list>
#include <stdlib.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//Graphics
void Rectangle(HDC*, int, int, int, int, COLORREF);
void Circle(HDC*, int, int, int, int, COLORREF);
void TrafficLights(HDC*);
void Roads(HDC*);
int light = 0;
int changeLight = 0;

std::list<Car*> cars;
std::list<Car*>::iterator it;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DAT154ASSIGNMENT2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DAT154ASSIGNMENT2));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DAT154ASSIGNMENT2));
wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DAT154ASSIGNMENT2);
wcex.lpszClassName = szWindowClass;
wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SetTimer(hWnd, 0, 30, (TIMERPROC)NULL);
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
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
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		TrafficLights(&hdc);
		Roads(&hdc);

		// Cars
		for (it = cars.begin(); it != cars.end(); ++it) {
			Rectangle(&hdc, (*it)->GetX(), (*it)->GetY(), (*it)->GetX() + 25, (*it)->GetY() + 25, RGB(0, 0, 255));
			Rectangle(&hdc, (*it)->GetX(), (*it)->GetY(), (*it)->GetX() + 25, (*it)->GetY() + 25, RGB(0, 0, 255));
			if ((*it)->GetY() > 775 || (*it)->GetX() > 1410) {
				//delete (*it);
			}
		}

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_TIMER:
	{
		int i = rand() % 100;
		int j = rand() % 100;
		if (i > 95) {
			cars.push_back(new Car(10, rand() % 80 + 285, false));
			if (j > 50) {
				cars.push_back(new Car(rand() % 80 + 655, 10, true));
			}
		}
		if (changeLight == 1770) {
			light = (light + 1) % 6;
			InvalidateRect(hWnd, 0, true);
		}
		for (it = cars.begin(); it != cars.end(); ++it) {
			if ((*it)->GetDrivingSouth()) {
				if (light == 3 || light == 4) {
					(*it)->Move();
					InvalidateRect(hWnd, 0, true);
				}
				else {
					if ((*it)->GetY() <= 265 || (*it)->GetY() > 295) {
						(*it)->Move();
						InvalidateRect(hWnd, 0, true);
					}
				}
			}
			else {
				if (light == 0 || light == 1) {
					(*it)->Move();
					InvalidateRect(hWnd, 0, true);
				}
				else {
					if ((*it)->GetX() <= 635 || (*it)->GetX() > 665) {
						(*it)->Move();
						InvalidateRect(hWnd, 0, true);
					}
				}
			}
		}
		changeLight = (changeLight + 30) % 1800;
		}
		break;
	case WM_LBUTTONDOWN:
		{
			int i = rand() % 10;
			if (i > 5) {
				cars.push_back(new Car(10, rand() % 80 + 285, false));
			}
		}
		break;
	case WM_RBUTTONDOWN:
		{
			int i = rand() % 10;
			if (i > 5) {
				cars.push_back(new Car(rand() % 80 + 655, 10, true));
			}
		}
		break;
    case WM_DESTROY:
		KillTimer(hWnd, 0);
		KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

void Rectangle(HDC* hdc, int l, int t, int r, int b, COLORREF color) {
	HBRUSH hBrush = CreateSolidBrush(color);
	HGDIOBJ hOrg = SelectObject(*hdc, hBrush);
	RECT rct;
	rct.left = l;
	rct.top = t;
	rct.right = r;
	rct.bottom = b;
	FillRect(*hdc, &rct, hBrush);
	SelectObject(*hdc, hOrg);
	DeleteObject(hBrush);
}

void Circle(HDC* hdc, int l, int t, int r, int b, COLORREF color) {
	HBRUSH hBrush = CreateSolidBrush(color);
	HGDIOBJ hOrg = SelectObject(*hdc, hBrush);
	Ellipse(*hdc, l, t, r, b);
	SelectObject(*hdc, hOrg);
	DeleteObject(hBrush);
}

void TrafficLights(HDC* hdc) {
	switch (light)
	{
	case 0:
	{
		//Red
		Rectangle(hdc, 550, 10, 640, 260, RGB(0, 0, 0));
		Circle(hdc, 560, 20, 630, 90, RGB(255, 0, 0));
		Circle(hdc, 560, 100, 630, 170, RGB(192, 192, 192));
		Circle(hdc, 560, 180, 630, 250, RGB(192, 192, 192));

		//Green
		Rectangle(hdc, 540, 415, 630, 665, RGB(0, 0, 0));
		Circle(hdc, 550, 425, 620, 495, RGB(192, 192, 192));
		Circle(hdc, 550, 505, 620, 575, RGB(192, 192, 192));
		Circle(hdc, 550, 585, 620, 655, RGB(0, 255, 0));
	}
	break;
	case 1:
	{
		//Red
		Rectangle(hdc, 550, 10, 640, 260, RGB(0, 0, 0));
		Circle(hdc, 560, 20, 630, 90, RGB(255, 0, 0));
		Circle(hdc, 560, 100, 630, 170, RGB(192, 192, 192));
		Circle(hdc, 560, 180, 630, 250, RGB(192, 192, 192));

		// Yellow
		Rectangle(hdc, 540, 415, 630, 665, RGB(0, 0, 0));
		Circle(hdc, 550, 425, 620, 495, RGB(192, 192, 192));
		Circle(hdc, 550, 505, 620, 575, RGB(255, 255, 0));
		Circle(hdc, 550, 585, 620, 655, RGB(192, 192, 192));
	}
	break;
	case 2:
	{
		//Red Yellow
		Rectangle(hdc, 550, 10, 640, 260, RGB(0, 0, 0));
		Circle(hdc, 560, 20, 630, 90, RGB(255, 0, 0));
		Circle(hdc, 560, 100, 630, 170, RGB(255, 255, 0));
		Circle(hdc, 560, 180, 630, 250, RGB(192, 192, 192));

		//Red
		Rectangle(hdc, 540, 415, 630, 665, RGB(0, 0, 0));
		Circle(hdc, 550, 425, 620, 495, RGB(255, 0, 0));
		Circle(hdc, 550, 505, 620, 575, RGB(192, 192, 192));
		Circle(hdc, 550, 585, 620, 655, RGB(192, 192, 192));
	}
	break;
	case 3:
	{
		// Green
		Rectangle(hdc, 550, 10, 640, 260, RGB(0, 0, 0));
		Circle(hdc, 560, 20, 630, 90, RGB(192, 192, 192));
		Circle(hdc, 560, 100, 630, 170, RGB(192, 192, 192));
		Circle(hdc, 560, 180, 630, 250, RGB(0, 255, 0));

		//Red
		Rectangle(hdc, 540, 415, 630, 665, RGB(0, 0, 0));
		Circle(hdc, 550, 425, 620, 495, RGB(255, 0, 0));
		Circle(hdc, 550, 505, 620, 575, RGB(192, 192, 192));
		Circle(hdc, 550, 585, 620, 655, RGB(192, 192, 192));
	}
	break;
	case 4:
	{
		// Yellow
		Rectangle(hdc, 550, 10, 640, 260, RGB(0, 0, 0));
		Circle(hdc, 560, 20, 630, 90, RGB(192, 192, 192));
		Circle(hdc, 560, 100, 630, 170, RGB(255, 255, 0));
		Circle(hdc, 560, 180, 630, 250, RGB(192, 192, 192));

		//Red
		Rectangle(hdc, 540, 415, 630, 665, RGB(0, 0, 0));
		Circle(hdc, 550, 425, 620, 495, RGB(255, 0, 0));
		Circle(hdc, 550, 505, 620, 575, RGB(192, 192, 192));
		Circle(hdc, 550, 585, 620, 655, RGB(192, 192, 192));
	}
	break;
	case 5:
	{
		// Red
		Rectangle(hdc, 550, 10, 640, 260, RGB(0, 0, 0));
		Circle(hdc, 560, 20, 630, 90, RGB(255, 0, 0));
		Circle(hdc, 560, 100, 630, 170, RGB(192, 192, 192));
		Circle(hdc, 560, 180, 630, 250, RGB(192, 192, 192));

		//Red Yellow
		Rectangle(hdc, 540, 415, 630, 665, RGB(0, 0, 0));
		Circle(hdc, 550, 425, 620, 495, RGB(255, 0, 0));
		Circle(hdc, 550, 505, 620, 575, RGB(255, 255, 0));
		Circle(hdc, 550, 585, 620, 655, RGB(192, 192, 192));
	}
	break;
	default:
		break;
	}
}

void Roads(HDC* hdc) {
	Rectangle(hdc, 650, 10, 775, 675, RGB(105, 105, 105));
	Rectangle(hdc, 10, 280, 1410, 405, RGB(105, 105, 105));

	Rectangle(hdc, 662, 10, 664, 675, RGB(255, 255, 255));
	Rectangle(hdc, 761, 10, 763, 675, RGB(255, 255, 255));

	Rectangle(hdc, 10, 292, 1410, 294, RGB(255, 255, 255));
	Rectangle(hdc, 10, 391, 1410, 393, RGB(255, 255, 255));
}