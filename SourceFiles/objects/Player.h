#pragma once
#include <Input.h>
#include <Model.h>
#include "LightGroup.h"
#include "Camera.h"

class Player
{
private:
	Input* input_;
	WorldTransform worldTransform_;
	bool isLight = true;
	std::unique_ptr<Model> model_;
	std::unique_ptr<Sprite> sprite_;
	Camera eyeCamera;
	bool isCameraChange = false;
	LightGroup* lightGroup_;

	void Move();
	void ChangeLight();
public:
	void Initialize(LightGroup* lightGroup);
	void Update();
	void Draw();
	WorldTransform GetWorldTransform() { return worldTransform_; }
	bool IsCameraChange() { return isCameraChange; }
	~Player() { sprite_.release(); }
};

