#include "MyGame.h"
#include "Model.h"
#include "ImGuiManager.h"
#include "UIDrawer.h"
#include "ParticleManager.h"

void MyGame::Initialize()
{
	Framework::Initialize();
	sceneManager_->SetNextScene(Scene::Play, false);
	Model::InitializeGraphicsPipeline();
	ParticleManager::Initialize();
	UIDrawer::LoadAll();
	postEffect = std::make_unique<PostEffect>();
	postEffect->Initialize();
}

void MyGame::Finalize() { Framework::Finalize(); }
void MyGame::Update() 
{
	Framework::Update(); 
	ParticleManager::Update();
}

void MyGame::Draw()
{
	postEffect->PreDrawScene();
	sceneManager_->Draw();
	postEffect->PostDrawScene();

	dxCommon->PreDraw();
	postEffect->Draw();
	ParticleManager::Draw();
	ImGuiManager::Draw();
	dxCommon->PostDraw();
}