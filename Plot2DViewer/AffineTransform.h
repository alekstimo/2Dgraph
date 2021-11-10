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

// ����� ��������� ����������� �������, ������������ ������� ������� ��:
Matrix<> Identity() {// ������������� ��;
	Matrix<> T(3);
	T.IdentityMatrix();
	return T;
}
Matrix<> Rotation(double t) { //������� �� ���� t;

	Matrix<> T(3);
	T.IdentityMatrix();
	T.set(0, 0, cos(t));
	T.set(0, 1, -sin(t));
	T.set(1, 0, sin(t));
	T.set(1, 1, cos(t));
	return T;
}
// Rotation(c, s) - ������� �� ����, ������� � ����� �������� ��������������� ��������� c � s;
Matrix<> Scaling(double kx, double ky) {  // ���������������;
	Matrix<> T(3);
	T.IdentityMatrix();
	T.set(0, 0, kx);
	T.set(1, 1, ky);
	return T;
}
// Mapping (��������� ���� ���������) - �� �������, ��� �������� ������ ��������� ����� ������������ ������� Scaling.

// � ���������� ������� ������������ ����������� ������� �� ������.
// �������������� ���� � ������ �������� �������� ������.
// ��������, ����� �������, ��������� ��������� �������,
// � ����������� ���������������� ��������� � �������������.

#endif AFFINE_TRANSFORM_H

