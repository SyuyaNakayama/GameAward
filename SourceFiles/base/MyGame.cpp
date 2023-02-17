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
	postEffect->Initialize(SpriteCommon::GetInstance()->LoadTexture("white1x1.png"));
	postEffect->SetSize({ 300,300 });
	postEffect->Update();
}

void MyGame::Finalize() { Framework::Finalize(); }
void MyGame::Update() { Framework::Update(); }

void MyGame::Draw()
{
	dxCommon->PreDraw();
	postEffect->Draw();
	//sceneManager_->Draw();
	ImGuiManager::Draw();
	dxCommon->PostDraw();
}