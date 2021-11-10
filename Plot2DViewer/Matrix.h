#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

using namespace std;


template <typename Cell = double>
class Matrix
{
private:
	//int size;
	int rows, cols;
	Cell** cells;
	void AllocateCells(int, int);
	void FreeCells();
public:
	Matrix() : rows(0), cols(0), cells(nullptr) {}	// ����������� �� ���������
	Matrix(const Matrix&);					// ����������� �����������
	Matrix(int, int);
	Matrix(int);// ����������� ������� �������
	Matrix(int, int, Cell*);
	Matrix(int, Cell*);	// ����������� ������� �� ������
	~Matrix();								// ����������

	Cell& operator()(int i, int j) { return cells[i - 1][j - 1]; }
	void IdentityMatrix() {
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++) {
				if (i == j)
					cells[i][j] = 1;
				else
					cells[i][j] = 0;
			}
	}
	void set(int i, int j, Cell x) { cells[i][j] = x; }
	void set(int j, Cell x, Cell y, Cell k) {
		cells[0][j] = x;
		cells[1][j] = y;
		cells[2][j] = k;
	}
	int getRows() { return rows; }
	int getCols() { return cols; }

	Matrix& operator = (const Matrix&);		// ���������� ��������� ������������
	Matrix  operator + (const Matrix&);		// �������� ������
	Matrix  operator - (const Matrix&);		// ��������� ������
	Matrix  operator * (const Matrix&);		// ��������� ������

	friend istream& operator >> <> (istream&, Matrix&);			// ���������� ��������� >> ��� ����� �������
	friend ostream& operator << <> (ostream&, const Matrix&);	// ���������� ��������� << ��� ������ �������
};


template <typename Cell>
Matrix<Cell>::Matrix(const Matrix<Cell>& M)
{
	AllocateCells(M.rows, M.cols);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			cells[i][j] = M.cells[i][j];
}

template <typename Cell>
Matrix<Cell>::Matrix(int Rows, int Cols)
{
	AllocateCells(Rows, Cols);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			cells[i][j] = 0;
}

template <typename Cell>
Matrix<Cell>::Matrix(int Rows)
{
	AllocateCells(Rows, Rows);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			cells[i][j] = 0;
}

template <typename Cell>
Matrix<Cell>::Matrix(int Rows, int Cols, Cell* list)
{
	AllocateCells(Rows, Cols);
	int k = 0;
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++) {
			cells[i][j] = list[k];
			k++;
		}
}

template <typename Cell>
Matrix<Cell>::Matrix(int Rows, Cell* list)
{
	AllocateCells(Rows, Rows);
	int k = 0;
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++) {
			cells[i][j] = list[k];
			k++;
		}
}

template <typename Cell>
Matrix<Cell>::~Matrix()
{
	FreeCells();
}

template <typename Cell>
Matrix<Cell>& Matrix<Cell>::operator=(const Matrix& M)
{
	if ((cols != M.cols) || (rows != M.rows))
	{
		FreeCells();
		AllocateCells(M.rows, M.cols);
	}
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			cells[i][j] = M.cells[i][j];
	return *this;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator+(const Matrix& M)
{
	Matrix<Cell> res(*this);
	if ((rows == M.rows) && (cols == M.cols))
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				res.cells[i][j] += M.cells[i][j];
	}
	return res;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator-(const Matrix& M)
{
	Matrix<Cell> res(*this);
	if ((rows == M.rows) && (cols == M.cols))
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				res.cells[i][j] -= M.cells[i][j];
	}
	return res;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator*(const Matrix& M)
{
	Matrix<Cell> res(rows, M.cols);
	if (cols == M.rows)
	{
		for (int i = 0; i < rows; i++) //��������� ��������� �� ������
			for (int j = 0; j < M.cols; j++) //��������� ��������� �� �������
				for (int k = 0; k < cols; k++) //��� ������������ ������ ��������� (������ �� �������)
					res.cells[i][j] += cells[i][k] * M.cells[k][j];
	}
	return res;
}

template <typename Cell>
void Matrix<Cell>::AllocateCells(int Rows, int Cols)
{
	cells = new Cell * [Rows];
	for (int i = 0; i < Rows; i++)
		cells[i] = new Cell[Cols];
	cols = Cols;
	rows = Rows;
}

template <typename Cell>
void Matrix<Cell>::FreeCells()
{
	for (int i = 0; i < rows; i++)
		delete[] cells[i];
	delete[] cells;
	rows = 0;
	cols = 0;
}

template <typename Cell>
istream& operator >> (istream& fi, Matrix<Cell>& M)
{
	for (int i = 0; i < M.rows; i++)
		for (int j = 0; j < M.cols; j++)
			fi >> M.cells[i][j];
	return fi;
}

template <typename Cell>
ostream& operator << (ostream& fo, const Matrix<Cell>& M)
{
	for (int i = 0; i < M.rows; i++)
	{
		fo << "  ";
		for (int j = 0; j < M.cols; j++)
			fo << M.cells[i][j] << " \t";
		fo << endl;
	}
	return fo;
}

#endif MATRIX_H