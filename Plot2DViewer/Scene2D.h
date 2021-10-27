#ifndef SCENE_2D_H
#define SCENE_2D_H

#include "Camera2D.h"

class Scene2D : public Camera2D
{
private:
	typedef void (*Func)(double &,double &,double);
public:
	Scene2D(double L, double R, double B, double T) : Camera2D(L, R, B, T)
	{
	}
	void Plot(HDC dc, Func f, bool z, bool axes = true)
	{
		HPEN pen = (HPEN)SelectObject(dc, CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
		if (axes)
			Axes(dc);
		HPEN hPen2 = CreatePen(PS_SOLID, 5, RGB(200, 0, 100));
		SelectObject(dc, hPen2);
		pix = (R - L) / W;
		if(z)
			change(); 
		double s = tl, x, y;
		f(x,y,s);
		MoveTo(x, y);
		while (s <= tr) {
			s += pix;
			f(x, y, s);
			LineTo(dc, x, y);
		}

		/*pix = (R - L) / W;
		double s =L;
		double fx = f(s);
		MoveTo(s, fx);
		while (s <= R) {
			s += pix;
			fx =  f(s);
			LineTo(dc, s, fx);
		}*/
		// Построение графика функции f
		// Здесь передаваемая в качестве параметра функция f вызывается как обычная функция:
		// f(x);
	}
};

#endif SCENE_2D_H


