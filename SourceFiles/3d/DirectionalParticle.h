#pragma once
#include "Timer.h"
#include <list>
#include "Model.h"

struct AddDirectionalParticleProp
{
	Vector3 start;
	Vector3 end;
	UINT16 splitNum = 1; // 制御点の数
	float angle = 0; // {0,1,0}が0ラジアン
	float radius;
	int lifeTime = 60;
};

// 始点から終点へ向かうパーティクル
class DirectionalParticle
{
private:
	// パーティクル1粒
	struct Particle
	{
		Vector3 start; // 始点
		Vector3 end; // 終点
		WorldTransform worldTransform; // 座標
		UINT16 splitNum;
		float angle;
		float radius;
		Timer frame;
		std::vector<Vector3> controlPoints; // 制御点

		void ComputeControlPoints();
	};

	std::list<Particle> particles;
	std::unique_ptr<Model> model;
public:
	void Initialize() { model = Model::Create("cube"); }
	void Add(const AddDirectionalParticleProp& particleProp);
	void Update();
	void Draw();
};