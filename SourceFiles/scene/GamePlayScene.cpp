#include "GamePlayScene.h"
#include <imgui.h>
#include "ImGuiManager.h"
#include "WindowsAPI.h"
#include "SceneManager.h"

void UIReset()
{
	// êCë‰ÇÃUI
	// ï`âÊèÛë‘èâä˙âª
	size_t candleUIIndex = (size_t)UIType::Play::Candle;
	for (size_t i = candleUIIndex; i < candleUIIndex + 8; i++) { UIDrawer::GetUI(i)->SetIsInvisible(true); }
	for (size_t i = 0; i < Candle::GetLightNum(); i++)
	{
		Sprite* candleUI = nullptr;
		candleUI = UIDrawer::GetUI(candleUIIndex + i);
		candleUI->SetIsInvisible(false);
		candleUI->SetPosition({ candleUI->GetSize().x * i + 32,85 });
		candleUI->SetColor({ 1,1,1,0.5f });
	}
	// åÆÇÃUI
	// ï`âÊèÛë‘èâä˙âª
	size_t keyUIIndex = (size_t)UIType::Play::Key;
	size_t keyPartsUIIndex = (size_t)UIType::Play::KeyParts;
	UIDrawer::GetUI(keyUIIndex)->SetIsInvisible(true);
	for (size_t i = keyPartsUIIndex; i < keyPartsUIIndex + 6; i++) { UIDrawer::GetUI(i)->SetIsInvisible(true); }
	for (size_t i = 0; i < KeyLock::GetKeyNum(); i++)
	{
		Sprite* keyUI = nullptr;
		if (KeyLock::GetKeyNum() == 1) { keyUI = UIDrawer::GetUI(keyUIIndex); }
		else { keyUI = UIDrawer::GetUI(keyPartsUIIndex + i); }
		keyUI->SetIsInvisible(false);
		keyUI->SetPosition({ keyUI->GetSize().x * i + 32, 165 });
		keyUI->SetColor({ 1,1,1,0.5f });
	}
}

void GamePlayScene::Initialize()
{
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		Model::GetLightGroup()->SetDirLightActive(i, false);
	}
	input = Input::GetInstance();
	debugCamera.Initialize({ 144 }, 200.0f);
	Model::SetViewProjection(&debugCamera.GetViewProjection());
	stage.Initialize();
	UIReset();
}

void GamePlayScene::Update()
{
	debugCamera.Update();
	stage.Update();
	if (input->IsTrigger(Key::R)) { sceneManager_->ChangeScene(Scene::Play); } // ÉäÉgÉâÉC
	if (input->IsTrigger(Mouse::Right)) { Model::SetViewProjection(&debugCamera.GetViewProjection()); }
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	stage.Draw();
	Model::PostDraw();
}