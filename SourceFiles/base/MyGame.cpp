#include "MyGame.h"
#include "Model.h"
#include "ImGuiManager.h"
#include "UIDrawer.h"
#include "Audio.h"

void MyGame::Initialize()
{
	Framework::Initialize();
	sceneManager->ChangeScene(Scene::Select, false);
	Model::StaticInitialize();
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		// 平行光源無効化
		Model::GetLightGroup()->SetDirLightActive(i, false);
	}
	ParticleManager::Initialize();
	ParticleGroup particleGroup;
	particleGroup.Initialize("Particle.png");
	ParticleManager::Add(particleGroup);
	particleGroup.Initialize("Particle2.png");
	ParticleManager::Add(particleGroup);
	UIDrawer::LoadAll();
	ImGuiManager::Initialize();
	Audio::StaticInitialize();
	postEffect = std::make_unique<PostEffect>();
	postEffect->Initialize();
}

void MyGame::Update()
{
	ImGuiManager::Begin();
	Framework::Update();
	Model::StaticUpdate();
	ParticleManager::Update();
	UIDrawer::Update();
	ImGuiManager::End();
}

void MyGame::Draw()
{
	postEffect->PreDrawScene();
	sceneManager->Draw();
	ParticleManager::Draw();
	UIDrawer::Draw();
	postEffect->PostDrawScene();
	
	dxCommon->PreDraw();
	postEffect->Draw();
	ImGuiManager::Draw();
	dxCommon->PostDraw();
}

void MyGame::Finalize()
{
	ImGuiManager::Finalize();
	Audio::Finalize();
	Framework::Finalize();
}