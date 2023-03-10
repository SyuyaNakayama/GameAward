#pragma once
#include "Input.h"
#include "Model.h"
#include "LightGroup.h"
#include "Camera.h"
#include "Collider.h"

class Player : public BoxCollider, public RayCollider
{
private:
	enum class PartId { body, legR, legL };

	Input* input_;
	bool isLight = true;
	std::array<WorldTransform, 3> modelsTrans_;
	std::array<std::unique_ptr<Model>, 3> model_;
	Camera eyeCamera;
	bool isCameraChange = false;
	LightGroup* lightGroup_;

	/// <param name="spd">à⁄ìÆë¨ìxÇÃåWêî</param>
	void Move(float spd);
	void ChangeLight();
public:
	void Initialize();
	void Update();
	void Draw();

	WorldTransform GetWorldTransform() { return worldTransform; }
	bool IsCameraChange() { return isCameraChange; }

	void OnCollision(BoxCollider* boxCollider) override;
	//void OnCollision(SphereCollider* sphereCollider) override;
	//void OnCollision(PlaneCollider* boxCollider) override;
	//void OnCollision(PolygonCollider* sphereCollider) override;
	//void OnCollision(RayCollider* sphereCollider) override;
	//void OnCollision(IncludeCollider* sphereCollider) override;
};

