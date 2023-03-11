#pragma once
#include "Timer.h"
#include <list>
#include "Vector.h"

// 拡散するパーティクル(今までのやつ)
class DiffuseParticle
{
private:
	// パーティクル1粒
	struct Particle
	{
		Vector3 position{}; // 座標
		Vector3 velocity{}; // 速度
		Vector3 accel{}; // 加速度
		Timer frame = 0;
		float scale = 1.0f; // スケール
		float s_scale = 1.0f; // 初期値
		float e_scale = 0.0f;	// 最終値

		void Update();
	};

	std::list<Particle> particles;

public:
	struct AddProp
	{
		Vector3 posOffset;
		Vector3 velOffset;
		Vector3 accOffset;
		float posRange = 1.0f;
		float velRange = 0.1f;
		float accRange = 0.001f;
		int lifeTime = 60;
		float start_scale = 1.0f;
		float end_scale = 0.0f;
		unsigned short addNum = 1;
	};

	void Initialize() {  }
	void Add(const AddProp& particleProp);
	void Update();
	void Draw();
};