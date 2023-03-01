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
	bool useLight;
	std::unique_ptr<Model> model_;
	std::unique_ptr<Sprite> sprite_;
	Camera eyeCamera;

	LightGroup* lightGroup_;

	void Move();
	void ChangeLight();
public:
	void Initialize();
	void Update();
	void Draw();
	WorldTransform GetWorldTransform() { return worldTransform_; }
	~Player() { sprite_.release(); }
};

