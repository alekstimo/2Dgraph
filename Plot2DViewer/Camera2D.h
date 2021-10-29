#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <string>
#include <windows.h>

class Camera2D
{
protected:
	/*double deltXForSize;
	double deltYForSize;*/
	double deltForSize = 1.125;
	double deltaXForDrag, deltaYForDrag;
	int pensolid = 5;
	double pix = (R - L) / W;
	double L, R, B, T;					// Мировые координаты границ рабочей области окна
	int W, H;							// Разрешение рабочей области окна
	int WorldToScreenX(double X)		// Переход от мировых координат к экранным (для абсциссы)
	{
		return  W / (R - L) * (X - L);
	}
	int WorldToScreenY(double Y)		// Переход от мировых координат к экранным (для ординаты)
	{
		return  H - H / (T - B) * (Y - B);
	}
	double ScreenToWorldX(double X)		// Переход от экранных координат к мировым (для абсциссы)
	{
		return (X * (R - L)) / W + L;
	}
	double ScreenToWorldY(double Y)		// Переход от экранных координат к мировым (для ординаты)
	{
		return ((H - Y) * (T - B)) / H + B;
	}
private:
	double posX, posY;					// Позиция графического курсора в мировых координатах (для функций MoveTo и LineTo)
	bool isDragging;
	double prx, pry;
public:
	Camera2D(double L, double R, double B, double T) : L(L), R(R), B(B), T(T)
	{
	}
	void Clear(HDC dc, HWND hWnd)
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		FillRect(dc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
	}
	void SetResolution(HWND hWnd)
	{
		RECT rect;
		GetWindowRect(hWnd, &rect);
		W = rect.right - rect.left - 15;         // Данная процедура вызывается при изменении размеров окна
		H = rect.bottom - rect.top - 40;         // В ней задаются значения величин W, H, а также настраиваются значения параметров L, R, B, T таким образом, чтобы обеспечить одинаковые масштабы по координатным осям
		double R1 = R;
		R = (T - B) / 2 * W / H + (L + R) / 2;
		L = -(T - B) / 2 * W / H + (L + R1) / 2;
		deltaXForDrag = (R - L) / W;
		deltaYForDrag = (T - B) / H;
	}
	void MoveTo(double X, double Y)
	{
		posX = X;        // Перемещение графического курсора (posX, posY)
		posY = Y;       
	}
	void LineTo(HDC dc, double X, double Y)
	{
		::MoveToEx(dc, WorldToScreenX(posX), WorldToScreenY(posY), NULL);
		::LineTo(dc, WorldToScreenX(X), WorldToScreenY(Y));
		MoveTo(X, Y);
		// Отрисовка линии из текущей позиции графического курсора в точку (X, Y) и его перемещение в эту точку
		// Обратите внимание, что мы действуем в мировых координатах
		// При отрисовке линии могут быть использованы WinApi функции
		// ::MoveToEx(dc, Xs, Ys, nullptr) и ::LineTo(dc, Xs, Ys)
	}
	void Axes(HDC dc)
	{
		MoveTo(L, 0);
		LineTo(dc, R, 0);
		::LineTo(dc, WorldToScreenX(R) - 10, WorldToScreenY(0) - 10); //отрисовка стрелок
		::MoveToEx(dc,WorldToScreenX(R), WorldToScreenY(0),NULL);
		::LineTo(dc, WorldToScreenX(R) - 10, WorldToScreenY(0) + 10);
		TextOut(dc, WorldToScreenX(R)-15, WorldToScreenY(0)+10, "X",1);
		MoveTo(0, B);
		LineTo(dc, 0, T);
		::LineTo(dc, WorldToScreenX(0) - 10, WorldToScreenY(T) + 10);
		::MoveToEx(dc, WorldToScreenX(0), WorldToScreenY(T), NULL);
		::LineTo(dc, WorldToScreenX(0) + 10, WorldToScreenY(T) + 10);
		TextOut(dc, WorldToScreenX(0)-15, WorldToScreenY(T)+10, "Y", 1);

		for (double i = 0; i < R; i += M_PI/2) {
			::MoveToEx(dc, WorldToScreenX(i), WorldToScreenY(0) - 10, NULL); //отрисовка засечек для Х  по пи/2
			::LineTo(dc, WorldToScreenX(i), WorldToScreenY(0) + 10);
		}
		for (double i = 0; i > L; i -= M_PI / 2) {
			::MoveToEx(dc, WorldToScreenX(i), WorldToScreenY(0) - 10, NULL);
			::LineTo(dc, WorldToScreenX(i), WorldToScreenY(0) + 10);
		}
		for (int i = 1; i < T; i++) {
			::MoveToEx(dc, WorldToScreenX(0)-10, WorldToScreenY(i), NULL); //отрисовка засечек для Y по 1
			::LineTo(dc, WorldToScreenX(0)+10, WorldToScreenY(i));
		}
		for (int i = 1; i > B; i--) {
			::MoveToEx(dc, WorldToScreenX(0) - 10, WorldToScreenY(i), NULL);
			::LineTo(dc, WorldToScreenX(0) + 10, WorldToScreenY(i));
		}
		TextOut(dc, WorldToScreenX(0), WorldToScreenY(0), "0", 1);
	}
	//void ChangeSize(double X, double Y,bool inc)
	//{
	//	
	//	double deltX = (abs(R)+abs(L)) / 24;
	//	double deltY = (abs(T)+abs(B)) / 24;
	//	prx = ScreenToWorldX(X);
	//	pry = ScreenToWorldY(Y);
	//	if (inc)
	//	{ 
	//		if (abs(R - prx) > abs(L - prx)) {
	//			R -= deltX;
	//			//L += deltX;
	//		}
	//		else if (abs(R - prx) < abs(L - prx)) {
	//			//R -= deltX;
	//			L += deltX;
	//		}
	//		else {
	//			R -= deltX;
	//			L += deltX;
	//		}
	//		if (abs(T - pry) > abs(B - pry)) {
	//			T -= deltY;
	//			//B += deltY;
	//		}
	//		else if (abs(T - pry) < abs(B - pry)) {
	//			//T -= deltY;
	//			B += deltY;
	//		}
	//		else {
	//			T -= deltY;
	//			B += deltY;
	//		}
	//	}
	//	else  {
	//		R += deltX;
	//		T += deltY;
	//		L -= deltX;
	//		B -= deltY;
	//	}

	//}
	void ChangeSize(double X, double Y, bool inc)
	{

		/*double deltX = M_PI / 24;
		double deltY = 0.8;*/
		prx = ScreenToWorldX(X);
		pry = ScreenToWorldY(Y);
		if (inc)
		{
			pensolid++;
			/*R -=abs(R-prx)*deltXForSize;
			L += abs(L - prx) * deltXForSize;
			T -= abs(T - pry) * deltYForSize;
			B += abs(B - pry) * deltYForSize;*/
			L = prx - (prx - L) / deltForSize;
			R = prx + (R-prx) / deltForSize;
			T = pry + (T-pry) / deltForSize;
			B = pry - (pry-B) / deltForSize;
		}
		else
		{
			if (pensolid > 1)
				pensolid--;
			/*R += abs(R - prx) * deltXForSize;
			L -= abs(L - prx) * deltXForSize;
			T += abs(T - pry) * deltYForSize;
			B -= abs(B - pry) * deltYForSize;*/
			L = prx - (prx-L) * deltForSize;
			R = prx + (R - prx) * deltForSize;
			T = pry + (T-pry) * deltForSize;
			B = pry - (pry-B) * deltForSize;
		}

	}
	void StartDragging(int X, int Y)
	{
		/*prx = ScreenToWorldX(X);
		pry = ScreenToWorldY(Y);*/
		prx = X;
		pry = Y;
		isDragging = true;
	}
	bool IsDragging()
	{
		return isDragging;
	}
	/*void Drag(int X, int Y)
	{
		double pox = ScreenToWorldX(X), poy = ScreenToWorldY(Y);

		T -= poy-pry;
		B -= poy-pry;
		L -= pox-prx;
		R -= pox-prx;
		prx = pox;
		pry = poy;
	}*/
	void StopDragging()
	{
		isDragging = false;
	}
	void Drag( int X, int Y) {
		if (X <= (WorldToScreenX(R) - 5) && X >= (WorldToScreenX(L) + 5) && Y <= (WorldToScreenY(B) - 5) && Y >= (WorldToScreenY(T) + 5))
		{
			//double x1 = ScreenToWorldX(X), y1 = ScreenToWorldY(Y);
			R -= deltaXForDrag * (X - prx);
			L -= deltaXForDrag * (X - prx);
			T += deltaYForDrag * (Y - pry);
			B += deltaYForDrag * (Y - pry);
			/*if (x1 - prx > 0) {
				R -= deltaX;
				L -= deltaX;
			}
			else if (x1 - prx < 0) {
				R += deltaX;
				L += deltaX;
			}
			if (y1 - pry > 0) {
				T -= deltaY;
				B -= deltaX;
			}
			else if (y1 - pry < 0) {
				T += deltaY;
				B += deltaY;
			}*/
			prx = X;
			pry = Y;
		}
		else StopDragging();
	}
	void change() {
		tl = L;
		tr = R;
	}
	void change(double a, double b, double c, double d, double t1, double t2) {
		L = a;
		R = b;
		T = c;
		B = d;
		double R1 = R;
		R = (T - B) / 2 * W / H + (L + R) / 2;
		L = -(T - B) / 2 * W / H + (L + R1) / 2;
		tl = t1;
		tr = t2;
		pensolid = 5;
	}
	void point(double X, double Y, HDC dc) {
		
		SetPixel(dc, X, Y, RGB(0, 200, 100));
		SetPixel(dc, X+1, Y+1, RGB(0, 200, 100)); 
		SetPixel(dc, X, Y+1, RGB(0, 200, 100));
		SetPixel(dc, X+1, Y, RGB(0, 200, 100));
		double xs = ScreenToWorldX(X),
		ys = ScreenToWorldY(Y);
		std::string str = "("+std::to_string(xs)+" , "+ std::to_string(ys)+")";
		int n;
		if (xs > 0)
			if (ys > 0)
				n = 22;
			else
				n = 23;
		else
			if (ys > 0)
				n = 23;
			else
				n = 24;
		char *buf=new char[n];
		strcpy(buf, str.c_str());
		if (X + 140 < W) {
			if(Y - 10 > 0) 
				TextOut(dc, X, Y - 15, buf, n);
			else
				TextOut(dc, X, Y + 10, buf, n);
		}
		else {
			if (Y - 10 > 0 )
				TextOut(dc, X-140, Y - 15, buf, n);
			else
				TextOut(dc, X-140, Y + 10, buf, n);
		}
	}
};

#endif CAMERA_2D_H
