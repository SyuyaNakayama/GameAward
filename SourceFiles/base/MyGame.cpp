#include "MyGame.h"
#include "Model.h"
#include "ImGuiManager.h"
#include "UIDrawer.h"

void MyGame::Initialize()
{
	Framework::Initialize();
	sceneManager_->SetNextScene(Scene::Play, false);
	Model::InitializeGraphicsPipeline();
	UIDrawer::LoadAll();
	postEffect = std::make_unique<PostEffect>();
	postEffect->Initialize();
	postEffect->SetSize({ 300,300 });
	postEffect->Update();
}

void MyGame::Finalize() { Framework::Finalize(); }
void MyGame::Update() { Framework::Update(); }

void MyGame::Draw()
{
	postEffect->PreDrawScene();
	sceneManager_->Draw();
	postEffect->PostDrawScene();

	dxCommon->PreDraw();
	postEffect->Draw();
	ImGuiManager::Draw();
	dxCommon->PostDraw();
}