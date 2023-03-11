#pragma once
#include "Timer.h"
#include <list>
#include "Model.h"

struct AddDirectionalParticleProp
{
	Vector3 start;
	Vector3 end;
	UINT16 splitNum = 1; // ����_�̐�
	float angle = 0; // {0,1,0}��0���W�A��
	float radius;
	int lifeTime = 60;
};

// �n�_����I�_�֌������p�[�e�B�N��
class DirectionalParticle
{
private:
	// �p�[�e�B�N��1��
	struct Particle
	{
		Vector3 start; // �n�_
		Vector3 end; // �I�_
		WorldTransform worldTransform; // ���W
		UINT16 splitNum;
		float angle;
		float radius;
		Timer frame;
		std::vector<Vector3> controlPoints; // ����_

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