#pragma once
#include "WorldTransform.h"

class Camera
{
private:
	WorldTransform worldTransform;
	ViewProjection viewProjection;
	float angleTarget = 0;

public:
	void Initialize();
	void Update();
	void SetParent(WorldTransform* parent) { worldTransform.parent = parent; }
	ViewProjection* GetViewProjection() { return &viewProjection; }
	float GetAngleTarget() { return angleTarget; }
};

