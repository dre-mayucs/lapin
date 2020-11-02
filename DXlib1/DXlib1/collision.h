#pragma once
#include <cmath>
class collision
{
private:
	double a, b, sum;
	bool __collision_sum;

public:
	double Deg_to_Rad_X(float angle);
	double Deg_to_Rad_Y(float angle);
	double Deg_to_Rad_X(float angle, float adjust);
	double Deg_to_Rad_Y(float angle, float adjust);
	bool Trigonometric_Fanc(double x1, double y1, double r1, double x2, double y2, double r2);
	bool box_Fanc(double ax1, double ax2, double ay1, double ay2, double bx1, double bx2, double by1, double by2);
};

