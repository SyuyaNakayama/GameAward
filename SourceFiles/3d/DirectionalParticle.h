#pragma once
#include "Timer.h"
#include <list>
#include "Model.h"

// �n�_����I�_�֌������p�[�e�B�N��
class DirectionalParticle
{
private:
	// �p�[�e�B�N��1��
	struct Particle
	{
		Vector3 start; // �n�_
		Vector3 end; // �I�_
		WorldTransform worldTransform;
		UINT16 splitNum = 1;
		float angle = 0;
		float radius = 0;
		Timer frame;
		std::vector<Vector3> controlPoints; // ����_

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
		UINT16 splitNum = 1; // ����_�̐�
		float angle = 0; // {0,1,0}��0���W�A��
		float radius = 0;
		int lifeTime = 60;
	};

	void Initialize() { model = Model::Create("cube"); }
	void Add(const AddProp& particleProp);
	void Update();
	void Draw();
};