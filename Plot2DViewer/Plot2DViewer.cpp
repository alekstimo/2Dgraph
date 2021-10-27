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
LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);						// прототип оконной процедуры
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)		// основная процедура
{
	// Первая составляющая часть основной процедуры - создание окна: сначала описывается оконный класс wc, затем создаётся окно hWnd
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;						// имя оконной процедуры, закрепленной за данным классом
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;						// идентификатор приложения, содержащий адрес начала выделенной ему области памяти
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = 0;							// меню в оконном классе отсутствует
	wc.lpszClassName = (LPCSTR)"MainWindowClass";	// имя оконного класса, используемое при создании экземпляров окна
	RegisterClass(&wc);								// регистрация класса wc

	HWND hWnd = CreateWindow(						// hWnd - дескриптор, идентифицирующий окно; функция создания окна заполняет дескриптор hWnd ненулевым значением
		(LPCSTR)"MainWindowClass",					// имя оконного класса
		(LPCSTR)"Plot2D Viewer",					// заголовок окна
		WS_OVERLAPPEDWINDOW,						// стиль окна
		200,200,400,400,							// координаты на экране левого верхнего угла окна, его ширина и высота
		nullptr,nullptr,hInstance,nullptr);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	//MessageBox(NULL, "Graph", "Select", MB_DEFBUTTON1,MB_DEFBUTTON2);

	// Вторая составляющая часть основной процедуры - основной цикл обработки системных сообщений, который ожидает сообщения и рассылает их соответствующим окнам
	MSG msg;
	while(GetMessage(&msg,nullptr,0,0))				// функция GetMessage выбирает из очереди сообщение и заносит его в структуру msg
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);						// функция DispatchMessage оповещает систему о необходимости вызова оконной процедуры
	}

	return 0;
}

// В основном модуле объявляется только одна глобальная переменная - создаётся объект класса Scene2D
// Все дальнейшие действия осуществляются посредством обращения к методам, реализованным в этом классе
Scene2D scene(L,R,B,T);
void (*Func)(double&, double&, double)=Sinusoid;
bool flag=true;
LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)		// оконная процедура принимает и обрабатывает все сообщения, отправленные окну
{
	switch(msg)
	{
	case WM_CREATE:
		/* при создании окна внедряем в него кнопочку */
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
		/* нажата наша кнопочка? */
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

			scene.Clear(dc,hWnd);				// Вызов реализованного в классе Camera2D метода, отвечающего за очистку рабочей области окна hWnd
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
