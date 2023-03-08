#pragma once
#include <Input.h>
#include "Model.h"
#include "LightGroup.h"
#include "Camera.h"

class Player
{
private:
	Input* input_;
	WorldTransform worldTransform_;
	bool isLight = true;
	WorldTransform modelsTrans_[4];
	std::unique_ptr<Model> model_[4];
	Camera eyeCamera;
	bool isCameraChange = false;
	LightGroup* lightGroup_;

	void Move();
	void ChangeLight();
public:
	void Initialize(LightGroup* lightGroup);

	enum class PartId {
		root,
		body,
		legR,
		legL
	};
	void Update();
	void Draw();
	WorldTransform GetWorldTransform() { return worldTransform_; }
	bool IsCameraChange() { return isCameraChange; }
};

