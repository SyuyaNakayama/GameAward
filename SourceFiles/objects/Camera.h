#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

// �I�u�W�F�N�g�ɒǏ]����J����
class Camera
{
private:
	WorldTransform worldTransform;
	ViewProjection viewProjection;
	Vector2 angle = {0,0.3f}; // �J������]��

public:
	void Initialize(WorldTransform* parent);
	void Update();
	ViewProjection* GetViewProjection() { return &viewProjection; }
	const Vector2& GetAngle() { return angle; }
};

