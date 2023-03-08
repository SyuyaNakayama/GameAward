#include "GamePlayScene.h"
#include <imgui.h>
#include "SpriteCommon.h"
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
	stage.Initialize();
	player.Initialize(lightGroup.get());
	input = Input::GetInstance();
	gimmick = std::make_unique<Door>();
	gimmick->Initialize();

	//ステージ開始のカメラの初期位置
	viewProjection.target = stage.GetDoorPos();
	viewProjection.eye = stage.GetDoorPos() + Vector3{ 0,10,-15 };
	viewProjection.farZ = 1500.0f;
}

void GamePlayScene::StartScene()
{
	float time = 100;

	if (gimmick->GetIsStart() == true)
	{
		timer++;
		viewProjection.eye = Lerp(stage.GetDoorPos() + Vector3{ 0,10,-15 }, { 0,50,-50 }, timer / time);
		viewProjection.target = Lerp(stage.GetDoorPos(), {}, timer / time);
		if(timer >= time)
		{
			timer = 0;
			gimmick->SetIsStart(false);
		}
	}
	ImGui::Text("DisST: %d", gimmick->GetIsStart());
}

void GamePlayScene::Update()
{
	StartScene();

	viewProjection.Update();
	skydome.Update();
	uiDrawer.Update();
	player.Update();
	debugCamera.Update();
	stage.Update();
	lightGroup->Update();

	ImGuiManager::PrintVector("vpEye", viewProjection.eye);
	ImGuiManager::PrintVector("vpTarget", viewProjection.target);

	if (WorldTransform::GetViewProjection() != &viewProjection && input->IsTrigger(Mouse::Right) && !player.IsCameraChange())
	{
		WorldTransform::SetViewProjection(&viewProjection);
	}
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	skydome.Draw();
	player.Draw();
	stage.Draw();
	Model::PostDraw();

	uiDrawer.Draw();
}