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
	viewProjection.eye = { 0,300,0 };
	viewProjection.up = { 0,0,1 };
	viewProjection.farZ = 1500.0f;
	skydome.Initialize(100.0f);
	Sprite* skydomeModelSprite = skydome.GetModelSprite();
	skydomeModelSprite->SetColor({ 0,0,0,1 });
	stage.Initialize();
	player.Initialize(lightGroup.get());
}

void GamePlayScene::Update()
{
	skydome.Update();
	uiDrawer.Update();
	viewProjection.Update();
	debugCamera.Update();
	stage.Update();
	player.Update();
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
	stage.Draw();
	player.Draw();
	Model::PostDraw();

	uiDrawer.Draw();
}