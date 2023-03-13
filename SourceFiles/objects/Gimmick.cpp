#include "Gimmick.h"
#include "ImGuiManager.h"
#include "Input.h"
#include <imgui.h>
#include <random> 
#include "SceneManager.h"

bool Gimmick::isStart_;
bool Gimmick::isGoal_;
LightGroup* Gimmick::lightGroup = nullptr;
size_t Candle::lightNum = 0;

/// <summary>
/// Doorクラスの初期化処理
/// </summary>
void Door::Initialize()
{
	// モデル読み込み
	model = Model::Create("door");
	model_back = Model::Create("door_back");

	// 各モデルのworldTransform初期化とモデルの位置調整
	worldTransform.Initialize();
	worldTransform.translation.y += 1.5f;
	worldTransform.translation.z += 0.2f;
	worldTransform.scale = { 1.8f,1.9f,2.0f };
	doorL = worldTransform;
	doorR = worldTransform;
	doorR.Initialize();
	doorL.Initialize();
	// 大きさを調整
	doorL.scale = { 2.0f,2.0f,2.0f };
	doorR.scale = { 2.0f,2.0f,2.0f };
	// 座標を調整
	doorL.translation += {-2.5f, -2.5f, 0.0f};
	doorR.translation += { 2.5f, -2.5f, 0.0f};

	// 開ける
	//doorR.rotation.y = -90 * PI / 180;
	doorL.rotation.y = 180 * PI / 180;
}

/// <summary>
/// ドアを開く
/// </summary>
void Door::Open()
{
	switch (SceneManager::GetInstance()->GetNowScene())
	{
	case Scene::Title:
		if (input->IsTrigger(Key::O)) { isOpen = true; }

		break;
	case Scene::Play:
		if (input->IsTrigger(Key::O)) { isOpen = true; }
		// ゴール判定
		// Playerのライトインデックスは飛ばす
		bool goalFlag = true;
		for (size_t i = 1; i <= Candle::GetLightNum(); i++)
		{
			goalFlag &= lightGroup->GetPointLightActive(i);
		}
		isOpen = goalFlag;
		break;
	}

	if (isOpen)
	{
		if (rot >= 90)
		{
			isOpen = false;
			isOpened = true;
		}
		else
		{
			++rot;
			isOpened = false;
		}
		doorR.rotation.y = -rot * PI / 180;
		doorL.rotation.y = (rot + 180) * PI / 180;
	}

}

/// <summary>
/// ドアを閉じる
/// </summary>
void Door::Close()
{
	if (input->IsTrigger(Key::P)) { isClose = true; }
	if (isClose)
	{
		if (--rot <= 0)
		{
			isClose = false;
			isStart_ = true;
		}
		doorR.rotation.y = -rot * PI / 180;
		doorL.rotation.y = (rot + 180) * PI / 180;
	}

}

/// <summary>
/// ドアに当たった時
/// </summary>
void Door::OnCollision(BoxCollider* boxCollider)
{
	if (isOpened) { isGoal_ = true; } // ドアが空いている時ゴール
}

/// <summary>
/// ドアの更新処理
/// </summary>
void Door::Update()
{
	Open();
	Close();

	doorR.Update();
	doorL.Update();
	worldTransform.Update();
}

/// <summary>
/// ドアの描画処理
/// </summary>
void Door::Draw()
{
	model->Draw(doorR);
	model->Draw(doorL);
	model_back->Draw(worldTransform);
}

void Candle::Initialize()
{
	model = Model::Create("candle", true);
	worldTransform.Initialize();
	worldTransform.translation.y -= 1.0f;
	worldTransform.scale = { 2.0f,2.0f,2.0f };
	lightGroup = Model::GetLightGroup();
	particleProp =
	{
		lightPos,{0,0.01f,0},{0,0.0005f,0},
		0.025f,0.001f,0,40,0.4f
	};
	lightPos = worldTransform.translation + Vector3(0, worldTransform.scale.y + 1.2f);
}

void Candle::Update()
{
	//ImGuiManager::DragVector("candlePos", worldTransform.translation);
	worldTransform.Update();

	lightGroup->SetPointLightActive(lightIndex, isLight);
	if (isLight)
	{
		if (particleTimer > 0)
		{
			particleTimer--;
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
		else
		{
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
			lightGroup->SetPointLightAtten(lightIndex, { 0.2f, 0.01f });
			lightGroup->SetPointLightColor(lightIndex, { 1,0.5f,0.5f });
			ParticleManager::Add(particleProp);
		}
	}
	else { model->SetAnbient({ 0,0,0 }); }
	model->Update();
}

void Candle::Draw()
{
	model->Draw(worldTransform);
}

void Candle::OnCollision(RayCollider* rayCollider)
{
	if (!Input::GetInstance()->IsTrigger(Mouse::Left)) { return; }
	if (Length(rayCollider->GetWorldPosition() - worldTransform.GetWorldPosition()) < 8.0f)
	{
		isLight = !isLight;
		particleTimer = 60;
		playerPos = rayCollider->GetWorldPosition();
	}
}

Wall::Wall(Vector3 scale)
{
	SetScale(scale);
}

void Wall::Initialize()
{
	model = Model::Create("cube", true);
	worldTransform.Initialize();
}

void Wall::Update()
{
	worldTransform.Update();
}