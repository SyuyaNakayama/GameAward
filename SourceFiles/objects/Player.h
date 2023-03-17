#pragma once
#include "Input.h"
#include "Model.h"
#include "LightGroup.h"
#include "Camera.h"
#include "Collider.h"
#include "Timer.h"

class Player : public BoxCollider, public RayCollider
{
private:
	enum class PartId { body, legR, legL };

	Input* input_;
	bool isLight = true;
	std::array<WorldTransform, 3> modelsTrans_;
	std::array<std::unique_ptr<Model>, 3> model_;
	Camera eyeCamera;
	bool isCameraChange = false;
	// ���C�g
	LightGroup* lightGroup_;
	bool useLight;
	// �O�t���[�����W
	Vector3 prePos;
	//���[�V����
	bool isStandby = true;
	bool isUp = true;
	Timer timerStandby = 50;
	bool isWaik = true;
	bool isSend = true;
	float timerWalk = 0;
	int walkNum = 0;
	void StandbyMotion();

	// �X�e�[�W�����A�c��
	Vector2 stageSize;
	void WalkMotion();

	/// <param name="spd">�ړ����x�̌W��</param>
	void Move(float spd);
	// ���C�g�؂�ւ�
	void ChangeLight();

public:
	void Initialize();
	void Update();
	void Draw();

	// �A�N�Z�b�T
	WorldTransform GetWorldTransform() { return worldTransform; }
	void SetStageSize(Vector2 size) { stageSize = { size.x - 1.0f, size.y - 1.0f }; }
	bool IsCameraChange() { return isCameraChange; }
	bool* GetIsLight() { return &isLight; }

	// �����蔻��̏���
	void OnCollision(BoxCollider* boxCollider) override;
	//void OnCollision(SphereCollider* sphereCollider) override;
	//void OnCollision(PlaneCollider* boxCollider) override;
	//void OnCollision(PolygonCollider* sphereCollider) override;
	//void OnCollision(RayCollider* sphereCollider) override;
	//void OnCollision(IncludeCollider* sphereCollider) override;
};

