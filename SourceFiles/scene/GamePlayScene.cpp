#include "GamePlayScene.h"
#include <imgui.h>
#include "SpriteCommon.h"
#include "CollisionManager.h"
#include "ImGuiManager.h"

void GamePlayScene::Initialize()
{
	uiDrawer.SetScene(Scene::Play);
	lightGroup = LightGroup::Create();
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		lightGroup->SetDirLightActive(i, false);
	}
	debugCamera.Initialize();
	WorldTransform::SetViewProjection(&debugCamera.GetViewProjection());
	Model::SetLightGroup(lightGroup.get());

	skydome.Initialize(100.0f);
	stage.Initialize(0);
	player.Initialize();

	//ステージ開始のカメラの初期位置
	viewProjection.target = stage.GetDoorPos();
	viewProjection.eye = stage.GetDoorPos() + Vector3{ 0,10,-15 };
	viewProjection.farZ = 1500.0f;
}

void GamePlayScene::StartScene()
{
	float time = 100; // カメラが移動する時間

	if (Gimmick::GetIsStart())
	{
		timer++;
		float timeRate = timer / time;
		viewProjection.eye = Lerp(stage.GetDoorPos() + Vector3{ 0,10,-15 }, { 0,50,-50 }, timeRate);
		viewProjection.target = Lerp(stage.GetDoorPos(), {}, timeRate);
		if (timer >= time)
		{
			timer = 0;
			Gimmick::SetIsStart(false);
		}
	}
}

void GamePlayScene::Update()
{
	StartScene();
	skydome.Update();
	uiDrawer.Update();
	viewProjection.Update();
	player.Update();
	debugCamera.Update();
	stage.Update();
	CollisionManager::CheckAllCollisions();
	lightGroup->Update();

	if (WorldTransform::GetViewProjection() != &viewProjection && input->IsTrigger(Mouse::Right) && !player.IsCameraChange())
	{
		WorldTransform::SetViewProjection(&viewProjection);
	}
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	//skydome.Draw();
	player.Draw();
	stage.Draw();
	Model::PostDraw();

	uiDrawer.Draw();
}