#include "Player.h"
#include "Stage.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include <algorithm>

void Player::Initialize(LightGroup* lightGroup)
{
	model_ = Model::Create("cube");
	sprite_ = Sprite::Create("white1x1.png");
	sprite_->SetColor({ 1,0,0,1 });
	model_->SetSprite(sprite_.get());
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	eyeCamera.SetParent(&worldTransform_);
	eyeCamera.Initialize();
	lightGroup_ = lightGroup;
	lightGroup_->SetPointLightActive(0, isLight);
	lightGroup_->SetPointLightColor(0, { 1,1,1 });
	lightGroup_->SetPointLightAtten(0, { 0,0.1f,0.01f });
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
	model_->TextureUpdate();
	worldTransform_.Update();
	ChangeLight();
}

void Player::Draw()
{
	model_->Draw(worldTransform_);
}

void Player::ChangeLight()
{
	if (input_->IsTrigger(Key::Space))
	{
		isLight = !isLight;
		lightGroup_->SetPointLightActive(0, isLight);
	}
	// ライトオン
	if (isLight) { lightGroup_->SetPointLightPos(0, worldTransform_.GetWorldPosition()); }
}