#pragma once
#include "Camera.h"
#include "Jump.h"
#include "PlayerHealZone.h"
#include "Motion.h"

class Heal : public PlayerHealZone
{
	int* hp = nullptr;
public:
	void SetHpPointer(int* hp_) { hp = hp_; }
	void OnCollision(SphereCollider* sphereCollider);
};

class Player : public BoxCollider, public RayCollider
{
public:
	static const int MAX_HP = 4000;

private:
	Input* input_;
	Camera eyeCamera;
	LightGroup* lightGroup_; // ライト
	Vector3 prePos; // 前フレーム座標
	int hp = MAX_HP;
	Jump jump;
	Sprite* hpUI;
	Heal heal; // 回復エリア
	PlayerMotion motion;

	void Move();
	// ライト処理
	void (Player::* LightUpdate)() = &Player::RedFire;
	void RedFire();
	void BlueFire();

	void ObjectUpdate();

public:
	void Initialize(const Vector3& startPos);
	void Update();
	void Draw();
	// HP減少 -> void Candle::OnCollision(RayCollider* rayCollider)で使用
	void HPDecrease(int decVal) { hp -= decVal; }
	// アクセッサ
	bool IsBlueFire() { return LightUpdate == &Player::BlueFire; } // 青炎ならtrue
	// 当たり判定の処理
	void OnCollision(BoxCollider* boxCollider) override;
};