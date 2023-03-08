#pragma once
#include "Collider.h"
#include "Model.h"
#include "ParticleManager.h"

class Gimmick : public SphereCollider
{
protected:
	std::unique_ptr<Model> model;

public:
	virtual ~Gimmick() { model.release(); }
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() { model->Draw(worldTransform); }
	Vector3 GetPosition() { return worldTransform.GetWorldPosition(); }
	void SetPosition(Vector3 pos) { worldTransform.translation = pos; }
};

class Door : public Gimmick
{
private:
	bool isOpen = false;
	WorldTransform flip;

public:
	void Initialize();
	void Update();
	void Draw() override;
};

class Candle : public Gimmick
{
private:
	bool isLight = true;
	size_t lightIndex = 0;
	LightGroup* lightGroup = nullptr;
	AddParticleProp particleProp;
	Vector3 lightPos;

public:
	Candle(size_t index) { lightIndex = index; }
	void Initialize();
	void Update();
	void Draw() override;
};