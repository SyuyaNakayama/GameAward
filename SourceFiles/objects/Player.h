#pragma once
#include "Input.h"
#include "Model.h"
#include "Camera.h"
#include "Jump.h"
#include "Timer.h"
#include "PlayerHealZone.h"

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
	enum class PartId { body, legR, legL };

	Input* input_;
	std::array<WorldTransform, 3> modelsTrans_;
	std::array<std::unique_ptr<Model>, 3> model_;
	Camera eyeCamera;
	bool isCameraChange = false;
	// ���C�g
	LightGroup* lightGroup_;
	// �O�t���[�����W
	Vector3 prePos;
	//���[�V����
	bool isUp = true;
	Timer timerStandby = 50;
	Timer timerWalk = 20;
	int walkNum = 0;
	int hp = MAX_HP;
	Jump jump;
	Sprite* hpUI;
	Heal heal; // �񕜃G���A

	void (Player::* State)() = nullptr;
	void StandbyMotion();
	void WalkMotion();
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

	// �A�N�Z�b�T
	bool IsCameraChange() { return isCameraChange; }
	bool IsBlueFire() { return LightUpdate == &Player::BlueFire; }

	// �����蔻��̏���
	void OnCollision(BoxCollider* boxCollider) override;
};

