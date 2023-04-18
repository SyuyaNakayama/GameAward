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
	LightGroup* lightGroup_ = nullptr; // ���C�g
	Vector3 prePos; // �O�t���[�����W
	int hp = 0;
	Jump jump;
	Sprite* hpUI = nullptr;
	Heal heal; // �񕜃G���A
	PlayerMotion motion;
	// �ړ�
	void Move();
	// ���C�g����
	void (Player::* LightUpdate)() = &Player::RedFire;
	void RedFire();
	void BlueFire();
	// �\�����̍X�V
	void ObjectUpdate();

public:
	void Initialize(const Vector3& startPos);
	void Update();
	void Draw();
	// HP���� -> void Candle::OnCollision(RayCollider* rayCollider)�Ŏg�p
	void HPDecrease(int decVal) { hp -= decVal; }
	// �A�N�Z�b�T
	bool IsBlueFire() { return LightUpdate == &Player::BlueFire; } // ���Ȃ�true
	static int GetMaxHp() { return maxHp; }
	// �����蔻��̏���
	void OnCollision(BoxCollider* boxCollider) override;
};