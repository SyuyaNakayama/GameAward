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
	LightGroup* lightGroup_; // ���C�g
	Vector3 prePos; // �O�t���[�����W
	int hp = MAX_HP;
	Jump jump;
	Sprite* hpUI;
	Heal heal; // �񕜃G���A
	PlayerMotion motion;

	void Move();
	// ���C�g����
	void (Player::* LightUpdate)() = &Player::RedFire;
	void RedFire();
	void BlueFire();

	void ObjectUpdate();

public:
	void Initialize(const Vector3& startPos);
	void Update();
	void Draw();
	// HP���� -> void Candle::OnCollision(RayCollider* rayCollider)�Ŏg�p
	void HPDecrease(int decVal) { hp -= decVal; }
	// �A�N�Z�b�T
	bool IsBlueFire() { return LightUpdate == &Player::BlueFire; } // ���Ȃ�true
	// �����蔻��̏���
	void OnCollision(BoxCollider* boxCollider) override;
};