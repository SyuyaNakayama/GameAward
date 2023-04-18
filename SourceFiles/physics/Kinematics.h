#pragma once
// 運動学

// 運動の法則
struct Motion
{
	float px = 0; // 位置
	float vx = 0; // 速度
	float ax = 0; // 加速度
	float fx = 0; // 物体に働く力
	float m = 1;  // 質量

	void Initialize(float px, float vx, float ax = 0, float fx = 0, float m = 1);
	void Update();
};