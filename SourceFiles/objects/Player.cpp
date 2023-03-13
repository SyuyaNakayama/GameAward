#include "Player.h"
#include "Stage.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include <algorithm>

// 移動制限用定数
const Vector2 STAGE_SIZE =
{
	Stage::STAGE_WIDTH - 1.0f, // 1.0fはズレの修正
	Stage::STAGE_HEIGHT - 1.0f
};

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
	modelsTrans_[(int)PartId::legR].parent = &worldTransform;
	modelsTrans_[(int)PartId::legL].parent = &worldTransform;

	modelsTrans_[(int)PartId::body].translation = { 0.0f,0.3f,0.0f };
	modelsTrans_[(int)PartId::legR].translation = { 0.0f,0.15f,0.0f };
	modelsTrans_[(int)PartId::legL].translation = { 0.0f,0.15f,0.0f };

	modelsTrans_[(int)PartId::body].scale = { 0.5f,0.5f,0.5f };
	modelsTrans_[(int)PartId::legR].scale = { 0.5f,0.5f,0.5f };
	modelsTrans_[(int)PartId::legL].scale = { 0.5f,0.5f,0.5f };

	//ライト
	lightGroup_ = Model::GetLightGroup();
	lightGroup_->SetPointLightActive(0, isLight);
	lightGroup_->SetPointLightColor(0, { 1,0.6f,0.6f });
	lightGroup_->SetPointLightAtten(0, { 0,0.001f,0.002f });
}

void Player::Move(float spd)
{
	// 前フレーム座標取得
	prePos = worldTransform.translation;
	// 移動
	Vector3 move;
	move.z = input_->Move(Key::W, Key::S, spd);
	move.x = input_->Move(Key::D, Key::A, spd);
	move = Quaternion::RotateVector(move, Quaternion::MakeAxisAngle(Vector3::MakeYAxis(), eyeCamera.GetAngleTarget()));
	worldTransform.translation += move;

	// 下限上限設定
	worldTransform.translation.x = std::clamp(worldTransform.translation.x, -STAGE_SIZE.x, STAGE_SIZE.x);
	worldTransform.translation.z = std::clamp(worldTransform.translation.z, -STAGE_SIZE.y, STAGE_SIZE.y);
}

void Player::StandbyMotion()
{

	Vector3 moveBody;
	Vector3 moveLeg;
	float rot;
	float time = 50;
	if(isUp == true)
	{
		timer++;
		moveBody.y = (0.4f - 0.3f) / time;
		moveLeg.y = (0.35f - 0.15f) / time;
		moveLeg.z = (0.5f - 0.0f) / time;
		rot = (20 - 0) / time;
		if (timer >= time)
		{
			isUp = false;
			timer = time;
		}
	}
	else
	{
		timer--;
		moveBody.y = (0.3f - 0.4f) / time;
		moveLeg.y = (0.15f - 0.35f) / time;
		moveLeg.z = (0.0f - 0.5f) / time;
		rot = (0 - 20) / time;
		if (timer <= 0)
		{
			isUp = true;
			timer = 0;
		}
	}

	//体
	modelsTrans_[(int)PartId::body].translation += moveBody;
	//左足
	modelsTrans_[(int)PartId::legL].translation += moveLeg;
	modelsTrans_[(int)PartId::legL].rotation.x += rot * PI / 180;
	//右足
	modelsTrans_[(int)PartId::legR].translation += moveLeg;
	modelsTrans_[(int)PartId::legR].rotation.x += rot * PI / 180;



	ImGui::Text("isUp = %d", isUp);
	ImGui::Text("move.y = %f", moveBody.y);
	ImGui::Text("trans.y = %f", modelsTrans_[(int)PartId::body].translation.y);
}

void Player::Update()
{
	isCameraChange = false;
	worldTransform.Update();
	if (WorldTransform::GetViewProjection() == eyeCamera.GetViewProjection())
	{
		Move(0.5f);
		eyeCamera.Update();
	}
	else if (input_->IsTrigger(Mouse::Right))
	{
		isCameraChange = true;
		WorldTransform::SetViewProjection(eyeCamera.GetViewProjection());
	}

	// 視点に合わせて回転する
	worldTransform.rotation.y = eyeCamera.GetAngleTarget();

	for (auto& w : modelsTrans_) { w.Update(); }

	ChangeLight();
	StandbyMotion();

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
	// それぞれの座標、半径取得
	Vector3 boxPos = boxCollider->GetWorldPosition();
	Vector3 boxRadius = boxCollider->GetRadius();
	Vector3 playerRadius = GetRadius();
	// 前フレームとの差で侵入方向を確認する
	if (prePos.x < boxPos.x - boxRadius.x) {
		// ボックスよりも左側に押し出す
		worldTransform.translation.x = std::clamp(worldTransform.translation.x, -STAGE_SIZE.x, boxPos	.x - boxRadius.x - playerRadius.x);
	}
	else if (prePos.x > boxPos.x  + boxRadius.x) {
		// ボックスよりも左側に押し出す
		worldTransform.translation.x = std::clamp(worldTransform.translation.x, boxPos.x + boxRadius.x + playerRadius.x, STAGE_SIZE.x);
	}
	else if (prePos.z < boxPos.z - boxRadius.z) {
		// ボックスよりも下側に押し出す
		worldTransform.translation.z = std::clamp(worldTransform.translation.z, -STAGE_SIZE.y, boxPos.z - boxRadius.z - playerRadius.z);
	}
	else if (prePos.z > boxPos.z + boxRadius.z) {
		// ボックスよりも上側に押し出す
		worldTransform.translation.z = std::clamp(worldTransform.translation.z, boxPos.z + boxRadius.z + playerRadius.z, STAGE_SIZE.y);
	}
	// 行列の更新
	worldTransform.Update();
}