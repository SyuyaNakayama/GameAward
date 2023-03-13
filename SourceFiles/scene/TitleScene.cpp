#include "TitleScene.h"
#include "ImGuiManager.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "CollisionManager.h"

void TitleScene::Initialize()
{
	uiDrawer.SetScene(Scene::Title);
	debugCamera.Initialize();
	WorldTransform::SetViewProjection(&viewProjection);
	//WorldTransform::SetViewProjection(&debugCamera.GetViewProjection());
	lightGroup = LightGroup::Create();
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		lightGroup->SetDirLightActive(i, false);
	}
	Model::SetLightGroup(lightGroup.get());
	viewProjection.eye = { 2.5f,1,-7 };
	viewProjection.target = { 2.5f,1 };
	player.Initialize();
	stage.Initialize(0);
}

void TitleScene::Update()
{
	uiDrawer.Update();
	debugCamera.Update();
	viewProjection.Update();
	player.Update();
	stage.Update();
	lightGroup->Update();
	CollisionManager::CheckAllCollisions();

	if (input->IsTrigger(Key::K))
	{
		ParticleManager::Clear();
		sceneManager_->SetNextScene(Scene::Play);
	}
}

void TitleScene::Draw()
{
	Model::PreDraw();
	player.Draw();
	stage.Draw();
	Model::PostDraw();

	uiDrawer.Draw();
}