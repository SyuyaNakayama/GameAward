#pragma once

const float PI = 3.141592f;

class Angle
{
	float angle;

	operator float() { return angle; }
	void operator=(float rad) { angle = rad; } // ƒ‰ƒWƒAƒ“‘ã“ü
	void operator=(int deg) { angle = (float)deg * PI / 180.0f; } // “x”‘ã“ü
};

enum class Axis { X, Y, Z };