#include "Player.h"
#include "Stage.h"
#include <imgui.h>

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
	//ˆÚ“®
	float speed = 0.5f;
	Vector3 move;
	move.z += input_->Move(Key::W, Key::S, speed);
	move.x += input_->Move(Key::D, Key::A, speed);
	worldTransform_.translation += move;

	//ˆÚ“®§ŒÀ
	const int StageWidth = Stage::STAGE_WIDTH - 0.5f;//0.5‚ÍƒYƒŒ‚ÌC³
	const int StageHeight = Stage::STAGE_HEIGHT - 0.5;

	worldTransform_.translation.x = max(worldTransform_.translation.x, -StageWidth);
	worldTransform_.translation.x = min(worldTransform_.translation.x, +StageWidth);
	worldTransform_.translation.z = max(worldTransform_.translation.z, -StageHeight);
	worldTransform_.translation.z = min(worldTransform_.translation.z, +StageHeight);
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