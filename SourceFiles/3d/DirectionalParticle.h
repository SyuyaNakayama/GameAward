#pragma once
#include "Timer.h"
#include <list>
#include "Model.h"

// 始点から終点へ向かうパーティクル
class DirectionalParticle
{
private:
	// パーティクル1粒
	struct Particle
	{
		Vector3 start; // 始点
		Vector3 end; // 終点
		WorldTransform worldTransform;
		UINT16 splitNum = 1;
		float angle = 0;
		float radius = 0;
		Timer frame;
		std::vector<Vector3> controlPoints; // 制御点

		void ComputeControlPoints();
		void Update();
	};

	std::list<Particle> particles;
	std::unique_ptr<Model> model;

public:
	struct AddProp
	{
		Vector3 start;
		Vector3 end;
		UINT16 splitNum = 1; // 制御点の数
		float angle = 0; // {0,1,0}が0ラジアン
		float radius = 0;
		int lifeTime = 60;
	};

	void Initialize() { model = Model::Create("cube"); }
	void Add(const AddProp& particleProp);
	void Update();
	void Draw();
};