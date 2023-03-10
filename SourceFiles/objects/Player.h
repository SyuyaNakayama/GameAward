#pragma once
#include <Input.h>
#include <Model.h>
#include "LightGroup.h"
#include "Camera.h"
#include "Collider.h"

class Player : public BoxCollider
{
private:
	Input* input_;
	std::unique_ptr<Model> model_;
	std::unique_ptr<Sprite> sprite_;
	// �J����
	Camera eyeCamera;
	bool isCameraChange = false;
	// ���C�g
	LightGroup* lightGroup_;
	bool useLight;
	// �O�t���[�����W
	Vector3 prePos;

	// �ړ�����
	void Move();
	// ���C�g�؂�ւ�
	void ChangeLight();

public:
	void Initialize();
	void Update();
	void Draw();

	// �A�N�Z�b�T
	WorldTransform GetWorldTransform() { return worldTransform; }
	bool IsCameraChange() { return isCameraChange; }
	// �f�X�g���N�^
	~Player() { sprite_.release(); }

	// �����蔻��̏���
	void OnCollision(BoxCollider* boxCollider) override;
	//void OnCollision(SphereCollider* sphereCollider) override;
	//void OnCollision(PlaneCollider* boxCollider) override;
	//void OnCollision(PolygonCollider* sphereCollider) override;
	//void OnCollision(RayCollider* sphereCollider) override;
	//void OnCollision(IncludeCollider* sphereCollider) override;
};

