#include <windows.h>
#include <windowsx.h>
#include "Data.h"
#include "Scene2D.h"

#define ID_SIN 1
#define ID_PAR 2
#define ID_BUT 3
#define ID_STR 4
#define ID_ELL 5
#define ID_SPR 6
#define ID_KRD 7
#define ID_LSG 8
#define LOBYTE(w)   ((BYTE) (w))
#define HIBYTE(w)   ((BYTE) (((WORD) (w) >> 8) & 0xFF))
#define LOWORD(l)   ((WORD) (l))
#define HIWORD(l)   ((WORD) (((DWORD) (l) >> 16) & 0xFFFF))
LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);						// �������� ������� ���������
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)		// �������� ���������
{
	// ������ ������������ ����� �������� ��������� - �������� ����: ������� ����������� ������� ����� wc, ����� �������� ���� hWnd
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;						// ��� ������� ���������, ������������ �� ������ �������
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;						// ������������� ����������, ���������� ����� ������ ���������� ��� ������� ������
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = 0;							// ���� � ������� ������ �����������
	wc.lpszClassName = (LPCSTR)"MainWindowClass";	// ��� �������� ������, ������������ ��� �������� ����������� ����
	RegisterClass(&wc);								// ����������� ������ wc

	HWND hWnd = CreateWindow(						// hWnd - ����������, ���������������� ����; ������� �������� ���� ��������� ���������� hWnd ��������� ���������
		(LPCSTR)"MainWindowClass",					// ��� �������� ������
		(LPCSTR)"Plot2D Viewer",					// ��������� ����
		WS_OVERLAPPEDWINDOW,						// ����� ����
		200,200,400,400,							// ���������� �� ������ ������ �������� ���� ����, ��� ������ � ������
		nullptr,nullptr,hInstance,nullptr);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	//MessageBox(NULL, "Graph", "Select", MB_DEFBUTTON1,MB_DEFBUTTON2);

	// ������ ������������ ����� �������� ��������� - �������� ���� ��������� ��������� ���������, ������� ������� ��������� � ��������� �� ��������������� �����
	MSG msg;
	while(GetMessage(&msg,nullptr,0,0))				// ������� GetMessage �������� �� ������� ��������� � ������� ��� � ��������� msg
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);						// ������� DispatchMessage ��������� ������� � ������������� ������ ������� ���������
	}

	return 0;
}

// � �������� ������ ����������� ������ ���� ���������� ���������� - �������� ������ ������ Scene2D
// ��� ���������� �������� �������������� ����������� ��������� � �������, ������������� � ���� ������
Scene2D scene(L,R,B,T);
void (*Func)(double&, double&, double)=Sinusoid;
bool flag=true;
LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)		// ������� ��������� ��������� � ������������ ��� ���������, ������������ ����
{
	switch(msg)
	{
	case WM_CREATE:
		/* ��� �������� ���� �������� � ���� �������� */
		CreateWindow("button", "Sinusoid", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			5, 5, 100, 20, hWnd, (HMENU)ID_SIN, NULL, NULL);
		CreateWindow("button", "Parabola", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			5, 30, 100, 20, hWnd, (HMENU)ID_PAR, NULL, NULL);
		CreateWindow("button", "Butterfly", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			5, 55, 100, 20, hWnd, (HMENU)ID_BUT, NULL, NULL);
		CreateWindow("button", "Star", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			5, 80, 100, 20, hWnd, (HMENU)ID_STR, NULL, NULL);
		CreateWindow("button", "Ellipse", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			5, 105, 100, 20, hWnd, (HMENU)ID_ELL, NULL, NULL);
		CreateWindow("button", "Sriral", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			5, 130, 100, 20, hWnd, (HMENU)ID_SPR, NULL, NULL);
		CreateWindow("button", "Kardiola", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			5, 155, 100, 20, hWnd, (HMENU)ID_KRD, NULL, NULL);
		CreateWindow("button", "Lassaghu", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			5, 180, 100, 20, hWnd, (HMENU)ID_LSG, NULL, NULL);
		return 0;
	case WM_COMMAND:
		/* ������ ���� ��������? */
		if (HIWORD(wParam) == 0)
			switch (LOWORD(wParam)) {
				case ID_SIN: {
					Func = &Sinusoid;
					flag = true;
					scene.change(-M_PI, M_PI, 1.5, -1.5, 0, 0);
					break;
				}
				case ID_PAR: {
					Func = &Parabola;
					flag = true;
					scene.change(-M_PI, M_PI, 1.5, -1.5, 0, 0);
					break;
				}
				case ID_BUT: {
					Func = &Butterfly;
					flag = false;
					scene.change(-2 * M_PI, 2 * M_PI, 3, -3, 0, 37.8);
					break;
				}
				case ID_STR: {
					Func = &Star;
					flag = false;
					scene.change(-M_PI, M_PI, 1.5, -1.5, 0, 2 * M_PI);
					break;
				}
				case ID_ELL: {
					Func = &Ellipse;
					flag = false;
					scene.change(-M_PI, M_PI, 1.5, -1.5, 0, 2 * M_PI);
					break;
				}
				case ID_SPR: {
					Func = &Spiral;
					flag = false;
					scene.change(-4 * M_PI, 4 * M_PI, 6, -6, 0, 5 * M_PI);
					break;
				}
				case ID_KRD: {
					Func = &Kardiola;
					flag = false;
					scene.change(-2 * M_PI, 2 * M_PI, 3, -3, 0, 2 * M_PI);
					break;
				}
				case ID_LSG: {
					Func = &Lassaghu;
					flag = false;
					scene.change(-M_PI, M_PI, 1.5, -1.5, 0, 2 * M_PI);
					break;
				}
			}
		InvalidateRect(hWnd, nullptr, false);
		return 0;
	case WM_PAINT:
		{
			
			HDC dc = GetDC(hWnd);

			scene.Clear(dc,hWnd);				// ����� �������������� � ������ Camera2D ������, ����������� �� ������� ������� ������� ���� hWnd
			scene.Plot(dc, Func, flag);
			ReleaseDC(hWnd,dc);
			return DefWindowProc(hWnd,msg,wParam,lParam);
		}

	case WM_SIZE:
		{
			HDC dc = GetDC(hWnd);
			scene.SetResolution(hWnd);
			ReleaseDC(hWnd,dc);
			InvalidateRect(hWnd,nullptr,false);
			return 0;
		}
	case WM_LBUTTONDOWN:
	{
			scene.StartDragging(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		HDC dc = GetDC(hWnd);
		scene.point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), dc);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		InvalidateRect(hWnd, nullptr, false);
	}
	case WM_MOUSEMOVE:
	{
		if (scene.IsDragging())
		{
			scene.Drag(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			InvalidateRect(hWnd, nullptr, false);
		}
		return 0;
	}
	case WM_LBUTTONUP:
	{
		scene.StopDragging();
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
			RECT rect;
			GetWindowRect(hWnd, &rect);
			bool zDelta = GET_WHEEL_DELTA_WPARAM(wParam) > 0;
			scene.ChangeSize(GET_X_LPARAM(lParam)- rect.left, GET_Y_LPARAM(lParam) - rect.top,zDelta);
			InvalidateRect(hWnd, nullptr, false);
		
		return 0;
	}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

	default:
		{
			return DefWindowProc(hWnd,msg,wParam,lParam);
		}
	}
	return 0;
}
