#include "GamePlayScene.h"
#include <imgui.h>
#include "SpriteCommon.h"


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

	//viewProjection.eye = { 0,300,0 };
	/*viewProjection.eye = { 0,50,-50 };
	viewProjection.up = { 0,0,1 };
	viewProjection.farZ = 1500.0f;*/
	
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
#pragma region eyeとtergetの移動の値セット
	Vector3 a = stage.GetDoorPos() + Vector3{ 0,10,-15 };
	Vector3 b;
	b.x = (0 - a.x) / time;
	b.y = (50 - a.y) / time;
	b.z = (-50 - a.z) / time;

	Vector3 c = stage.GetDoorPos();
	Vector3 d;
	d.x = (0 - c.x) / time;
	d.y = (0 - c.y) / time;
	d.z = (0 - c.z) / time;
#pragma endregion
	if (gimmick->GetIsStart() == true)
	{
		timer++;
		viewProjection.eye += b;
		viewProjection.target += d;
		if(timer >= time)
		{
			timer = 0;
			isStart = false;
			gimmick->SetIsStart(isStart);
			viewProjection.eye = { 0,50,-50 };
			viewProjection.target = { 0,0,0 };
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