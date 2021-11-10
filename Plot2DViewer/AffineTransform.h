#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H

Matrix<> Translation(double x, double y)
{
	Matrix<> T(3);
	T.IdentityMatrix();
	T.set(0, 2, x);
	T.set(1, 2, y);
	return T;
}

// Здесь требуется реализовать функции, возвращающие матрицы базовых АП:
Matrix<> Identity() {// тождественное АП;
	Matrix<> T(3);
	T.IdentityMatrix();
	return T;
}
Matrix<> Rotation(double t) { //поворот на угол t;

	Matrix<> T(3);
	T.IdentityMatrix();
	T.set(0, 0, cos(t));
	T.set(0, 1, -sin(t));
	T.set(1, 0, sin(t));
	T.set(1, 1, cos(t));
	return T;
}
// Rotation(c, s) - поворот на угол, косинус и синус которого пропорциональны величинам c и s;
Matrix<> Scaling(double kx, double ky) {  // масштабирование;
	Matrix<> T(3);
	T.IdentityMatrix();
	T.set(0, 0, kx);
	T.set(1, 1, ky);
	return T;
}
// Mapping (различные виды отражений) - по желанию, для создания матриц отражения можно использовать функцию Scaling.

// В приведённом примере используется конструктор матрицы из списка.
// Приветствуются идеи о других способах создания матриц.
// Например, вызов функции, создающей единичную матрицу,
// с последующим переопределением некоторых её коэффициентов.

#endif AFFINE_TRANSFORM_H

