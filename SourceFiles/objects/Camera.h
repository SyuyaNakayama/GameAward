#pragma once
#include "WorldTransform.h"
#include "Quaternion.h"

// �I�u�W�F�N�g�ɒǏ]����J����
class Camera
{
private:
	WorldTransform worldTransform;
	ViewProjection viewProjection;
	float angleTarget = 0;
	Quaternion rotQ = Quaternion::Identity();

public:
	void Initialize(WorldTransform* parent);
	void Update();
	ViewProjection* GetViewProjection() { return &viewProjection; }
	float GetAngleTarget() { return angleTarget; }
};

