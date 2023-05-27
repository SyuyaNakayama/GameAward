#include "TitleScene.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include "UIDrawer.h"
#include "WindowsAPI.h"
#include "SceneManager.h"

void SelectUI::Initialize()
{
	lerpRateRad = 0;
	offset = {};
	sprite->SetPosition(pos + offset);
}

void SelectUI::Update()
{
	lerpRateRad += 5;
	offset.x = (-std::cos(lerpRateRad) + 1.0f) * 50.0f;
	sprite->SetPosition(pos + offset);
}

bool SelectUI::PreFade()
{
	lerpRateRad += 90;
	sprite->SetColor({ 1,1,1,std::sin(lerpRateRad) });
	return fadeTimer.CountDown();
}

void TitleScene::Initialize()
{
	Stage::SetStageNum(0);
	stage.Initialize();
	titleUI = UIDrawer::GetUI(0);
	titleUI->SetPosition({ WindowsAPI::WIN_SIZE.x / 2.0f,250.0f });
	for (size_t i = 0; i < 2; i++)
	{
		selectUIs[i].pos = { WindowsAPI::WIN_SIZE.x / 2.0f,550.0f + (float)i * 150.0f };
		selectUIs[i].sprite = UIDrawer::GetUI(i + 1);
		selectUIs[i].SetSpritePos();
	}
	nextUI = UIDrawer::GetUI((size_t)3 + input->IsConnectGamePad());
	nextUI->SetPosition({ WindowsAPI::WIN_SIZE.x / 2.0f ,850.0f });
	nextUI->SetIsInvisible(false);
	if (!input->IsConnectGamePad())
	{
		nextUI->SetTextureLeftTop({ 16,0 });
		nextUI->SetTextureSize({ 200,100 });
		nextUI->SetSize({ 200,100 });
	}
	else
	{
		nextUI->SetTextureLeftTop({ 32,0 });
		nextUI->SetTextureSize({ 96,100 });
		nextUI->SetSize({ 96,100 });
	}
}

void TitleScene::Update()
{
	AudioManager::Play(BGMName::Select);
	stage.Update();
	padStick = input->GetPadState().LNormalize();
	// 次へ進めるボタン表示の点滅
	float alpha = (std::cos(nextUIAlphaRate) + 1.0f) * 0.5f;
	nextUIAlphaRate += 5;
	nextUI->SetColor({ 1,1,1,alpha });
	// チュートリアルかゲームプレイか選択する処理
	(this->*Select)();
}

void TitleScene::ToPlay()
{
	selectUIs[0].Update();
	if (input->IsTrigger(Key::S) || input->IsTrigger(Key::Down) || padStick.y >= 0.2f)
	{
		AudioManager::Play(SEName::Cursor,{0,0,-20});
		selectUIs[0].Initialize();
		Select = &TitleScene::ToTutorial;
	}
	if (input->IsTrigger(Key::Space) || input->IsTrigger(JoyPad::A))
	{
		AudioManager::Play(SEName::Button, { 0,0,-20 });
		selectUIs[0].Initialize();
		Select = &TitleScene::PreFadeToPlay;
	}
}

void TitleScene::ToTutorial()
{
	selectUIs[1].Update();
	if (input->IsTrigger(Key::W) || input->IsTrigger(Key::Up) || padStick.y <= -0.2f)
	{
		AudioManager::Play(SEName::Cursor, { 0,0,-20 });
		selectUIs[1].Initialize();
		Select = &TitleScene::ToPlay;
	}
	if (input->IsTrigger(Key::Space) || input->IsTrigger(JoyPad::A))
	{
		AudioManager::Play(SEName::Button, { 0,0,-20 });
		selectUIs[1].Initialize();
		Select = &TitleScene::PreFadeToTutorial;
	}
}

void TitleScene::PreFadeToPlay()
{
	nextUI->SetColor({ 1,1,1,0 }); 
	if (selectUIs[0].PreFade()) { sceneManager_->ChangeScene(Scene::Select, false); }
}

void TitleScene::PreFadeToTutorial()
{
	nextUI->SetColor({ 1,1,1,0 });
	if (selectUIs[1].PreFade()) { sceneManager_->ChangeScene(Scene::Tutorial); }
}

void TitleScene::Draw()
{
	Model::PreDraw();
	stage.Draw();
	Model::PostDraw();
}