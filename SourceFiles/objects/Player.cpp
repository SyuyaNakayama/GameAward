#include "Player.h"
#include "ImGuiManager.h"
#include "UIDrawer.h"
#include <imgui.h>
#include <algorithm>

void Player::Initialize(const Vector3& startPos)
{
	// �����蔻��ݒ�
	collisionAttribute = CollisionAttribute::Player;
	collisionMask = CollisionMask::Player;
	// ���f���ǂݍ���
	model_[(int)PartId::body] = Model::Create("player_body", true);		//��
	model_[(int)PartId::legR] = Model::Create("player_shoesR", true);	//�E��
	model_[(int)PartId::legL] = Model::Create("player_shoesL", true);	//����
	worldTransform.Initialize();
	worldTransform.translation = startPos;
	input_ = Input::GetInstance();
	eyeCamera.Initialize(&worldTransform);

	for (auto& w : modelsTrans_) { w.Initialize(); }
	// �e�q�֌W
	modelsTrans_[(int)PartId::body].parent = &worldTransform;
	modelsTrans_[(int)PartId::legR].parent = &modelsTrans_[(int)PartId::body];
	modelsTrans_[(int)PartId::legL].parent = &modelsTrans_[(int)PartId::body];

	modelsTrans_[(int)PartId::body].scale = { 0.5f,0.5f,0.5f };
	modelsTrans_[(int)PartId::body].translation = { 0.0f,0.3f,0.0f };
	modelsTrans_[(int)PartId::legR].translation = { 0.0f,-0.15f,0.0f };
	modelsTrans_[(int)PartId::legL].translation = { 0.0f,-0.15f,0.0f };

	lightGroup_ = Model::GetLightGroup();
	lightGroup_->SetPointLightActive(0, true);
	lightGroup_->SetPointLightAtten(0, { 0,0.000f,0.001f });

	hpUI = UIDrawer::GetUI(6);
	hpUI->SetColor({ 1,0,0,1 });

	jump.SetGravity(0.1f);
	jump.SetWorldTransform(&worldTransform);
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
	move = Quaternion::RotateVector(move, Quaternion::MakeAxisAngle(Vector3::MakeYAxis(), eyeCamera.GetAngleTarget()));
	move.Normalize();
	worldTransform.translation += move;

	// ��������ݒ�
	worldTransform.translation.x = std::clamp(worldTransform.translation.x, -stageSize.x, stageSize.x);
	worldTransform.translation.z = std::clamp(worldTransform.translation.z, -stageSize.y, stageSize.y);

	// ���_�ɍ��킹�ĉ�]����
	worldTransform.rotation.y = eyeCamera.GetAngleTarget();
	// ���[���h�s��̍X�V
	worldTransform.Update();
	// FPS�J�����̍X�V
	eyeCamera.Update();
	WalkMotion();
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

void Player::StandbyMotion()
{
	Vector3 moveBody;
	Vector3 moveLeg;
	float rot = 0;
	if (timerStandby.CountDown())
	{
		isUp = !isUp;
		if (!isUp) { timerStandby = 25; }
	}

	float time = timerStandby.GetInterval();

	if (isUp)
	{
		moveBody.y = (0.4f - 0.3f) / time;
		moveLeg.y = (0.35f - 0.15f) / time;
		moveLeg.z = (0.5f - 0.0f) / time;
		rot = (20 - 0) / time;
	}
	else
	{
		moveBody.y = (0.3f - 0.4f) / time;
		moveLeg.y = (0.15f - 0.35f) / time;
		moveLeg.z = (0.0f - 0.5f) / time;
		rot = (0 - 20) / time;
	}

	//��
	modelsTrans_[(int)PartId::body].translation += moveBody;
	//����
	modelsTrans_[(int)PartId::legL].translation += moveLeg;
	modelsTrans_[(int)PartId::legL].rotation.x += rot * PI / 180;
	//�E��
	modelsTrans_[(int)PartId::legR].translation += moveLeg;
	modelsTrans_[(int)PartId::legR].rotation.x += rot * PI / 180;
}

void Player::WalkMotion()
{
	Vector3 moveBody;
	Vector3 moveLeg;
	float rotR = 0;
	float rotL = 0;
	float time = timerWalk.GetInterval();
	if (timerWalk.CountDown())
	{
		walkNum++;
		if (walkNum >= 4) { walkNum = 0; }
	}

	// fraction.x = moveLeg.y�̕��q
	// fraction.y = moveLeg.z�̕��q
	// fraction.z = rotR�̕��q
	auto& WalkMotionFunc = [&](Vector3 fraction)
	{
		moveLeg.y = fraction.x / time;
		moveLeg.z = fraction.y / time;
		rotR = fraction.z / time;
		rotL = -rotR;
	};

	switch (walkNum)
	{
	case 0://�O�֏o��
		WalkMotionFunc({ 0.35f ,0.5f ,-20.0f });
		break;
	case 1://�O����߂�
		WalkMotionFunc({ -0.35f , -0.5f ,+20.0f });
		break;
	case 2://���Ɉ���
		WalkMotionFunc({ 0.35f ,-0.5f ,20.0f });
		break;
	case 3://��납��߂�
		WalkMotionFunc({ -0.35f ,+0.5f , -20.0f });
		break;
	}

	//���f����
	////��
	//modelsTrans_[(int)PartId::body].translation += moveBody;
	//����
	modelsTrans_[(int)PartId::legL].translation += moveLeg;
	modelsTrans_[(int)PartId::legL].rotation.x += rotR * PI / 180;
	//�E��
	modelsTrans_[(int)PartId::legR].translation.y += moveLeg.y;
	modelsTrans_[(int)PartId::legR].translation.z -= moveLeg.z;
	modelsTrans_[(int)PartId::legR].rotation.x += rotL * PI / 180;
}

void Player::Update()
{
	// �W�����v
	if (input_->IsTrigger(Key::_1)) { jump.Start(1); }
	jump.Update();

	isCameraChange = false;
	// FPS���_�̎�
	if (WorldTransform::GetViewProjection() == eyeCamera.GetViewProjection())
	{
		State = &Player::Move;
	}
	// FPS���_����Ȃ��Ƃ�
	else
	{
		State = &Player::StandbyMotion;
		if (input_->IsTrigger(Mouse::Right))
		{
			isCameraChange = true;
			WorldTransform::SetViewProjection(eyeCamera.GetViewProjection());
		}
	}

	hpUI->SetSize({ (float)hp / MAX_HP * WindowsAPI::WIN_SIZE.x,64 });
	lightGroup_->SetPointLightPos(0, worldTransform.GetWorldPosition());
	(this->*LightUpdate)();
	if (State) { (this->*State)(); }
	for (auto& w : modelsTrans_) { w.Update(); }
}

void Player::Draw()
{
	for (size_t i = 0; i < modelsTrans_.size(); i++) { model_[i]->Draw(modelsTrans_[i]); }
}

void Player::OnCollision(BoxCollider* boxCollider)
{
	// ���ꂼ��̍��W�A���a�擾
	Vector3 boxPos = boxCollider->GetWorldPosition();
	Vector3 boxRadius = boxCollider->GetRadius();
	Vector3 pPos = worldTransform.translation;
	Vector3 playerRadius = GetRadius();

	// �O�t���[���Ƃ̍��ŐN���������m�F����
	if (prePos.x < boxPos.x - boxRadius.x) {
		// �{�b�N�X���������ɉ����o��
		worldTransform.translation.x = std::clamp(worldTransform.translation.x, -stageSize.x, boxPos.x - boxRadius.x - playerRadius.x);
	}
	else if (prePos.x > boxPos.x + boxRadius.x) {
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
	if (prePos.y > boxPos.y + boxRadius.y) {
		worldTransform.translation.y = std::clamp(worldTransform.translation.y, boxPos.y + boxRadius.y + playerRadius.y, 100.0f);
	}
	// �s��̍X�V
	worldTransform.Update();
}