#pragma once
class Effects
{
public:
	/// <summary>
	/// イージング関数
	/// </summary>
	/// <param name="end_position">	移動距離</param>
	/// <param name="position">		最初の位置</param>
	/// <param name="time">			移動フレーム</param>
	/// <param name="now_time">		現在フレーム</param>
	/// <returns></returns>
	double Ease_In(double end_position, double position, double time, double now_time);
	double Ease_Out(double end_position, double position, double time, double now_time);
	double Ease_InOut(double end_position, double position, double time, double now_time);
};

