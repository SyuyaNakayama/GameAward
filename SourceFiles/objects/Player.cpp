#include "Player.h"
#include "ImGuiManager.h"
#include "UIDrawer.h"
#include <imgui.h>
#include <algorithm>

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
	lightGroup_->SetPointLightAtten(0, { 0,0.000f,0.001f });
	lightGroup_->SetPointLightColor(0, { 1.0f,0.5f,0.5f });

	hpUI = UIDrawer::GetUI(6);
	hpUI->SetColor({ 1,0,0,1 });

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
	// 移動
	Vector3 move;
	float spd = 0.5f;
	move.z = input_->Move(Key::W, Key::S, spd);
	move.x = input_->Move(Key::D, Key::A, spd);
	move *= Matrix4::RotateY(eyeCamera.GetAngle().x);
	move.Normalize();
	worldTransform.translation += move;

	// 視点に合わせて回転する
	worldTransform.rotation.y = eyeCamera.GetAngle().x;
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
	motion.Update();
	eyeCamera.Update();
	lightGroup_->SetPointLightPos(0, worldTransform.GetWorldPosition());
}

void Player::Update()
{
	// ジャンプ
	if (input_->IsTrigger(Key::_1)) { jump.Start(1); }
	jump.Update();
	Move(); // 移動
	hpUI->SetSize({ (float)hp / MAX_HP * WindowsAPI::WIN_SIZE.x,64 }); // HPゲージの調整
	(this->*LightUpdate)(); // ライト
	ObjectUpdate(); // オブジェクトの更新
	heal.Update(); // 回復エリア更新
}

void Player::Draw()
{
	motion.Draw();
}

void Player::OnCollision(BoxCollider* boxCollider)
{
	// それぞれの座標、半径取得
	Vector3 boxPos = boxCollider->GetWorldPosition();
	Vector3 boxRadius = boxCollider->GetRadius();
	Vector3 pPos = worldTransform.translation;
	Vector3 playerRadius = BoxCollider::GetRadius();

	// 押し戻し処理
	auto PushBack = [&](size_t axis)
	{
		// 前フレームとの差で侵入方向を確認する
		if (prePos[axis] < boxPos[axis] - boxRadius[axis]) {
			worldTransform.translation[axis] = std::clamp(
				worldTransform.translation[axis], -150.0f, boxPos[axis] - boxRadius[axis] - playerRadius[axis]);
		}
		else if (prePos[axis] > boxPos[axis] + boxRadius[axis]) {
			worldTransform.translation[axis] = std::clamp(
				worldTransform.translation[axis], boxPos[axis] + boxRadius[axis] + playerRadius[axis], 150.0f);
		}
	};

	// 押し出す
	for (size_t i = 0; i < 3; i++) { PushBack(i); }
	// 行列の更新
	ObjectUpdate();
}

void Heal::OnCollision(SphereCollider* sphereCollider)
{
	*hp += 2;
	*hp = min(*hp, Player::MAX_HP + 1);
}