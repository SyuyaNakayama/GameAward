#pragma once
#include "Input.h"
#include "Model.h"
#include "Camera.h"
#include "Jump.h"
#include "Timer.h"

class Player : public BoxCollider, public Jump
{
private:
	const int MAX_HP = 4000;

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
	// �X�e�[�W�����A�c��
	Vector2 stageSize;
	int hp = MAX_HP;

	void (Player::* State)() = nullptr;
	void StandbyMotion();
	void WalkMotion();
	void Move();
	// ���C�g����
	void (Player::* LightUpdate)() = &Player::RedFire;
	void RedFire();
	void BlueFire();

	Sprite* hpUI;

public:
	void Initialize(const Vector3& startPos);
	void Update();
	void Draw();

	// �A�N�Z�b�T
	WorldTransform GetWorldTransform() { return worldTransform; }
	void SetStageSize(Vector2 size) { stageSize = { size.x - 1.0f, size.y - 1.0f }; }
	bool IsCameraChange() { return isCameraChange; }
	bool IsBlueFire() { return LightUpdate == &Player::BlueFire; }

	// �����蔻��̏���
	void OnCollision(BoxCollider* boxCollider) override;
	//void OnCollision(SphereCollider* sphereCollider) override;
	//void OnCollision(PlaneCollider* boxCollider) override;
	//void OnCollision(PolygonCollider* sphereCollider) override;
	//void OnCollision(RayCollider* sphereCollider) override;
	//void OnCollision(IncludeCollider* sphereCollider) override;
};

