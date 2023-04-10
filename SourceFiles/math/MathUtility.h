#pragma once

const float PI = 3.141592f;

class Angle
{
	float angle;

	operator float() { return angle; }
	void operator=(float rad) { angle = rad; } // ラジアン代入
	void operator=(int deg) { angle = (float)deg * PI / 180.0f; } // 度数代入
};

enum class Axis { X, Y, Z };