#include "collision.h"
#define PI 3.14159265359

double collision::Deg_to_Rad_X(float angle)
{
	return cos(PI / 180 * angle);
}

double collision::Deg_to_Rad_Y(float angle)
{
	return sin(PI / 180 * angle);
}

double collision::Deg_to_Rad_X(float angle, float adjust)
{
	return cos(PI / 180 * angle + adjust);
}

double collision::Deg_to_Rad_Y(float angle, float adjust)
{
	return sin(PI / 180 * angle + adjust);
}

bool collision::Trigonometric_Fanc(double x1, double y1, double r1, double x2, double y2, double r2)
{
	a = x2 - x1;
	b = y2 - y1;
	sum = sqrt((a * a) + (b * b));
	if (sum <= r1 + r2) { __collision_sum = true; }
	else { __collision_sum = false; }

	return __collision_sum;
}

bool collision::box_Fanc(double ax1, double ax2, double ay1, double ay2, double bx1, double bx2, double by1, double by2)
{
	if (
		bx1 < ax2 &&
		ax1 < bx2 &&
		by1 < ay2 &&
		ay1 < by2
		)
	{
		__collision_sum = true;
	}
	else { __collision_sum = false; }

	return __collision_sum;
}