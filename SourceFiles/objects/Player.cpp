#include "Player.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include <algorithm>

void Player::Initialize()
{
	// �����蔻��ݒ�
	SetCollisionAttribute(CollisionAttribute::Player);
	SetCollisionMask(CollisionMask::Player);
	// ���f���ǂݍ���
	model_[(int)PartId::body] = Model::Create("player_body", true);		//��
	model_[(int)PartId::legR] = Model::Create("player_shoesR", true);	//�E��
	model_[(int)PartId::legL] = Model::Create("player_shoesL", true);	//����
	worldTransform.Initialize();
	input_ = Input::GetInstance();
	eyeCamera.SetParent(&worldTransform);
	eyeCamera.Initialize();

	for (auto& w : modelsTrans_) { w.Initialize(); }
	// �e�q�֌W
	modelsTrans_[(int)PartId::body].parent = &worldTransform;
	modelsTrans_[(int)PartId::legR].parent = &modelsTrans_[(int)PartId::body];
	modelsTrans_[(int)PartId::legL].parent = &modelsTrans_[(int)PartId::body];

	modelsTrans_[(int)PartId::body].translation = { 0.0f,0.15f,0.0f };
	lightGroup_ = Model::GetLightGroup();
	lightGroup_->SetPointLightActive(0, isLight);
	lightGroup_->SetPointLightColor(0, { 1,0.6f,0.6f });
	lightGroup_->SetPointLightAtten(0, { 0,0.001f,0.002f });

	modelsTrans_[(int)PartId::body].scale = { 0.5f,0.5f,0.5f };
}

void Player::Move(float spd)
{
	// �O�t���[�����W�擾
	prePos = worldTransform.translation;
	// �ړ�
	Vector3 move;
	move.z = input_->Move(Key::W, Key::S, spd);
	move.x = input_->Move(Key::D, Key::A, spd);
	move = Quaternion::RotateVector(move, Quaternion::MakeAxisAngle(Vector3::MakeYAxis(), eyeCamera.GetAngleTarget()));
	worldTransform.translation += move;

	// ��������ݒ�
	worldTransform.translation.x = std::clamp(worldTransform.translation.x, -stageSize.x, stageSize.x);
	worldTransform.translation.z = std::clamp(worldTransform.translation.z, -stageSize.y, stageSize.y);
}

void Player::Update()
{
	isCameraChange = false;
	worldTransform.Update();
	if (WorldTransform::GetViewProjection() == eyeCamera.GetViewProjection())
	{
		Move(0.5f);
		eyeCamera.Update();
	}
	else if (input_->IsTrigger(Mouse::Right))
	{
		isCameraChange = true;
		WorldTransform::SetViewProjection(eyeCamera.GetViewProjection());
	}

	// ���_�ɍ��킹�ĉ�]����
	worldTransform.rotation.y = eyeCamera.GetAngleTarget();

	for (auto& w : modelsTrans_) { w.Update(); }

	ChangeLight();
}

void Player::Draw()
{
	if (WorldTransform::GetViewProjection() != eyeCamera.GetViewProjection()) // FPS���_����Ȃ��Ƃ�
	{
		for (size_t i = 0; i < modelsTrans_.size(); i++) { model_[i]->Draw(modelsTrans_[i]); }
	}
}

void Player::ChangeLight()
{
	if (input_->IsTrigger(Key::Space))
	{
		isLight = !isLight;
		lightGroup_->SetPointLightActive(0, isLight);
	}
	// ���C�g�I��
	if (isLight) { lightGroup_->SetPointLightPos(0, worldTransform.GetWorldPosition()); }

	ImGuiManager::PrintVector("PlayerPos", worldTransform.GetWorldPosition());
}

void Player::OnCollision(BoxCollider* boxCollider)
{
	// ���ꂼ��̍��W�A���a�擾
	Vector3 boxPos = boxCollider->GetWorldPosition();
	Vector3 boxRadius = boxCollider->GetRadius();
	Vector3 playerRadius = GetRadius();
	// �O�t���[���Ƃ̍��ŐN���������m�F����
	if (prePos.x < boxPos.x - boxRadius.x) {
		// �{�b�N�X���������ɉ����o��
		worldTransform.translation.x = std::clamp(worldTransform.translation.x, -stageSize.x, boxPos	.x - boxRadius.x - playerRadius.x);
	}
	else if (prePos.x > boxPos.x  + boxRadius.x) {
		// �{�b�N�X�����E���ɉ����o��
		worldTransform.translation.x = std::clamp(worldTransform.translation.x, boxPos.x + boxRadius.x + playerRadius.x, stageSize.x);
	}
	else if (prePos.z < boxPos.z - boxRadius.z) {
		// �{�b�N�X���������ɉ����o��
		worldTransform.translation.z = std::clamp(worldTransform.translation.z, -stageSize.y, boxPos.z - boxRadius.z - playerRadius.z);
	}
	else if (prePos.z > boxPos.z + boxRadius.z) {
		// �{�b�N�X�����㑤�ɉ����o��
		worldTransform.translation.z = std::clamp(worldTransform.translation.z, boxPos.z + boxRadius.z + playerRadius.z, stageSize.y);
	}
	// �s��̍X�V
	worldTransform.Update();
}