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
}

void Player::Move()
{
	//移動
	float speed = 0.5f;
	Vector3 move;
	move.z += input_->Move(Key::W, Key::S, speed);
	move.x += input_->Move(Key::D, Key::A, speed);
	worldTransform_.translation += move;

	//移動制限
	const float StageWidth = Stage::STAGE_WIDTH - 1.0f; // 0.5はズレの修正
	const float StageHeight = Stage::STAGE_HEIGHT - 1.0f;

	worldTransform_.translation.x = std::clamp(worldTransform_.translation.x, -StageWidth, StageWidth);
	worldTransform_.translation.z = std::clamp(worldTransform_.translation.z, -StageHeight, StageHeight);
}

void Player::Update()
{
	Move();

	model_->TextureUpdate();
	worldTransform_.Update();
}

void Player::Draw()
{
	model_->Draw(worldTransform_);
}