#include "Effects.h"
double Effects::Ease_In(double end_position, double position, double time, double now_time)
{
	now_time /= time;
	return end_position * now_time * now_time + position;
}

double Effects::Ease_Out(double end_position, double position, double time, double now_time)
{
	now_time /= time;
	return -(end_position)*now_time * (now_time - 2) + position;
}

double Effects::Ease_InOut(double end_position, double position, double time, double now_time)
{
	now_time /= time / 2;
	if (now_time < 1) {
		return end_position / 2 * now_time * now_time + position;
	}
	return -(end_position) / 2 * ((--now_time) * (now_time - 2) - 1) + position;
}
