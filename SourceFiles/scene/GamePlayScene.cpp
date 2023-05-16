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
	debugCamera.Initialize({ 144 }, 200.0f);
	Model::SetViewProjection(&debugCamera.GetViewProjection());
	stage.Initialize();
	// 燭台のUI
	// 描画状態初期化
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
	// 鍵のUI
	// 描画状態初期化
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

	/*uiBoxes[0].Initialize({ -69,4,85 }, { 15,5,15 }, 0);
	uiBoxes[1].Initialize({ 80,0,65 }, { 10,4,15 }, 0);
	uiBoxes[2].Initialize({ 30,0,80 }, { 30,4,40 }, 13);*/

	uiBoxes[0].Initialize({ -33,9,-60 }, { 12,10,15 }, 1);//移動＆視点移動
	uiBoxes[1].Initialize({ -60,9,-60 }, { 15,10,15 }, 1);//近くの燭台を灯そう
	uiBoxes[2].Initialize({ -60,9,-21 }, { 15,10,3 }, 14);//ジャンプ
	uiBoxes[3].Initialize({ -60,9,-12 }, { 15,10,21 }, 1);//HPの増減について
	uiBoxes[4].Initialize({ -60,9,21 }, { 15,10,12 }, 0);//青で消える壁
	uiBoxes[5].Initialize({ -48,9,60 }, { 9,10,15 }, 0);//赤で消える壁
	uiBoxes[6].Initialize({ -24,9,60 }, { 9,10,15 }, 1);//スイッチ*1
	uiBoxes[7].Initialize({ 18,9,60 }, { 9,10,15 }, 1);//スイッチ*2
	uiBoxes[8].Initialize({ 60,9,0 }, { 15,10,9 }, 1);//エレベーター
}

void GamePlayScene::Update()
{
	debugCamera.Update();
	stage.Update();
	// リトライ
	if (input->IsTrigger(Key::R)) { sceneManager_->ChangeScene(Scene::Play); }
	if (input->IsTrigger(Mouse::Right)) { Model::SetViewProjection(&debugCamera.GetViewProjection()); }
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	stage.Draw();
	Model::PostDraw();
}