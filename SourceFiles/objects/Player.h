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
private:
	static int maxHp;
	Input* input_ = nullptr;
	Camera eyeCamera;
	LightGroup* lightGroup_ = nullptr; // ライト
	Vector3 prePos; // 前フレーム座標
	int hp = 0;
	Jump jump;
	Sprite* hpUI = nullptr;
	Heal heal; // 回復エリア
	PlayerMotion motion;
	// 移動
	void Move();
	// ライト処理
	void (Player::* LightUpdate)() = &Player::RedFire;
	void RedFire();
	void BlueFire();
	// 表示物の更新
	void ObjectUpdate();

public:
	void Initialize(const Vector3& startPos);
	void Update();
	void Draw();
	// HP減少 -> void Candle::OnCollision(RayCollider* rayCollider)で使用
	void HPDecrease(int decVal) { hp -= decVal; }
	// アクセッサ
	bool IsBlueFire() { return LightUpdate == &Player::BlueFire; } // 青炎ならtrue
	static int GetMaxHp() { return maxHp; }
	// 当たり判定の処理
	void OnCollision(BoxCollider* boxCollider) override;
};