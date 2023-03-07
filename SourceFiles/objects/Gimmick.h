#pragma once
#include "Collider.h"
#include "Model.h"

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
	bool isLight;

public:
	void Initialize();
	void Update();
};