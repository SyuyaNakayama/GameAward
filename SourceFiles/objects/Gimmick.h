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
	static bool isGoal_;	// trueになったらゴールできる
	static LightGroup* lightGroup;

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
	enum class DoorState
	{
		Open, // 開く
		Opened, // 開いている
		Close // 閉じる
	};

	DoorState doorState = DoorState::Close;

	bool isOpen = false;	//扉が開くフラグ
	bool isOpened = false;	//扉が空いてるかどうか
	bool isClose = false;	//扉が閉まるフラグ

	UINT16 doorIndex = 0;
	std::unique_ptr<Model> model_back;
	WorldTransform doorL;
	WorldTransform doorR;
	Input* input = Input::GetInstance();
	float rot = 0;

	void OnCollision(BoxCollider* boxCollider);
	void Open();
	void Close();
public:
	Door(UINT16 doorIndex_) { doorIndex = doorIndex_; }
	void Initialize();
	void Update();
	void Draw() override;

	//当たり判定の大きさを調整
	Vector3 GetRadius() { return { 1.8f,1.9f,1.0f }; }
};

class Candle : public Gimmick, public SphereCollider
{
private:
	bool isLight = false;
	size_t lightIndex = 0;
	DiffuseParticle::AddProp particleProp;
	Vector3 lightPos;
	static size_t lightNum;
	int particleTimer = 60; // 方向パーティクル発生時間
	Vector3 playerPos;

public:
	Candle(size_t index) { lightIndex = index; lightNum++; }
	void OnCollision(RayCollider* rayCollider);
	void Initialize();
	void Update();
	void Draw() override;
	static size_t GetLightNum() { return lightNum; }
	static void ResetLightNum() { lightNum = 0; }
};

class Wall : public Gimmick
{
private:

public:
	Wall(Vector3 scale);
	void Initialize();
	void Update();
};