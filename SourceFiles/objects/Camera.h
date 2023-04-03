#pragma once
#include "WorldTransform.h"
#include "Quaternion.h"

// �I�u�W�F�N�g�ɒǏ]����J����
class Camera
{
private:
	WorldTransform worldTransform;
	ViewProjection viewProjection;
	Matrix4 rotMat;
	Vector2 angle; // �J������]��

public:
	void Initialize(WorldTransform* parent);
	void Update();
	ViewProjection* GetViewProjection() { return &viewProjection; }
	const Matrix4& GetRotMatrix() { return rotMat; }
	const Vector2& GetAngle() { return angle; }
};

