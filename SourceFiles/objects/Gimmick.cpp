#include <imgui.h>
#include <random>
#include "Gimmick.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "Stage.h"
#include "SceneManager.h"
#include "UIDrawer.h"

bool Gimmick::isStart_;
LightGroup* Gimmick::lightGroup = nullptr;
size_t Candle::lightNum = 0;
Player* Block::player = nullptr;
// 鍵の静的メンバ変数の初期化
size_t KeyLock::keyNum = 0;
size_t KeyLock::collectKeyNum = 0;
bool KeyLock::isCollectAll = false;

void Gimmick::Initialize(const GimmickParam& param)
{
	worldTransform.translation = param.pos;
	worldTransform.scale = param.scale;
	worldTransform.rotation = param.rot * (PI / 180);
}

/// <summary>
/// Doorクラスの初期化処理
/// </summary>
void Door::Initialize(const GimmickParam& param)
{
	// パラメータセット
	Gimmick::Initialize(param);
	// モデル読み込み
	model = Model::Create("door", true);
	model_back = Model::Create("door_back");
	model->SetAnbient({ 1,1,1 });
	model->Update();
	// 各モデルのworldTransform初期化とモデルの位置調整
	worldTransform.Initialize();
	worldTransform.translation += { 0, 1.35f };
	worldTransform.scale = { 1.8f,1.9f,2.0f };
	for (auto& w : door)
	{
		// 親子関係
		w.parent = &worldTransform;
		w.Initialize();
		// 大きさを調整
		w.scale = { 1.1f,1.05f,1.0f };
		// 開ける
		w.rotation.y = -PI / 2.0f;
	}
	// 座標を調整
	door[(int)WTType::L].translation += {-1.4f, -1.3f, 0.0f};
	door[(int)WTType::R].translation += { 1.4f, -1.3f, 0.0f};
}

/// <summary>
/// ドアの更新処理
/// </summary>
void Door::Update()
{
	assert(Move);
	(this->*Move)(); // 扉を動かす

	for (auto& w : door) { w.Update(); }
	worldTransform.Update();
}

/// <summary>
/// ドアを開く
/// </summary>
void Door::Open()
{
	if (++rot >= 90) { Move = &Door::Opened; }

	door[(int)WTType::L].rotation.y = (rot + 180) * PI / 180;
	door[(int)WTType::R].rotation.y = -rot * PI / 180;
}

/// <summary>
/// ドアを閉じる
/// </summary>
void Door::Close()
{
	if (--rot <= 0)
	{
		isStart_ = true;
		Move = &Door::Closed;
	}

	door[(int)WTType::L].rotation.y = (rot + 180) * PI / 180;
	door[(int)WTType::R].rotation.y = -rot * PI / 180;
}

void Door::Opened()
{
	if (input->IsTrigger(Key::P)) { Move = &Door::Close; }	// 扉を閉める
}

void Door::Closed()
{
	if (input->IsTrigger(Key::O)) { Move = &Door::Open; }
	// ゴール判定
	// Playerのライトインデックスは飛ばす
	switch (SceneManager::GetInstance()->GetNowScene())
	{
	case Scene::Title:
		if (lightGroup->GetPointLightActive(doorIndex))
		{
			// ライトがついている時
			Move = &Door::Open;
		}
		return;
	case Scene::Play:
		for (size_t i = 1; i <= Candle::GetLightNum(); i++)
		{
			if (lightGroup->GetPointLightActive(i) && KeyLock::GetIsCollectAll())
			{
				// ライトがついている時
				Move = &Door::Open;
				continue;
			}
			// ライトがついていない時は関数を終了する
			Move = &Door::Closed;
			return;
		}
	}
}

/// <summary>
/// ドアに当たった時
/// </summary>
void Door::OnCollision(BoxCollider* boxCollider)
{
	if (Move != &Door::Opened) { return; } // ドアが空いている時ゴール

	SceneManager* sceneManager = SceneManager::GetInstance();

	switch (sceneManager->GetNowScene())
	{
	case Scene::Title:
		Stage::SetStageNum(doorIndex);
		sceneManager->SetNextScene(Scene::Play);
		return;
	case Scene::Play:
		//sceneManager->SetNextScene(Scene::Clear);
		return;
	}
}

/// <summary>
/// ドアの描画処理
/// </summary>
void Door::Draw()
{
	for (auto& w : door) { model->Draw(w); }
	model_back->Draw(worldTransform);
}

void KeyLock::Initialize(const GimmickParam& param)
{
	// 当たり判定設定
	collisionAttribute = CollisionAttribute::Block;
	collisionMask = CollisionMask::Block;
	// モデル読み込み
	model = Model::Create("cube", true);
	// 初期化
	worldTransform.Initialize();
	// パラメータセット
	Gimmick::Initialize(param);
	// 鍵(欠片?)の数を増やす
	keyNum++;
}

void KeyLock::Update()
{
	worldTransform.Update();
	// 鍵を全て集めたらフラグをオンにする
	if (!isCollectAll && collectKeyNum == keyNum) {
		isCollectAll = true;
	}
}

void KeyLock::Draw()
{
	// まだ取得されてないなら描画する
	if (!isCollected) { model->Draw(worldTransform); }
}

void KeyLock::OnCollision(BoxCollider* boxCollider)
{
	// 取得した欠片の数を増やす
	collectKeyNum++;
	// 収集済みフラグをオンにする
	isCollected = true;
	// 当たり判定をなくす
	collisionMask = CollisionMask::None;
}

void Candle::Initialize(const GimmickParam& param)
{
	// パラメータセット
	Gimmick::Initialize(param);
	model = Model::Create("candle", true);
	worldTransform.Initialize();
	worldTransform.translation.y -= 1.0f;
	worldTransform.scale = { 2.0f,2.0f,2.0f };
	lightGroup = Model::GetLightGroup();
	lightPos = worldTransform.translation + Vector3(0, worldTransform.scale.y + 1.2f);
	particleProp =
	{
		lightPos,{0,0.01f,0},{0,0.0005f,0},
		0.025f,0.001f,0,40,0.4f
	};
	lightGroup->SetPointLightAtten(lightIndex, { 0.2f, 0.01f });
	lightGroup->SetPointLightColor(lightIndex, { 1,0.5f,0.5f });
	ui = UIDrawer::GetUI((size_t)2 + Input::GetInstance()->IsConnectGamePad());
}

void Candle::Update()
{
	worldTransform.Update();
	(this->*Fire)();
	model->Update();
	isLight = false;
	ui->SetIsInvisible(true);
}

void Candle::Dark()
{
	lightGroup->SetPointLightActive(lightIndex, false);
	model->SetAnbient({ 0.1f,0.1f,0.1f });
	if (isLight)
	{
		Fire = &Candle::PreLight;
		particleTimer = 60;
	}
}

void Candle::PreLight()
{
	if (--particleTimer <= 0) { Fire = &Candle::PostLight; }
	std::random_device rnd;
	std::mt19937 rnddev(rnd());
	std::uniform_real_distribution<float> randRadius(0, 2.0f);
	std::uniform_real_distribution<float> randAngle(-PI / 2.0f, PI / 2.0f);
	DirectionalParticle::AddProp particleProp =
	{
		playerPos,worldTransform.translation + Vector3(0, worldTransform.scale.y + 1.2f),
		0.5f,2,randAngle(rnddev),randRadius(rnddev),60
	};

	ParticleManager::Add(particleProp);
}

void Candle::PostLight()
{
	lightGroup->SetPointLightActive(lightIndex, true);
	model->SetAnbient({ 0.7f,0.3f,0.3f });
	lightPos = worldTransform.translation + Vector3(0, worldTransform.scale.y + 1.2f);

	particleProp.posOffset = lightPos;

	lightGroup->SetPointLightPos(lightIndex, lightPos);
	ParticleManager::Add(particleProp);
}

void Candle::OnCollision(RayCollider* rayCollider)
{
	if (Length(rayCollider->GetWorldPosition() - worldTransform.GetWorldPosition()) >= 8.0f) { return; }
	if (Stage::GetStageNum() == (UINT16)Stage::StageNum::Tutorial)
	{
		ui->SetIsInvisible(Fire != &Candle::Dark);
		ui->SetPosition(To2DVector(worldTransform.GetWorldPosition() + Vector3(0, 1, 0)));
	}
	if (!Input::GetInstance()->IsTrigger(Mouse::Left)) { return; }
	isLight = true;
	playerPos = rayCollider->GetWorldPosition();
}

void Block::Initialize(const GimmickParam& param)
{
	// 当たり判定設定
	collisionAttribute = CollisionAttribute::Block;
	collisionMask = CollisionMask::Block;
	// パラメータセット
	Gimmick::Initialize(param);
	if (param.vanishFlag == 1) { blockState |= (int)BlockStatus::VANISH_RED; }
	else if (param.vanishFlag == 2) { blockState |= (int)BlockStatus::VANISH_BLUE; }
	if (param.moveFlag == 1) { blockState |= (int)BlockStatus::MOVE; limits = param.limits; }

	// テクスチャ読み込み
	std::unique_ptr<Sprite> sprite;
	switch (param.textureIndex)
	{
	case 0:	sprite = Sprite::Create("white1x1.png");		break;
	case 1:	sprite = Sprite::Create("stages/floor.png");	break;
	}
	sprite->SetSize(sprite->GetSize() / max(max(param.scale.x, param.scale.y), param.scale.z) * 10.0f);
	// モデル読み込み
	model = Model::Create("cube");
	model->SetSprite(std::move(sprite));
	model->Update();
	// 初期化
	worldTransform.Initialize();
}

void Block::Update()
{
	// 当たり判定設定
	if ((blockState & (int)BlockStatus::VANISH_RED) && !player->IsBlueFire()) { collisionMask = CollisionMask::None; }
	//else if (blockState & (int)WallStatus::VANISH_RED && player->IsRedFire()) { collisionMask = CollisionMask::None; }
	else { collisionMask = CollisionMask::Block; }
	// 移動
	if (blockState & (int)BlockStatus::MOVE) { Move(); }
	// 更新
	worldTransform.Update();
}

void Block::Draw()
{
	// 当たり判定がないなら描画しない
	if (collisionMask == CollisionMask::None) { return; }
	// あるなら描画
	else { Gimmick::Draw(); }
}

void Block::Move()
{
	worldTransform.translation.y += speed;
	if (interval > 0) { interval--; return; }
	if (worldTransform.translation.y > limits.x) { speed = -speed; interval = 120; }
	if (worldTransform.translation.y < limits.y) { speed = -speed; interval = 120; }
}
