#ifndef SCENE_2D_H
#define SCENE_2D_H

#include "Camera2D.h"
#include "Model2D.h"
class Scene2D : public Camera2D
{
private:
	
	typedef void (*Func)(double &,double &,double);
public:
	Model2D Model;
	Scene2D(double L, double R, double B, double T, Matrix<double> V, Matrix<int> E) : Camera2D(L, R, B, T),Model(V,E) {
		/*Model = new Model2D[2];
		Model[1](V, E);*/
	}
	void Plot(HDC dc, Func f, bool z, bool axes = true)
	{
		HPEN pen = (HPEN)SelectObject(dc, CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
		if (axes)
			Axes(dc);
		HPEN hPen2 = CreatePen(PS_SOLID, pensolid, RGB(200, 0, 100));
		SelectObject(dc, hPen2);
		pix = (R - L) / W;
		if (z)
			change();
		double s = tl, x, y;
		f(x, y, s);
		MoveTo(x, y);
		while (s <= tr) {
			s += pix;
			f(x, y, s);
			LineTo(dc, x, y);
		}
	}
		// Построение графика функции f
		// Здесь передаваемая в качестве параметра функция f вызывается как обычная функция:
		// f(x);
		void plot(Func f, bool z)
		{
			
			pix = (R - L) / W;
			if (z)
				change();
			double s = tl, x, y;
			int size = (tr - tl) / pix;
			Matrix<> Vert(3, size);
			Matrix<int> Edg(size, 2);
			int e = 1;
			f(x, y, s);
			Vert.set(e - 1, x, y, 1);
				while (e < size) {
					s += pix;
					e++;
					f(x, y, s);
					Vert.set(e - 1, x, y, 1);
					Edg.set(e - 2, 0, e - 1);
					Edg.set(e - 2, 1, e);
				}
				Model.setVertices(Vert);
				Model.setEdges(Edg);
		}
	
		void Render(HDC dc) {
			HPEN hPen2 = CreatePen(PS_SOLID, pensolid, RGB(0, 200, 100));
			SelectObject(dc, hPen2);
			double x, y;
			int Edge = 1;
			Matrix<int> Edges = Model.GetEdges();
			int row = Edges.getRows();
			x = Model.GetVertexX(Edge);
			y = Model.GetVertexY(Edge);
			MoveTo(x, y);
			for (int i = 1; i <= row; i++) {
				if (Edges(i, 1) == Edge) {
					Edge = Edges(i, 2);
					x = Model.GetVertexX(Edge);
					y = Model.GetVertexY(Edge);
					LineTo(dc, x, y);
				}
				else {
					Edge = Edges(i, 1);
					x = Model.GetVertexX(Edge);
					y = Model.GetVertexY(Edge);
					MoveTo(x, y);
					Edge = Edges(i, 2);
					x = Model.GetVertexX(Edge);
					y = Model.GetVertexY(Edge);
					LineTo(dc, x, y);
				}
			}
		}
		
		
		
	
};

#endif SCENE_2D_H


