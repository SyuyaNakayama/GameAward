#pragma once
#include "Collider.h"

// —Ž‰º‚àŠÜ‚Þ
class Jump : public RayCollider
{
private:
	float jumpSpd = 0;
	float gravity = 0;
	float endHeight = 0;
	bool isJump = false;

public:
	void SetGravity(float gravity_) { gravity = gravity_; }
	void Start(float jumpV0, float endHeight);
	void Update();
	const Vector3 GetRayDirection() { return { 0,-1,0 }; }
	void OnCollision(BoxCollider* collider);
};