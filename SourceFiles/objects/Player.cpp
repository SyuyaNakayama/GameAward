#include "Player.h"
#include "ImGuiManager.h"
#include "UIDrawer.h"
#include "Stage.h"
#include <imgui.h>
#include <algorithm>

int Player::maxHp = 0;

void Player::Initialize(const Vector3& startPos)
{
	// 当たり判定設定
	collisionAttribute = CollisionAttribute::Player;
	collisionMask = CollisionMask::Player;
	worldTransform.Initialize();
	worldTransform.translation = startPos;
	input_ = Input::GetInstance();
	eyeCamera.Initialize(&worldTransform);

	lightGroup_ = Model::GetLightGroup();
	lightGroup_->SetPointLightActive(0, true);
	lightGroup_->SetPointLightAtten(0, { 0.2f,0.0f,0.001f });
	lightGroup_->SetPointLightColor(0, { 1.0f,0.5f,0.5f });

	maxHp = 4000; // 最大HP
	// ステージ2の場合プレイヤーの最大HPを減らす
	if (Stage::GetStageNum() == (int)Stage::StageNum::Stage2) { maxHp /= 4; }
	hp = maxHp;
	// HPゲージ
	if (Stage::GetStageNum() != (int)Stage::StageNum::Select)
	{
		hpUI = UIDrawer::GetUI(4);
		hpUI->SetColor({ 1,0,0,1 });
	}

	jump.SetGravity(0.1f);
	jump.SetWorldTransform(&worldTransform);

	heal.Initialize(&worldTransform);
	heal.SetHpPointer(&hp);

	motion.Initialize(&worldTransform);
}

void Player::Move()
{
	// 前フレーム座標取得
	prePos = worldTransform.translation;
	// 移動ベクトルを計算
	Vector3 move;
	move.z = input_->Move(Key::W, Key::S, 1.0f);
	move.x = input_->Move(Key::D, Key::A, 1.0f);

	// 移動している時
	if (move.Length() == 0) { return; } // 止まっている時
	move *= Matrix4::RotateY(eyeCamera.GetAngle().x);
	move.Normalize();
	// y軸回転を取り出す
	// 移動方向に合わせて回転する
	float bodyRotY = motion.GetBodyRotation().y;
	// 2Dベクトルの作成
	Vector2 forward = { std::cos(bodyRotY + PI / 2.0f),std::sin(bodyRotY + PI / 2.0f) }; // 向いてる方向
	Vector2 move2D = { -move.x, move.z }; // 向かせたい方向
	float sign = Cross(forward, move2D) > 0 ? 1 : -1; // 2Dベクトルの左右判定
	float angle = std::acos(Dot(forward, move2D)) * sign; // 角度の差を計算
	motion.SetBodyRotation({ 0,bodyRotY + angle * 0.4f }); // 回転の補間
	// 移動
	const float MOVE_SPD = 0.5f;
	move *= MOVE_SPD;
	worldTransform.translation += move;
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
	// 行列の更新
	worldTransform.Update();
	motion.TransformUpdate();
	eyeCamera.Update();
	lightGroup_->SetPointLightPos(0, worldTransform.GetWorldPosition());
}

void Player::Update()
{
	// ジャンプ
	if (input_->IsInput(Key::_1)) { jump.Start(1); }
	jump.Update();
	Move(); // 移動
	if (hpUI) { hpUI->SetSize({ (float)hp / maxHp * WindowsAPI::WIN_SIZE.x,64 }); } // HPゲージの調整
	(this->*LightUpdate)(); // ライト
	motion.MotionUpdate();
	ObjectUpdate(); // オブジェクトの更新
	heal.Update(); // 回復エリア更新
	baseRayDirection = Vector3::MakeAxis(Axis::Z) * Matrix4::RotateY(motion.GetBodyRotation().y);
	// パーティクル
	//DiffuseParticle::AddProp addProp =
	//{
	//	worldTransform.translation + Vector3(0,0.5f),
	//	{0,0.01f,0},{0,0.0005f,0},
	//	0.025f,0.001f,0,40,0.8f
	//};
	//ParticleManager::Add(addProp);
}

void Player::Draw()
{
	motion.Draw();
}

void Player::OnCollision(BoxCollider* boxCollider)
{
	// それぞれの座標、半径取得
	Vector3 boxPos = boxCollider->GetWorldPosition();
	Vector3 boxRadius = boxCollider->GetRadius3D();
	Vector3 pPos = worldTransform.translation;
	Vector3 playerRadius = BoxCollider::GetRadius3D();

	// 前フレームとの差で侵入方向を確認する
	if (prePos.y < boxPos.y - boxRadius.y) {
		// ボックスよりも下側に押し出す
		worldTransform.translation.y = std::clamp(worldTransform.translation.y, -150.0f, boxPos.y - boxRadius.y - playerRadius.y);
		// 行列の更新
		ObjectUpdate();
		return;
	}
	else if (prePos.y > boxPos.y + boxRadius.y) {
		// ボックスよりも上側に押し出す
		worldTransform.translation.y = std::clamp(worldTransform.translation.y, boxPos.y + boxRadius.y + playerRadius.y, 150.0f);
		// 行列の更新
		ObjectUpdate();
		return;
	}
	if (prePos.x < boxPos.x - boxRadius.x) {
		// ボックスよりも左側に押し出す
		worldTransform.translation.x = std::clamp(worldTransform.translation.x, -150.0f, boxPos.x - boxRadius.x - playerRadius.x);
	}
	else if (prePos.x > boxPos.x + boxRadius.x) {
		// ボックスよりも右側に押し出す
		worldTransform.translation.x = std::clamp(worldTransform.translation.x, boxPos.x + boxRadius.x + playerRadius.x, 150.0f);
	}
	if (prePos.z < boxPos.z - boxRadius.z) {
		// ボックスよりも手前側に押し出す
		worldTransform.translation.z = std::clamp(worldTransform.translation.z, -150.0f, boxPos.z - boxRadius.z - playerRadius.z);
	}
	else if (prePos.z > boxPos.z + boxRadius.z) {
		// ボックスよりも奥側に押し出す
		worldTransform.translation.z = std::clamp(worldTransform.translation.z, boxPos.z + boxRadius.z + playerRadius.z, 150.0f);
	}
	// 行列の更新
	ObjectUpdate();
}

void Heal::OnCollision(SphereCollider* sphereCollider)
{
	*hp += 2;
	*hp = min(*hp, Player::GetMaxHp() + 1);
}