#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "Data.h"
#include "Scene2D.h"
#include "Matrix.h"
#include "AffineTransform.h"
#include "Model2D.h"



//#define LOBYTE(w)   ((BYTE) (w))
//#define HIBYTE(w)   ((BYTE) (((WORD) (w) >> 8) & 0xFF))
HINSTANCE hInstance = NULL;
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
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
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
int Angle = 0;
int Mapping = IDC_RADIO2;
int TranslationX = 0, TranslationY = 0;
int ScalingX = 1, ScalingY = 1;
int Scl = IDC_BIG;
INT_PTR CALLBACK OptionsProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam) {
	switch (Msg)
	{
	case WM_INITDIALOG:
	{
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO4, Angle);
		CheckRadioButton(hDlg, IDC_BIG, IDC_SML, ScalingX);
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			case IDOK:
			{
				TranslationX = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
				TranslationY = GetDlgItemInt(hDlg, IDC_EDIT2, NULL, TRUE);

				ScalingX = GetDlgItemInt(hDlg, IDC_SCRX, NULL, FALSE);
				ScalingY = GetDlgItemInt(hDlg, IDC_SCRY, NULL, FALSE);
				if (BST_CHECKED == IsDlgButtonChecked(hDlg, IDC_BIG)) Scl = IDC_BIG;
				if (BST_CHECKED == IsDlgButtonChecked(hDlg, IDC_SML)) Scl = IDC_SML;

				if (BST_CHECKED == IsDlgButtonChecked(hDlg, IDC_RADIO1)) {
					Angle = GetDlgItemInt(hDlg, IDC_TEXT, NULL, TRUE);
					Mapping = IDC_RADIO1;
				}
				if (BST_CHECKED == IsDlgButtonChecked(hDlg, IDC_RADIO2)) Mapping = IDC_RADIO2;
				if (BST_CHECKED == IsDlgButtonChecked(hDlg, IDC_RADIO3)) Mapping = IDC_RADIO3;
				if (BST_CHECKED == IsDlgButtonChecked(hDlg, IDC_RADIO4)) Mapping = IDC_RADIO4;
				EndDialog(hDlg, 0);
				return TRUE;
			}
			case IDCANCEL: {
				ScalingX = 1;
				ScalingY = 1;
				TranslationX = 0;
				TranslationY = 0;
				Angle = 0;
				Mapping = IDC_RADIO1;
				EndDialog(hDlg, 0);
				return TRUE;
			}
			
		}
		break;
	}
	
	}
	return FALSE;
}


Scene2D scene(L,R,B,T,V,E);
void (*Func)(double&, double&, double)=Sinusoid;
bool flag=true;
COLORREF colorGraph = RGB(200, 0, 100), colorModel = RGB(0,200,100);
LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)		// оконная процедура принимает и обрабатывает все сообщения, отправленные окну
{
	switch(msg)
	{
	
	case WM_COMMAND:
		/* нажата наша кнопочка? */
		if (HIWORD(wParam) == 0)
			switch (LOWORD(wParam)) {
			case ID_WINDOW_EXIT:
			{
				PostMessage(hWnd, WM_CLOSE, 0, 0);
				break;
			}
				case ID_GRAPHICS_SINUSOID:
				{
					Func = &Sinusoid;
					flag = true;
					scene.change(-M_PI, M_PI, 1.5, -1.5, 0, 0);
					scene.plot(Func, flag);
					break;
				}
				case ID_GRAPHICS_PARABOLA:
				{
					Func = &Parabola;
					flag = true;
					scene.change(-M_PI, M_PI, 1.5, -1.5, 0, 0);
					scene.plot(Func, flag);
					break;
				}
				case ID_GRAPHICS_BUTTERFLY: 
				{
					Func = &Butterfly;
					flag = false;
					scene.change(-2 * M_PI, 2 * M_PI, 3, -3, 0, 37.8);	
					scene.plot(Func, flag);
					break;
				}
				case ID_GRAPHICS_STAR: {
					Func = &Star;
					flag = false;
					scene.change(-M_PI, M_PI, 1.5, -1.5, 0, 2 * M_PI);
					scene.plot(Func, flag);
					break;
				}
				case ID_GRAPHICS_ELLIPSE: {
					Func = &Ellipse;
					flag = false;
					scene.change(-M_PI, M_PI, 1.5, -1.5, 0, 2 * M_PI);
					scene.plot(Func, flag);
					break;
				}
				case ID_GRAPHICS_SPIRAL: {
					Func = &Spiral;
					flag = false;
					scene.change(-4 * M_PI, 4 * M_PI, 6, -6, 0, 5 * M_PI);
					scene.plot(Func, flag);
					break;
				}
				case ID_GRAPHICS_KARDIOLA: {
					Func = &Kardiola;
					flag = false;
					scene.change(-2 * M_PI, 2 * M_PI, 3, -3, 0, 2 * M_PI);
					scene.plot(Func, flag);
					break;
				}
				case ID_GRAPHICS_LASSAGHU: {
					Func = &Lassaghu;
					flag = false;
					scene.change(-M_PI, M_PI, 1.5, -1.5, 0, 2 * M_PI);
					scene.plot(Func, flag);
					break;
				}
				case ID_GRAPH_RED: {
					colorGraph = RGB(200, 0, 100);
					break;
				}
				case ID_GRAPH_BLUE: {
					colorGraph = RGB(0, 100, 200);
					break;
				}
				case ID_GRAPH_GREEN: {
					colorGraph = RGB(0, 200, 100);
					break;
				}
				case ID_GRAPH_PURPLE: {
					colorGraph = RGB(200, 10, 200);
					break;
				}
				case ID_MODEL_RED: {
					colorModel = RGB(200, 0, 100);
					break;
				}
				case ID_MODEL_BLUE: {
					colorModel = RGB(0, 100, 200);
					break;
				}
				case ID_MODEL_GREEN: {
					colorModel = RGB(0, 200, 100);
					break;
				}
				case ID_MODEL_PURPLE: {
					colorModel = RGB(200, 10, 200);
					break;
				}
				case ID_MODEL_YELLOW: {
					colorModel = RGB(255, 255, 0);
					break;
				}
				case ID_AFFINETRANSFORM_ROTATION:{
					DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, OptionsProc);
					switch (Mapping) {
						case IDC_RADIO1:
						{
							scene.Model.Apply(Rotation(Angle));
							break;
						}
						case IDC_RADIO2:
						{
							scene.Model.Apply(Scaling(-1,1));
							break;
						}
						case IDC_RADIO3:
						{
							scene.Model.Apply(Scaling(1, -1));
							break;
						}
						case IDC_RADIO4:
						{
							scene.Model.Apply(Scaling(-1, -1));
							break;
						}
					}
					
					break;
				}
				case ID_AFFINETRANSFORM_TRANSLATION: {
					DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, OptionsProc);
					scene.Model.Apply(Translation(TranslationX, TranslationY));
					break;
				}
				case ID_AFFINETRANSFORM_SCALING:
				{
					DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, OptionsProc);
					switch (Scl) {
						case IDC_BIG: {
							scene.Model.Apply(Scaling(ScalingX, ScalingY));
							break;
						}
						case IDC_SML: {
							scene.Model.Apply(Scaling(1./ScalingX, 1./ScalingY));
							break;
						}
					}
					break;
				}

			}
		InvalidateRect(hWnd, nullptr, false);
		return 0;
	case WM_PAINT:
		{
			
			HDC dc = GetDC(hWnd);

			scene.Clear(dc,hWnd);				// Вызов реализованного в классе Camera2D метода, отвечающего за очистку рабочей области окна hWnd
			scene.Plot(dc, Func, flag,colorGraph);
			scene.Render(dc,colorModel);
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
			POINT P;
			P.x = GET_X_LPARAM(lParam);
			P.y = GET_Y_LPARAM(lParam);
			ScreenToClient(hWnd, &P);
			scene.ChangeSize(P.x, P.y,zDelta);
			InvalidateRect(hWnd, nullptr, false);
		
		return 0;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
			case VK_RIGHT:
			{
				//CreateWindow("button", "Sinusoid", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
					//5, 5, 100, 20, hWnd, (HMENU)IDD_DIALOG1, NULL, NULL);
				scene.Model.Apply(Translation(0.5, 0));
				break;
			}
			case VK_LEFT:
			{
				scene.Model.Apply(Translation(-0.5, 0));
				break;
			}
			case VK_UP:
			{
				scene.Model.Apply(Translation(0, 0.5));
				break;
			}
			case VK_DOWN:
			{
				scene.Model.Apply(Translation(0, -0.5));
				break;
			}
			case VK_ADD: {
				scene.Model.Apply(Rotation(30));
				break;
			}
			case VK_SUBTRACT:
			{
				scene.Model.Apply(Rotation(-30));
				break;
			}
			case VK_MULTIPLY: {
				scene.Model.Apply(Scaling(1.6, 1.6));
				break;
			}
			case VK_DIVIDE:
			{
				scene.Model.Apply(Scaling(0.625, 0.625));
				break;
			}
		}
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
