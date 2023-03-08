#include "Player.h"
#include "Stage.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include <algorithm>
#include "Timer.h"

void Player::Initialize()
{
	model_[(int)PartId::root] = Model::Create("cube");					//�匳(�Ԃ��l�p)
	model_[(int)PartId::body] = Model::Create("player_body",true);		//��
	model_[(int)PartId::legR] = Model::Create("player_shoesR",true);	//�E��
	model_[(int)PartId::legL] = Model::Create("player_shoesL",true);	//����
	sprite_ = Sprite::Create("white1x1.png");
	sprite_->SetColor({ 1,0,0,1 });
	model_[0]->SetSprite(sprite_.get());
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	eyeCamera.SetParent(&worldTransform_);
	eyeCamera.Initialize();

	for(int i= 0;i < 4;i++)
	{
		modelsTrans_[i].Initialize();
	}
	//�e�q�֌W
	modelsTrans_[(int)PartId::root].parent = &worldTransform_;
	modelsTrans_[(int)PartId::body].parent = &modelsTrans_[(int)PartId::root];
	modelsTrans_[(int)PartId::legR].parent = &modelsTrans_[(int)PartId::body];
	modelsTrans_[(int)PartId::legL].parent = &modelsTrans_[(int)PartId::body];

	modelsTrans_[(int)PartId::body].translation = { 0.0f,0.15f,0.0f };
	modelsTrans_[(int)PartId::legR].translation = { 0.0f,0.0f,0.0f };
	modelsTrans_[(int)PartId::legL].translation = { 0.0f,0.0f,0.0f };

	modelsTrans_[(int)PartId::body].scale = { 0.5f,0.5f,0.5f };
}

void Player::Move()
{
	// �ړ�
	float speed = 0.5f;
	Vector3 move;
	move.z = input_->Move(Key::W, Key::S, speed);
	move.x = input_->Move(Key::D, Key::A, speed);
	move = Quaternion::RotateVector(move, Quaternion::MakeAxisAngle(Vector3::MakeYAxis(), eyeCamera.GetAngleTarget()));
	worldTransform_.translation += move;

	// �ړ�����
	const Vector2 STAGE_SIZE =
	{
		Stage::STAGE_WIDTH - 1.0f, // 1.0f�̓Y���̏C��
		Stage::STAGE_HEIGHT - 1.0f
	};

	worldTransform_.translation.x = std::clamp(worldTransform_.translation.x, -STAGE_SIZE.x, STAGE_SIZE.x);
	worldTransform_.translation.z = std::clamp(worldTransform_.translation.z, -STAGE_SIZE.y, STAGE_SIZE.y);
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
	model_[(int)PartId::root]->TextureUpdate();
	worldTransform_.Update();

	//���_�ɍ��킹�ĉ�]����
	modelsTrans_[(int)PartId::root].rotation.y = eyeCamera.GetAngleTarget();

	for (size_t i = 0; i < 4; i++){
		modelsTrans_[i].Update();
	}

	ImGui::Text("%.2f", eyeCamera.GetAngleTarget()/PI*180);
}

void Player::Draw()
{
	if(WorldTransform::GetViewProjection() != eyeCamera.GetViewProjection())//FPS���_����Ȃ��Ƃ�
	{
		model_[(int)PartId::body]->Draw(modelsTrans_[(int)PartId::body]);
		model_[(int)PartId::legR]->Draw(modelsTrans_[(int)PartId::legR]);
		model_[(int)PartId::legL]->Draw(modelsTrans_[(int)PartId::legL]);
	}	
}

void Player::ChangeLight()
{

}