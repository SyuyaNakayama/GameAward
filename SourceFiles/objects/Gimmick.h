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

	static bool GetIsGoal() { return isGoal_; }
	static void SetIsGoal(bool isGoal) { isGoal_ = isGoal; }
};

class Door : public Gimmick
{
private:
	enum class WTType { L, R };

	// ドアが閉じている時にnullptrになる
	void (Door::* Move)() = &Door::Opened;

	UINT16 doorIndex = 0;
	std::unique_ptr<Model> model_back;
	std::array<WorldTransform, 2> door;
	Input* input = Input::GetInstance();
	float rot = 90;

	void Open();	// ドアが開く時に呼び出される関数
	void Close();	// ドアが閉じる時に呼び出される関数
	void Opened();	// ドアが開いている時に呼び出される関数
	void Closed();	// ドアが閉じている時に呼び出される関数
	void OnCollision(BoxCollider* boxCollider);
public:
	Door(UINT16 doorIndex_) { doorIndex = doorIndex_; }
	void Initialize();
	void Update();
	void Draw();

	//当たり判定の大きさを調整
	Vector3 GetRadius() { return { 1.8f,1.9f,1.0f }; }
};

class Candle : public Gimmick, public SphereCollider
{
private:
	void (Candle::* Fire)() = &Candle::Dark;

	bool isLight = false;
	size_t lightIndex = 0;
	DiffuseParticle::AddProp particleProp;
	Vector3 lightPos;
	static size_t lightNum;
	int particleTimer = 60; // 方向パーティクル発生時間
	Vector3 playerPos;

	void Dark(); // 光っていない時の処理
	void PreLight(); // 光る前
	void PostLight(); // 光った後
public:
	Candle(size_t index) { lightIndex = index; lightNum++; }
	void OnCollision(RayCollider* rayCollider);
	void Initialize();
	void Update();
	static size_t GetLightNum() { return lightNum; }
	static void ResetLightNum() { lightNum = 0; }
};

class Wall : public Gimmick
{
private:

public:
	Wall(Vector3 scale) { worldTransform.scale = scale; }
	void Initialize();
	void Update();
};