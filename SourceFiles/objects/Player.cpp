#include "Player.h"
#include "Stage.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include <algorithm>
#include "Timer.h"

void Player::Initialize()
{
	model_[(int)PartId::root] = Model::Create("cube");					//大元(赤い四角)
	model_[(int)PartId::body] = Model::Create("player_body",true);		//体
	model_[(int)PartId::legR] = Model::Create("player_shoesR",true);	//右足
	model_[(int)PartId::legL] = Model::Create("player_shoesL",true);	//左足
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
	//親子関係
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
	// 移動
	float speed = 0.5f;
	Vector3 move;
	move.z = input_->Move(Key::W, Key::S, speed);
	move.x = input_->Move(Key::D, Key::A, speed);
	move = Quaternion::RotateVector(move, Quaternion::MakeAxisAngle(Vector3::MakeYAxis(), eyeCamera.GetAngleTarget()));
	worldTransform_.translation += move;

	// 移動制限
	const Vector2 STAGE_SIZE =
	{
		Stage::STAGE_WIDTH - 1.0f, // 1.0fはズレの修正
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

	//視点に合わせて回転する
	modelsTrans_[(int)PartId::root].rotation.y = eyeCamera.GetAngleTarget();

	for (size_t i = 0; i < 4; i++){
		modelsTrans_[i].Update();
	}

	ImGui::Text("%.2f", eyeCamera.GetAngleTarget()/PI*180);
}

void Player::Draw()
{
	if(WorldTransform::GetViewProjection() != eyeCamera.GetViewProjection())//FPS視点じゃないとき
	{
		model_[(int)PartId::body]->Draw(modelsTrans_[(int)PartId::body]);
		model_[(int)PartId::legR]->Draw(modelsTrans_[(int)PartId::legR]);
		model_[(int)PartId::legL]->Draw(modelsTrans_[(int)PartId::legL]);
	}	
}

void Player::ChangeLight()
{

}