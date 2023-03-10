#include "Player.h"
#include "Stage.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include <algorithm>

void Player::Initialize()
{
	model_[(int)PartId::body] = Model::Create("player_body", true);		//体
	model_[(int)PartId::legR] = Model::Create("player_shoesR", true);	//右足
	model_[(int)PartId::legL] = Model::Create("player_shoesL", true);	//左足
	worldTransform.Initialize();
	input_ = Input::GetInstance();
	eyeCamera.SetParent(&worldTransform);
	eyeCamera.Initialize();

	for (auto& w : modelsTrans_) { w.Initialize(); }
	// 親子関係
	modelsTrans_[(int)PartId::body].parent = &worldTransform;
	modelsTrans_[(int)PartId::legR].parent = &modelsTrans_[(int)PartId::body];
	modelsTrans_[(int)PartId::legL].parent = &modelsTrans_[(int)PartId::body];

	modelsTrans_[(int)PartId::body].translation = { 0.0f,0.15f,0.0f };
	lightGroup_ = Model::GetLightGroup();
	lightGroup_->SetPointLightActive(0, isLight);
	lightGroup_->SetPointLightColor(0, { 1,0.6f,0.6f });
	//lightGroup_->SetPointLightAtten(0, { 0,0.001f,0.002f });
	lightGroup_->SetPointLightAtten(0, { 0.2f });

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
	worldTransform.translation += move;

	// 移動制限
	const Vector2 STAGE_SIZE =
	{
		Stage::STAGE_WIDTH - 1.0f, // 1.0fはズレの修正
		Stage::STAGE_HEIGHT - 1.0f
	};

	worldTransform.translation.x = std::clamp(worldTransform.translation.x, -STAGE_SIZE.x, STAGE_SIZE.x);
	worldTransform.translation.z = std::clamp(worldTransform.translation.z, -STAGE_SIZE.y, STAGE_SIZE.y);
}

void Player::Update()
{
	isCameraChange = false;
	worldTransform.Update();
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

	// 視点に合わせて回転する
	modelsTrans_[(int)PartId::body].rotation.y = eyeCamera.GetAngleTarget();

	for (auto& w : modelsTrans_) { w.Update(); }

	ChangeLight();
}

void Player::Draw()
{
	if (WorldTransform::GetViewProjection() != eyeCamera.GetViewProjection()) // FPS視点じゃないとき
	{
		for (size_t i = 0; i < modelsTrans_.size(); i++) { model_[i]->Draw(modelsTrans_[i]); }
	}
}

void Player::ChangeLight()
{
	if (input_->IsTrigger(Key::Space))
	{
		isLight = !isLight;
		lightGroup_->SetPointLightActive(0, isLight);
	}
	// ライトオン
	if (isLight) { lightGroup_->SetPointLightPos(0, worldTransform.GetWorldPosition()); }

	ImGuiManager::PrintVector("PlayerPos", worldTransform.GetWorldPosition());
}

void Player::OnCollision(BoxCollider* boxCollider)
{
	// 移動
	float speed = -0.5f;
	Vector3 move;
	move.z = input_->Move(Key::W, Key::S, speed);
	move.x = input_->Move(Key::D, Key::A, speed);
	move = Quaternion::RotateVector(move, Quaternion::MakeAxisAngle(Vector3::MakeYAxis(), eyeCamera.GetAngleTarget()));
	worldTransform.translation += move;

	// 移動制限
	const Vector2 STAGE_SIZE =
	{
		Stage::STAGE_WIDTH - 1.0f, // 1.0fはズレの修正
		Stage::STAGE_HEIGHT - 1.0f
	};

	worldTransform.translation.x = std::clamp(worldTransform.translation.x, -STAGE_SIZE.x, STAGE_SIZE.x);
	worldTransform.translation.z = std::clamp(worldTransform.translation.z, -STAGE_SIZE.y, STAGE_SIZE.y);
	worldTransform.Update();
}