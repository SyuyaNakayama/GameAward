#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

// オブジェクトに追従するカメラ
class Camera
{
private:
	WorldTransform worldTransform;
	ViewProjection viewProjection;
	Vector2 angle = {0,0.3f}; // カメラ回転量

public:
	void Initialize(WorldTransform* parent);
	void Update();
	ViewProjection* GetViewProjection() { return &viewProjection; }
	const Vector2& GetAngle() { return angle; }
};

