#include "MyGame.h"
#include "Model.h"
#include "ImGuiManager.h"
#include "UIDrawer.h"
#include "ParticleManager.h"
#include "Audio.h"
#include <random>

void MyGame::Initialize()
{
	Framework::Initialize();
	sceneManager->SetNextScene(Scene::Play, false);
	Model::StaticInitialize();
	ParticleManager::Initialize();
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
	// 炎の揺らぎ
	for (size_t i = 0; i < LightGroup::POINT_LIGHT_NUM; i++)
	{
		LightGroup* lightGroup = Model::GetLightGroup();
		if (!lightGroup->GetPointLightActive(i)) { continue; }
		std::random_device rnd;
		std::mt19937 rnddev(rnd());
		std::uniform_real_distribution<float> randRadius(0, 0.6f);
		lightGroup->SetPointLightPos(i, lightGroup->GetPointLightPos(i) + Vector3(randRadius(rnddev), 0, randRadius(rnddev)));
	}
	Model::LightUpdate();
	WorldTransform::CameraUpdate();
	ParticleManager::Update();
	UIDrawer::Update();
	ImGuiManager::End();
}

void MyGame::Draw()
{
	postEffect->PreDrawScene();
	sceneManager->Draw();
	ParticleManager::Draw();
	//UIDrawer::Draw();
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