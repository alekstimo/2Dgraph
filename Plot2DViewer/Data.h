#define _USE_MATH_DEFINES
#include <cmath>
#include <wingdi.h>

double					// Исходные значения параметров L, R, B, T
	L=-M_PI,
	R=M_PI,
	B=-1.5,
	T=1.5,
	tl = 0,
	tr =0; 

void Parabola(double &x,double &y, double t)
{
	x = t;
	y= x*x;
}

void Sinusoid(double &x, double &y,double t)
{
	
	x = t;
	y = sin(x);
}


void Butterfly(double& x, double& y, double t) {
	x = sin(t) * (exp(cos(t)) - 2 * cos(4 * t) + pow(sin(t / 12), 5));
	y = cos(t) * (exp(cos(t)) - 2 * cos(4 * t) + pow(sin(t / 12), 5));
}
void Star(double& x, double& y, double t) {
	x = 2 * pow(sin(t), 3);
	y = 2 * pow(cos(t), 3);
}

void Ellipse(double& x, double& y, double t) {
	x = sin(t);
	y = cos(t);
}

void Spiral(double& x, double& y, double t) {
	x = t * sin(t);
	y = t * cos(t);
}
void Kardiola(double& x, double& y, double t) {
	x = (1+cos(t))*cos(t);
	y = (1 + cos(t)) * sin(t);
}
void Lassaghu(double& x, double& y, double t) {
	x = sin(5 * t + M_PI / 2);
	y = sin(6 * t);
}

