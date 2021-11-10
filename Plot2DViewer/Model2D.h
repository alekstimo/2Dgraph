
#ifndef MODEL_2D_H
#define MODEL_2D_H
#include <string>
#include <fstream>
#include "Matrix.h"
//#include "AffineTransform.h"
class Model2D
{
private:
	Matrix<> Vertices;
	Matrix<int> Edges;
	Matrix<> CumulativeAT;
	Matrix<> InitialVertices;
public:
	Model2D() : Vertices(), Edges(), CumulativeAT(), InitialVertices() {}
	Model2D(string, string);
	Model2D(const Matrix<> Vertices, const Matrix<int> Edges) :
		Vertices(Vertices), Edges(Edges), CumulativeAT(3), InitialVertices(Vertices) {
		CumulativeAT.IdentityMatrix();
	}
	Matrix<> GetVertices() { return Vertices; }
	Matrix<int> GetEdges() { return Edges; }
	void setVertices(Matrix<> Vert) { Vertices = Vert; InitialVertices = Vert; CumulativeAT.IdentityMatrix(); }
	void setEdges(Matrix<int> Edg) { Edges = Edg;}
	//int GetEdge(int);
	double GetVertexX(int);
	double GetVertexY(int);
	void Apply(Matrix<>);
};
#endif MODEL_2D_H

