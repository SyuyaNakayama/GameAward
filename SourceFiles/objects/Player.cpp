#include "Player.h"
#include "Stage.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include <algorithm>

// �ړ������p�萔
const Vector2 STAGE_SIZE =
{
	Stage::STAGE_WIDTH - 1.0f, // 1.0f�̓Y���̏C��
	Stage::STAGE_HEIGHT - 1.0f
};

void Player::Initialize()
{
	model_ = Model::Create("cube");
	sprite_ = Sprite::Create("white1x1.png");
	sprite_->SetColor({ 1,0,0,1 });
	model_->SetSprite(sprite_.get());
	worldTransform.Initialize();
	input_ = Input::GetInstance();
	eyeCamera.SetParent(&worldTransform);
	eyeCamera.Initialize();
}

void Player::Move()
{
	// �O�t���[�����W�擾
	prePos = worldTransform.translation;
	// �ړ�
	float speed = 0.5f;
	Vector3 move;
	move.z = input_->Move(Key::W, Key::S, speed);
	move.x = input_->Move(Key::D, Key::A, speed);
	move = Quaternion::RotateVector(move, Quaternion::MakeAxisAngle(Vector3::MakeYAxis(), eyeCamera.GetAngleTarget()));
	worldTransform.translation += move;

	// ��������ݒ�
	worldTransform.translation.x = std::clamp(worldTransform.translation.x, -STAGE_SIZE.x, STAGE_SIZE.x);
	worldTransform.translation.z = std::clamp(worldTransform.translation.z, -STAGE_SIZE.y, STAGE_SIZE.y);
}

void Player::Update()
{
	isCameraChange = false;
	if (WorldTransform::GetViewProjection() == eyeCamera.GetViewProjection())
	{
		Move();
		eyeCamera.Update();
	}
	else if (input_->IsTrigger(Mouse::Right))
	{
		isCameraChange = true;
		WorldTransform::SetViewProjection(eyeCamera.GetViewProjection());
	}
	model_->TextureUpdate();
	worldTransform.Update();
}

void Player::Draw()
{
	model_->Draw(worldTransform);
}

void Player::ChangeLight()
{
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
		worldTransform.translation.x = std::clamp(worldTransform.translation.x, -STAGE_SIZE.x, boxPos	.x - boxRadius.x - playerRadius.x);
	}
	else if (prePos.x > boxPos.x  + boxRadius.x) {
		// �{�b�N�X���������ɉ����o��
		worldTransform.translation.x = std::clamp(worldTransform.translation.x, boxPos.x + boxRadius.x + playerRadius.x, STAGE_SIZE.x);
	}
	else if (prePos.z < boxPos.z - boxRadius.z) {
		// �{�b�N�X���������ɉ����o��
		worldTransform.translation.z = std::clamp(worldTransform.translation.z, -STAGE_SIZE.y, boxPos.z - boxRadius.z - playerRadius.z);
	}
	else if (prePos.z > boxPos.z + boxRadius.z) {
		// �{�b�N�X�����㑤�ɉ����o��
		worldTransform.translation.z = std::clamp(worldTransform.translation.z, boxPos.z + boxRadius.z + playerRadius.z, STAGE_SIZE.y);
	}
	// �s��̍X�V
	worldTransform.Update();
}