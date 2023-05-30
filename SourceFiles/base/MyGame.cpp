#include "MyGame.h"
#include "Model.h"
#include "UIDrawer.h"
#include "AudioManager.h"

void MyGame::Initialize()
{
	Framework::Initialize();
	sceneManager->ChangeScene(Scene::Play, false);
	Model::StaticInitialize();
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		// 平行光源無効化
		Model::GetLightGroup()->SetDirLightActive(i, false);
	}
	ParticleManager::Initialize();
	ParticleManager::AddParticleGroup("Particle.png");
	ParticleManager::AddParticleGroup("Particle2.png");
	UIDrawer::LoadAll();
	Audio::StaticInitialize();
	AudioManager::LoadAll();
}

void MyGame::Update()
{
	Framework::Update();
	Model::StaticUpdate();
	ParticleManager::Update();
	AudioManager::SEUpdate();
	UIDrawer::Update();
}

void MyGame::Draw()
{
	dxCommon->PreDraw();
	sceneManager->Draw();
	ParticleManager::Draw();
	UIDrawer::Draw();
	dxCommon->PostDraw();
}

void MyGame::Finalize()
{
	Audio::Finalize();
	Framework::Finalize();
}