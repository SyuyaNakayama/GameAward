#pragma once
#include <Input.h>
#include <Model.h>
#include "LightGroup.h"

class Player
{
private:
	Input* input_;
	std::unique_ptr<Model> model_;
	std::unique_ptr<Sprite> sprite_;
	WorldTransform worldTransform_;

	LightGroup* lightGroup_;
	bool useLight_;

	void Move();
	void ChangeLight();
public:
	void Initialize();
	void Update();
	void Draw();
	WorldTransform GetWorldTransform() { return worldTransform_; }
	~Player() { sprite_.release(); }
};

