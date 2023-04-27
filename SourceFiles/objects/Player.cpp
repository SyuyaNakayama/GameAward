#include "Player.h"
#include "ImGuiManager.h"
#include "UIDrawer.h"
#include "Stage.h"
#include "SceneManager.h"
#include <imgui.h>
#include <algorithm>

int Player::maxHp = 0;

void Player::Initialize(const Vector3& startPos, const Vector3& startRot)
{
	// �����蔻��ݒ�
	collisionAttribute = CollisionAttribute::Player;
	collisionMask = CollisionMask::Player;
	worldTransform.Initialize();
	worldTransform.translation = startPos;
	worldTransform.rotation = startRot * (PI / 180);
	input_ = Input::GetInstance();
	eyeCamera.Initialize(&worldTransform);

	lightGroup_ = Model::GetLightGroup();
	lightGroup_->SetPointLightActive(0, true);
	lightGroup_->SetPointLightAtten(0, { 0.2f,0.0f,0.001f });
	lightGroup_->SetPointLightColor(0, { 1.0f,0.5f,0.5f });

	maxHp = 6000; // �ő�HP
	// �X�e�[�W1�̏ꍇ�v���C���[�̍ő�HP�����炷
	if (Stage::GetStageNum() == (int)Stage::StageNum::Stage1) { maxHp /= 4; }
	hp = maxHp;
	// HP�Q�[�W
	if (Stage::GetStageNum() != (int)Stage::StageNum::Select)
	{
		hpUI = UIDrawer::GetUI(4);
		hpUI->SetColor({ 1,0,0,1 });
	}

	jump.SetGravity(0.1f);
	jump.SetWorldTransform(&worldTransform);

	heal.Initialize(&worldTransform);
	heal.SetHpPointer(&hp);

	motion.Initialize(&worldTransform);
	UIDrawer::GetUI(17)->SetPosition({ WindowsAPI::WIN_SIZE.x / 2.0f,40 });
}

void Player::Move()
{
	// �O�t���[�����W�擾
	prePos = worldTransform.translation;
	// �ړ��x�N�g�����v�Z
	Vector3 move;
	move.z = input_->Move(Key::W, Key::S, 1.0f);
	move.x = input_->Move(Key::D, Key::A, 1.0f);

	// �ړ����Ă��鎞
	if (move.Length() == 0) { return; } // �~�܂��Ă��鎞
	move *= Matrix4::RotateY(eyeCamera.GetAngle().x + worldTransform.rotation.y);
	move.Normalize();
	// y����]�����o��
	// �ړ������ɍ��킹�ĉ�]����
	float bodyRotY = motion.GetBodyRotation().y;
	// 2D�x�N�g���̍쐬
	Vector2 forward =
	{
		// �����Ă����
		std::cos(bodyRotY + PI / 2.0f + worldTransform.rotation.y),
		std::sin(bodyRotY + PI / 2.0f + worldTransform.rotation.y)
	};
	forward.Normalize();
	Vector2 move2D = { -move.x, move.z }; // ��������������
	float sign = Cross(forward, move2D) > 0 ? 1 : -1; // 2D�x�N�g���̍��E����
	float angle = std::acos(Dot(forward, move2D)) * sign; // �p�x�̍����v�Z
	if (angle != angle) { SceneManager::GetInstance()->SetNextScene(Scene::Play); } // ���[�V�������o�O�����狭�����g���C
	motion.SetBodyRotation({ 0,bodyRotY + angle * 0.4f }); // ��]�̕��
	// �ړ�
	const float MOVE_SPD = 0.5f;
	move *= MOVE_SPD;
	worldTransform.translation += move;
}

void Player::RedFire()
{
	if (input_->IsTrigger(Key::Space))
	{
		LightUpdate = &Player::BlueFire;
		lightGroup_->SetPointLightColor(0, { 0.5f,0.5f,1 });
	}
	if (Stage::GetStageNum() != (int)Stage::StageNum::Select) { hp--; }
}

void Player::BlueFire()
{
	if (input_->IsTrigger(Key::Space))
	{
		LightUpdate = &Player::RedFire;
		lightGroup_->SetPointLightColor(0, { 1.0f,0.5f,0.5f });
	}
	if (Stage::GetStageNum() != (int)Stage::StageNum::Select) { hp -= 2; }
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
	if (input_->IsInput(Key::Return)) { jump.Start(1); }
	jump.Update();
	Move(); // �ړ�
	if (hpUI) { hpUI->SetSize({ (float)hp / maxHp * WindowsAPI::WIN_SIZE.x / 3.0f,32 }); } // HP�Q�[�W�̒���
	(this->*LightUpdate)(); // ���C�g
	motion.MotionUpdate();
	ObjectUpdate(); // �I�u�W�F�N�g�̍X�V
	heal.Update(); // �񕜃G���A�X�V
	baseRayDirection = Vector3::MakeAxis(Axis::Z) * Matrix4::RotateY(motion.GetBodyRotation().y);
	// �����邩HP��0�ɂȂ����狭�����g���C
	if (worldTransform.translation.y <= -20.0f || hp <= 0) { SceneManager::GetInstance()->SetNextScene(Scene::Play); }
	UIDrawer::GetUI(17)->SetIsInvisible(true);
	// �p�[�e�B�N��
	//DiffuseParticle::AddProp addProp =
	//{
	//	worldTransform.translation + Vector3(0,0.5f),
	//	{0,0.01f,0},{0,0.0005f,0},
	//	0.025f,0.001f,0,40,0.8f
	//};
	//ParticleManager::Add(addProp);
}

void Player::Draw()
{
	motion.Draw();
}

void Player::OnCollision(BoxCollider* boxCollider)
{
	if (boxCollider->GetCollisionAttribute() == CollisionAttribute::UI) { return; }
	// ���ꂼ��̍��W�A���a�擾
	Vector3 boxPos = boxCollider->GetWorldPosition();
	Vector3 boxRadius = boxCollider->GetRadius3D();
	Vector3 pPos = worldTransform.translation;
	Vector3 playerRadius = BoxCollider::GetRadius3D();

	// �O�t���[���Ƃ̍��ŐN���������m�F����
	if (prePos.y < boxPos.y - boxRadius.y) {
		// �{�b�N�X���������ɉ����o��
		worldTransform.translation.y = std::clamp(worldTransform.translation.y, -150.0f, boxPos.y - boxRadius.y - playerRadius.y);
		// �s��̍X�V
		ObjectUpdate();
		return;
	}
	else if (prePos.y > boxPos.y + boxRadius.y) {
		// �{�b�N�X�����㑤�ɉ����o��
		worldTransform.translation.y = std::clamp(worldTransform.translation.y, boxPos.y + boxRadius.y + playerRadius.y, 1500.0f);
		// �s��̍X�V
		ObjectUpdate();
		return;
	}
	if (prePos.x < boxPos.x - boxRadius.x) {
		// �{�b�N�X���������ɉ����o��
		worldTransform.translation.x = std::clamp(worldTransform.translation.x, -1500.0f, boxPos.x - boxRadius.x - playerRadius.x);
	}
	else if (prePos.x > boxPos.x + boxRadius.x) {
		// �{�b�N�X�����E���ɉ����o��
		worldTransform.translation.x = std::clamp(worldTransform.translation.x, boxPos.x + boxRadius.x + playerRadius.x, 1500.0f);
	}
	if (prePos.z < boxPos.z - boxRadius.z) {
		// �{�b�N�X������O���ɉ����o��
		worldTransform.translation.z = std::clamp(worldTransform.translation.z, -150.0f, boxPos.z - boxRadius.z - playerRadius.z);
	}
	else if (prePos.z > boxPos.z + boxRadius.z) {
		// �{�b�N�X���������ɉ����o��
		worldTransform.translation.z = std::clamp(worldTransform.translation.z, boxPos.z + boxRadius.z + playerRadius.z, 1500.0f);
	}
	// �s��̍X�V
	ObjectUpdate();
}

void Heal::OnCollision(SphereCollider* sphereCollider)
{
	UIDrawer::GetUI(17)->SetIsInvisible(false);
	*hp += 7;
	*hp = min(*hp, Player::GetMaxHp() + 1);
}