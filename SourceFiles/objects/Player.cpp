#include "Player.h"
#include "Stage.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include <algorithm>

void Player::Initialize()
{
	model_ = Model::Create("cube");
	sprite_ = Sprite::Create("white1x1.png");
	sprite_->SetColor({ 1,0,0,1 });
	model_->SetSprite(sprite_.get());
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	eyeCamera.SetParent(&worldTransform_);
	eyeCamera.Initialize();
}

void Player::Move()
{
	// ˆÚ“®
	float speed = 0.5f;
	Vector3 move;
	move.z = input_->Move(Key::W, Key::S, speed);
	move.x = input_->Move(Key::D, Key::A, speed);
	move = Quaternion::RotateVector(move, Quaternion::MakeAxisAngle(Vector3::MakeYAxis(), eyeCamera.GetAngleTarget()));
	worldTransform_.translation += move;

	// ˆÚ“®§ŒÀ
	const Vector2 STAGE_SIZE =
	{
		Stage::STAGE_WIDTH - 1.0f, // 1.0f‚ÍƒYƒŒ‚ÌC³
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
}

void Player::Draw()
{
	model_->Draw(worldTransform_);
}

void Player::ChangeLight()
{
}