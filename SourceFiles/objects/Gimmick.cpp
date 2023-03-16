#include "Gimmick.h"
#include "ImGuiManager.h"
#include "Input.h"
#include <imgui.h>
#include <random>
#include "Stage.h"
#include "SceneManager.h"

bool Gimmick::isStart_;
LightGroup* Gimmick::lightGroup = nullptr;
size_t Candle::lightNum = 0;

/// <summary>
/// Doorクラスの初期化処理
/// </summary>
void Door::Initialize()
{
	// パラメータセット
	SetParameter(gimmickParam);
	// モデル読み込み
	model = Model::Create("door", true);
	model_back = Model::Create("door_back");

	// 各モデルのworldTransform初期化とモデルの位置調整
	worldTransform.Initialize();
	worldTransform.translation += { 0, 1.5f, 0.2f };
	worldTransform.scale = { 1.8f,1.9f,2.0f };
	for (auto& w : door)
	{
		w = worldTransform;
		w.Initialize();
		// 大きさを調整
		w.scale = { 2.0f,2.0f,2.0f };
		// 開ける
		w.rotation.y = -PI / 2.0f;
	}
	// 座標を調整
	door[(int)WTType::L].translation += {-2.5f, -2.5f, 0.0f};
	door[(int)WTType::R].translation += { 2.5f, -2.5f, 0.0f};
}

/// <summary>
/// ドアの更新処理
/// </summary>
void Door::Update(bool isLight)
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
			if (lightGroup->GetPointLightActive(i))
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
		sceneManager->SetNextScene(Scene::Clear);
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

void Candle::Initialize()
{
	// パラメータセット
	SetParameter(gimmickParam);
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
}

void Candle::Update(bool isLight)
{
	//ImGuiManager::DragVector("candlePos", worldTransform.translation);
	worldTransform.Update();
	(this->*Fire)();
	model->Update();
	isLight = false;
}

void Candle::Dark()
{
	lightGroup->SetPointLightActive(lightIndex, false);
	model->SetAnbient({ 0,0,0 });
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
	//ImGuiManager::DragVector("particleVelOffset", particleProp.velOffset, 0.001f);
	//ImGuiManager::DragVector("particleAccOffset", particleProp.accOffset, 0.00001f);
	//ImGui::DragFloat("particlePosRange", &particleProp.posRange, 0.002f, 0.002f, 2.0f);
	//ImGui::DragFloat("particleVelRange", &particleProp.velRange, 0.0001f, 0.0001f, 0.1f);
	//ImGui::DragFloat("particleAccRange", &particleProp.accRange, 0.00001f, 0.00001f, 0.01f);
	//ImGui::DragInt("particleLifeTime", &particleProp.lifeTime, 1, 1, 1000);
	//ImGui::DragFloat("particleStartScale", &particleProp.start_scale, 0.05f, 0.05f, 10.0f);

	lightGroup->SetPointLightPos(lightIndex, lightPos);
	ParticleManager::Add(particleProp);
}

void Candle::OnCollision(RayCollider* rayCollider)
{
	if (!Input::GetInstance()->IsTrigger(Mouse::Left)) { return; }
	if (Length(rayCollider->GetWorldPosition() - worldTransform.GetWorldPosition()) < 8.0f)
	{
		isLight = true;
		playerPos = rayCollider->GetWorldPosition();
	}
}

void Wall::Initialize()
{
	// 当たり判定設定
	collisionAttribute = CollisionAttribute::Block;
	collisionMask = CollisionMask::Block;
	// パラメータセット
	SetParameter(gimmickParam);
	isVanish = gimmickParam.flag;
	// モデル読み込み
	model = Model::Create("cube", true);
	// 初期化
	worldTransform.Initialize();
	// ワールドトランスフォームを変更
	worldTransform.scale.y = 5.0f;
	worldTransform.translation.y = 4.0f;
}

void Wall::Update(bool isLight)
{
	// プレイヤーの光が点いているときは透過する
	if (!isLight) {
		// 当たり判定設定
		collisionAttribute = CollisionAttribute::MouseRay;
		collisionMask = CollisionMask::MouseRay;
		isExist = false;
	}
	else if (isLight) {
		// 当たり判定設定
		collisionAttribute = CollisionAttribute::Block;
		collisionMask = CollisionMask::Block;
		isExist = true;
	}
	// 更新
	worldTransform.Update();
}

void Wall::Draw()
{
	if (isExist) { model->Draw(worldTransform); }
}
