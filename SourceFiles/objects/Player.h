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
	WorldTransform modelsTrans_[4];
	bool useLight;
	std::unique_ptr<Model> model_[4];
	std::unique_ptr<Sprite> sprite_;
	Camera eyeCamera;
	bool isCameraChange = false;
	LightGroup* lightGroup_;

	void Move();
	void ChangeLight();
public:
	enum class PartId {
		root,
		body,
		legR,
		legL
	};

	void Initialize();
	void Update();
	void Draw();
	WorldTransform GetWorldTransform() { return worldTransform_; }
	bool IsCameraChange() { return isCameraChange; }
	~Player() { sprite_.release(); }
};

