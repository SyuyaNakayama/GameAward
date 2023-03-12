#include "TitleScene.h"
#include "ImGuiManager.h"
#include <random>
#include "ParticleManager.h"
#include "SceneManager.h"

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
	stage.Initialize();
}

void TitleScene::Update()
{
	ImGuiManager::InputVector("viewProjection.eye", viewProjection.eye);
	ImGuiManager::InputVector("viewProjection.target", viewProjection.target);
	uiDrawer.Update();
	debugCamera.Update();
	viewProjection.Update();
	player.Update();
	stage.Update();

	std::random_device rnd;
	std::mt19937 rnddev(rnd());
	std::uniform_real_distribution<float> randRadius(0, 1.0f);
	std::uniform_real_distribution<float> randAngle(-PI / 2.0f, PI / 2.0f);
	DirectionalParticle::AddProp particleProp =
	{
		player.GetWorldTransform().translation,{0,2,0},2,randAngle(rnddev),randRadius(rnddev),60
	};
	ParticleManager::Add(particleProp);

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