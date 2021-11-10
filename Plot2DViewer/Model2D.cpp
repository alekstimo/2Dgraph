#include "Model2D.h"

Model2D::Model2D(string nameFVertices, string nameFEdges)
{
	ifstream fileVertices(nameFVertices);
	if (!fileVertices.is_open())
		cout << "Ошибка открытия файла" << endl;
	else {
		int row; fileVertices >> row;
		int col; fileVertices >> col;
		Vertices = Matrix<>(row, col);
		fileVertices >> Vertices;
		InitialVertices = Vertices;
	}
	ifstream fileEdges(nameFEdges);
	if (!fileEdges.is_open())
		cout << "Ошибка открытия файла" << endl;
	else {
		int row; fileEdges >> row;
		int col; fileEdges >> col;
		Edges = Matrix<int>(row, col);
		fileEdges >> Edges;
	}
	CumulativeAT = Matrix<>(3);
	CumulativeAT.IdentityMatrix();
}



double Model2D::GetVertexX(int num)
{
	return Vertices(1, num);
}

double Model2D::GetVertexY(int num)
{
	return Vertices(2, num);
}

void Model2D::Apply(Matrix<> T)
{
	CumulativeAT = T * CumulativeAT;
	Vertices = CumulativeAT * InitialVertices;
}
