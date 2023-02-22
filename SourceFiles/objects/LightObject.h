#pragma once
#include "Collider.h"
#include "LightGroup.h"
#include "Model.h"

class BaseLight : public RayCollider
{
protected:
	LightGroup* lightGroup = nullptr;
	std::unique_ptr<Model> model;

public:
	virtual ~BaseLight() = default;
	virtual void Initialize(LightGroup* lightGroup_) = 0;
	virtual void Update() = 0;
	virtual void Draw() { model->Draw(worldTransform); }
	Vector3 GetLightDir(){ return Normalize(Vector3::MakeZAxis()* Matrix4::Rotate(worldTransform.rotation)); }
};

class NormalLight : public BaseLight
{
public:
	void Initialize(LightGroup* lightGroup_);
	void Update();
};

class BlackLight : public BaseLight
{
public:
	void Initialize(LightGroup* lightGroup_);
	void Update();
};