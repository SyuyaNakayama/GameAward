#pragma once
#include <Input.h>
#include "Model.h"
#include "LightGroup.h"
#include "Camera.h"

class Player
{
private:
	enum class PartId { body, legR, legL };

	Input* input_;
	WorldTransform worldTransform_;
	bool isLight = true;
	std::array<WorldTransform, 3> modelsTrans_;
	std::array<std::unique_ptr<Model>, 3> model_;
	Camera eyeCamera;
	bool isCameraChange = false;
	LightGroup* lightGroup_;

	void Move();
	void ChangeLight();
public:
	void Initialize();

	void Update();
	void Draw();
	WorldTransform GetWorldTransform() { return worldTransform_; }
	bool IsCameraChange() { return isCameraChange; }
};

