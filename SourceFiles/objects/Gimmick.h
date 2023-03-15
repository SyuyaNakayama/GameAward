#pragma once
#include "Collider.h"
#include "Model.h"
#include "ParticleManager.h"
#include "Input.h"
#include "Collider.h"

// ギミックのパラメータ
struct GimmickParam {
	Vector3 pos;		// 座標
	Vector3 scale;	// スケール
	Vector3 rot;		// 回転
	bool flag = false;	// フラグ
};

class Gimmick : public BoxCollider
{
protected:
	std::unique_ptr<Model> model;
	static bool isStart_;	// trueになったらカメラを引く
	static bool isGoal_;		// trueになったらゴールできる
	static LightGroup* lightGroup;
	GimmickParam gimmickParam;

public:
	virtual ~Gimmick() { model.release(); }
	virtual void Initialize() = 0;
	virtual void Update(bool isLight) = 0;
	virtual void Draw() { model->Draw(worldTransform); }

	// アクセッサ
	Vector3 GetPosition() { return worldTransform.GetWorldPosition(); }
	void SetPosition(Vector3 pos) { worldTransform.translation = pos; }
	Vector3 GetScale() { return worldTransform.scale; }
	void SetScale(Vector3 scale) { worldTransform.scale = scale; }
	Vector3 GetRotation() { return worldTransform.rotation; }
	void SetRotation(Vector3 rot) { worldTransform.rotation = rot * (PI / 180); }

	void SetParameter(GimmickParam param) { SetPosition(param.pos); SetScale(param.scale); SetRotation(param.rot); }

	static bool GetIsStart() { return isStart_; }
	static void SetIsStart(bool isStart) { isStart_ = isStart; }

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
	Input* input = nullptr;
	float rot = 90;

	void OnCollision(BoxCollider* boxCollider);
	void Open();
	void Close();
public:
	// 引数付きコンストラクタ
	Door(GimmickParam gimmickParam) { this->gimmickParam = gimmickParam; }
	void Initialize();
	void Update(bool isLight);
	void Draw() override;

	//当たり判定の大きさを調整
	Vector3 GetRadius() { return { 1.8f,1.9f,1.0f }; }
};

class Candle : public Gimmick, public SphereCollider
{
private:
	bool isLight_ = false;
	size_t lightIndex = 0;
	AddParticleProp particleProp;
	Vector3 lightPos;
	static size_t lightNum;

public:
	// 引数付きコンストラクタ
	Candle(GimmickParam gimmickParam, size_t index) { this->gimmickParam = gimmickParam; lightIndex = index; lightNum++; }
	void OnCollision(RayCollider* rayCollider);
	void Initialize();
	void Update(bool isLight);
	void Draw() override;
	static size_t GetLightNum() { return lightNum; }
	static void ResetLightNum() { lightNum = 0; }
};

class Wall : public Gimmick
{
private:
	// 普通の壁か消える壁かのフラグ
	bool isVanish = false;
	// 存在するかのフラグ
	bool isExist = true;
public:
	// 引数付きコンストラクタ
	Wall(GimmickParam gimmickParam) { this->gimmickParam = gimmickParam; }
	void Initialize();
	void Update(bool isLight);
	void Draw() override;
};