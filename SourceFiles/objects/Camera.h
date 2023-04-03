#pragma once
#include "WorldTransform.h"
#include "Quaternion.h"

// �I�u�W�F�N�g�ɒǏ]����J����
class Camera
{
private:
	WorldTransform worldTransform;
	ViewProjection viewProjection;
	Vector2 angle; // �J������]��

public:
	void Initialize(WorldTransform* parent);
	void Update();
	ViewProjection* GetViewProjection() { return &viewProjection; }
	const Vector2& GetAngle() { return angle; }
};

