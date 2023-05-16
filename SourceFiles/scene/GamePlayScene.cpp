#include "GamePlayScene.h"
#include <imgui.h>
#include "ImGuiManager.h"
#include "WindowsAPI.h"
#include "SceneManager.h"

void GamePlayScene::Initialize()
{
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		Model::GetLightGroup()->SetDirLightActive(i, false);
	}
	input = Input::GetInstance();
	debugCamera.Initialize({144},200.0f);
	Model::SetViewProjection(&debugCamera.GetViewProjection());
	stage.Initialize();
	// êCë‰ÇÃUI
	// ï`âÊèÛë‘èâä˙âª
	for (size_t i = 5; i < 13; i++) { UIDrawer::GetUI(i)->SetIsInvisible(true); }
	for (size_t i = 0; i < Candle::GetLightNum(); i++)
	{
		Sprite* candleUI = nullptr;
		candleUI = UIDrawer::GetUI(5 + i);
		candleUI->SetIsInvisible(false);
		candleUI->SetPosition({ candleUI->GetSize().x * i + 32,85 });
		candleUI->SetColor({ 1,1,1,0.5f });
	}
	// åÆÇÃUI
	// ï`âÊèÛë‘èâä˙âª
	UIDrawer::GetUI(16)->SetIsInvisible(true);
	for (size_t i = 18; i < 24; i++) { UIDrawer::GetUI(i)->SetIsInvisible(true); }
	for (size_t i = 0; i < KeyLock::GetKeyNum(); i++)
	{
		Sprite* keyUI = nullptr;
		if (KeyLock::GetKeyNum() == 1) { keyUI = UIDrawer::GetUI(16); }
		else { keyUI = UIDrawer::GetUI(18 + i); }
		keyUI->SetIsInvisible(false);
		keyUI->SetPosition({ keyUI->GetSize().x * i + 32, 165 });
		keyUI->SetColor({ 1,1,1,0.5f });
	}
}

void GamePlayScene::Update()
{
	debugCamera.Update();
	stage.Update();
	// ÉäÉgÉâÉC
	if (input->IsTrigger(Key::R)) { sceneManager_->ChangeScene(Scene::Play); }
	if (input->IsTrigger(Mouse::Right)) { Model::SetViewProjection(&debugCamera.GetViewProjection()); }
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	stage.Draw();
	Model::PostDraw();
}