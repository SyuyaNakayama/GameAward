#include "Player.h"
#include "ImGuiManager.h"
#include "UIDrawer.h"
#include "Stage.h"
#include <imgui.h>
#include <algorithm>

int Player::maxHp = 0;

void Player::Initialize(const Vector3& startPos)
{
	// �����蔻��ݒ�
	collisionAttribute = CollisionAttribute::Player;
	collisionMask = CollisionMask::Player;
	worldTransform.Initialize();
	worldTransform.translation = startPos;
	input_ = Input::GetInstance();
	eyeCamera.Initialize(&worldTransform);

	lightGroup_ = Model::GetLightGroup();
	lightGroup_->SetPointLightActive(0, true);
	lightGroup_->SetPointLightAtten(0, { 0.000f,0.001f,0.001f });
	lightGroup_->SetPointLightColor(0, { 0.5f,0.25f,0.25f });

	maxHp = 4000; // �ő�HP
	// �X�e�[�W2�̏ꍇ�v���C���[�̍ő�HP�����炷
	if (Stage::GetStageNum() == (int)Stage::StageNum::Stage2) { maxHp /= 4; }
	hp = maxHp;
	hpUI = UIDrawer::GetUI(4);
	hpUI->SetColor({ 1,0,0,1 });

	jump.SetGravity(0.1f);
	jump.SetWorldTransform(&worldTransform);

	heal.Initialize(&worldTransform);
	heal.SetHpPointer(&hp);

	motion.Initialize(&worldTransform);
}

void Player::Move()
{
	// �O�t���[�����W�擾
	prePos = worldTransform.translation;
	// �ړ�
	Vector3 move;
	float spd = 0.5f;
	move.z = input_->Move(Key::W, Key::S, spd);
	move.x = input_->Move(Key::D, Key::A, spd);
	move *= Matrix4::RotateY(eyeCamera.GetAngle().x);
	move.Normalize();
	worldTransform.translation += move;

	// �ړ����Ă��鎞�A�ړ������ɍ��킹�ĉ�]����
	if (move.Length() == 0) { return; } // �~�܂��Ă��鎞
	// y����]�����o��
	float bodyRotY = motion.GetBodyRotation().y;
	// 2D�x�N�g���̍쐬
	Vector2 forward = { std::cos(bodyRotY + PI / 2.0f),std::sin(bodyRotY + PI / 2.0f) }; // �����Ă����
	Vector2 move2D = { -move.x, move.z }; // ��������������
	float sign = Cross(forward, move2D) > 0 ? 1 : -1; // 2D�x�N�g���̍��E����
	float angle = std::acos(Dot(forward, move2D)) * sign; // �p�x�̍����v�Z
	motion.SetBodyRotation({ 0,bodyRotY + angle * 0.4f }); // ��]�̕��
}

void Player::RedFire()
{
	if (input_->IsTrigger(Key::Space))
	{
		LightUpdate = &Player::BlueFire;
		lightGroup_->SetPointLightColor(0, { 0.5f,0.5f,1 });
	}
	hp--;
}

void Player::BlueFire()
{
	if (input_->IsTrigger(Key::Space))
	{
		LightUpdate = &Player::RedFire;
		lightGroup_->SetPointLightColor(0, { 1.0f,0.5f,0.5f });
	}
	hp -= 2;
}

void Player::ObjectUpdate()
{
	// �s��̍X�V
	worldTransform.Update();
	motion.TransformUpdate();
	eyeCamera.Update();
	lightGroup_->SetPointLightPos(0, worldTransform.GetWorldPosition());
}

void Player::Update()
{
	// �W�����v
	if (input_->IsTrigger(Key::_1)) { jump.Start(1); }
	jump.Update();
	Move(); // �ړ�
	hpUI->SetSize({ (float)hp / maxHp * WindowsAPI::WIN_SIZE.x,64 }); // HP�Q�[�W�̒���
	(this->*LightUpdate)(); // ���C�g
	motion.MotionUpdate();
	ObjectUpdate(); // �I�u�W�F�N�g�̍X�V
	heal.Update(); // �񕜃G���A�X�V
	baseRayDirection *= Matrix4::RotateY(motion.GetBodyRotation().y);
	ImGui::Text("playerHp = %d", hp);
}

void Player::Draw()
{
	motion.Draw();
}

void Player::OnCollision(BoxCollider* boxCollider)
{
	// ���ꂼ��̍��W�A���a�擾
	Vector3 boxPos = boxCollider->GetWorldPosition();
	Vector3 boxRadius = boxCollider->GetRadius3D();
	Vector3 pPos = worldTransform.translation;
	Vector3 playerRadius = BoxCollider::GetRadius3D();

	// �����߂�����
	auto PushBack = [&](size_t axis)
	{
		// �O�t���[���Ƃ̍��ŐN���������m�F����
		if (prePos[axis] < boxPos[axis] - boxRadius[axis]) {
			worldTransform.translation[axis] = std::clamp(
				worldTransform.translation[axis], -150.0f, boxPos[axis] - boxRadius[axis] - playerRadius[axis]);
		}
		else if (prePos[axis] > boxPos[axis] + boxRadius[axis]) {
			worldTransform.translation[axis] = std::clamp(
				worldTransform.translation[axis], boxPos[axis] + boxRadius[axis] + playerRadius[axis], 150.0f);
		}
	};

	// �����o��
	for (size_t i = 0; i < 3; i++) { PushBack(i); }
	// �s��̍X�V
	ObjectUpdate();
}

void Heal::OnCollision(SphereCollider* sphereCollider)
{
	*hp += 2;
	*hp = min(*hp, Player::GetMaxHp() + 1);
}