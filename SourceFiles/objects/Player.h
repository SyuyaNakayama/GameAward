#pragma once
#include <Input.h>
#include <Model.h>
#include "LightGroup.h"
#include "Camera.h"
#include "Collider.h"

class Player : public BoxCollider
{
private:
	Input* input_;
	bool useLight;
	std::unique_ptr<Model> model_;
	std::unique_ptr<Sprite> sprite_;
	Camera eyeCamera;
	bool isCameraChange = false;
	LightGroup* lightGroup_;

	void Move();
	void ChangeLight();
public:
	void Initialize();
	void Update();
	void Draw();

	WorldTransform GetWorldTransform() { return worldTransform; }
	bool IsCameraChange() { return isCameraChange; }
	~Player() { sprite_.release(); }

	void OnCollision(BoxCollider* boxCollider) override;
	//void OnCollision(SphereCollider* sphereCollider) override;
	//void OnCollision(PlaneCollider* boxCollider) override;
	//void OnCollision(PolygonCollider* sphereCollider) override;
	//void OnCollision(RayCollider* sphereCollider) override;
	//void OnCollision(IncludeCollider* sphereCollider) override;
};

