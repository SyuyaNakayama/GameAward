#pragma once
#include "Collider.h"
#include "Model.h"
#include "ParticleManager.h"
#include "Input.h"
#include "Collider.h"


class Gimmick : public BoxCollider
{
protected:
	std::unique_ptr<Model> model;
	static bool isStart_;	// trueになったらカメラを引く
	static bool isGoal_;		// trueになったらゴールできる
public:
	virtual ~Gimmick() { model.release(); }
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() { model->Draw(worldTransform); }
	Vector3 GetPosition() { return worldTransform.GetWorldPosition(); }
	void SetPosition(Vector3 pos) { worldTransform.translation = pos; }

	static bool GetIsStart() { return isStart_; }
	static void SetIsStart(bool isStart) { isStart_ = isStart; }
	Vector3 GetScale() { return worldTransform.scale; }
	void SetScale(Vector3 scale) { worldTransform.scale = scale; }

	static bool GetIsGoal() { return isGoal_; }
	static void SetIsGoal(bool isStart) { isGoal_ = isStart; }
};

class Door : public Gimmick
{
private:
	bool isOpen = false;	//扉が開くフラグ
	bool isOpened = false;	//扉が空いてるかどうか
	bool isClose = false;	//扉が閉まるフラグ
	
	std::unique_ptr<Model> model_back;
	WorldTransform doorL;
	WorldTransform doorR;
	Input* input;
	float rot = 90;

	void OnCollision(BoxCollider* boxCollider);
	void Open();
	void Close();
public:
	void Initialize();
	void Update();
	void Draw() override;

	Vector3 GetRadius() { return { 1.8f,1.9f,1.0f }; }
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

class Wall : public Gimmick
{
private:

public:
	Wall(Vector3 scale);
	void Initialize();
	void Update();
};