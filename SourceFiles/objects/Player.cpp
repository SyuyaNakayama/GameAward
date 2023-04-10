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
	modelsTrans_[(int)PartId::body].scale = { 0.5f,0.5f,0.5f };
	modelsTrans_[(int)PartId::body].translation = { 0.0f,0.3f,0.0f };
	for (int i = (int)PartId::legR; i <= (int)PartId::legL; i++)
	{
		modelsTrans_[i].parent = &modelsTrans_[(int)PartId::body];
		modelsTrans_[i].translation = { 0.0f,-0.15f,0.0f };
	}

	lightGroup_ = Model::GetLightGroup();
	lightGroup_->SetPointLightActive(0, true);
	lightGroup_->SetPointLightAtten(0, { 0,0.000f,0.001f });
	lightGroup_->SetPointLightColor(0, { 1.0f,0.5f,0.5f });

	hpUI = UIDrawer::GetUI(6);
	hpUI->SetColor({ 1,0,0,1 });

	jump.SetGravity(0.1f);
	jump.SetWorldTransform(&worldTransform);

	heal.Initialize(&worldTransform);
	heal.SetHpPointer(&hp);
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

	// ���_�ɍ��킹�ĉ�]����
	worldTransform.rotation.y = eyeCamera.GetAngle().x;
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

void Player::ObjectUpdate()
{
	// �s��̍X�V
	worldTransform.Update();
	for (auto& w : modelsTrans_) { w.Update(); }
	eyeCamera.Update();
	lightGroup_->SetPointLightPos(0, worldTransform.GetWorldPosition());
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

	// �W��(1 or -1)
	int k = isUp ? 1 : -1;

	moveBody.y = 0.1f / time * k;
	moveLeg.y = 0.2f / time * k;
	moveLeg.z = 0.5f / time * k;
	rot = 20.0f / time * k;

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
		walkNum = NumberLoop(walkNum + 1, 3);
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

	switch (walkNum / 2)
	{
	case 0://�O�֏o��
		WalkMotionFunc(Vector3(0.35f, 0.5f, -20.0f) * (walkNum % 2 ? -1 : 1));
		break;
	case 1://�O����߂�
		WalkMotionFunc(Vector3(0.35f, -0.5f, 20.0f) * (walkNum % 2 ? -1 : 1));
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
			WorldTransform::SetViewProjection(eyeCamera.GetViewProjection());
		}
	}

	hpUI->SetSize({ (float)hp / MAX_HP * WindowsAPI::WIN_SIZE.x,64 });
	lightGroup_->SetPointLightPos(0, worldTransform.GetWorldPosition());
	(this->*LightUpdate)();
	if (State) { (this->*State)(); }
	ObjectUpdate();
	heal.Update();
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
	Vector3 playerRadius = BoxCollider::GetRadius();

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
	*hp = min(*hp, Player::MAX_HP + 1);
}