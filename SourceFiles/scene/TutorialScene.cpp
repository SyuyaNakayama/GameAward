#include "TutorialScene.h"
#include <imgui.h>

UINT16 UIBox::uiBoxNum = 0;

void TutorialScene::Initialize()
{
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		Model::GetLightGroup()->SetDirLightActive(i, false);
	}

	Stage::SetStageNum((int)Stage::StageNum::Tutorial);
	stage.Initialize();

	uiBoxes[0].Initialize({ -69,4,85 }, { 15,5,15 }, 0);
	uiBoxes[1].Initialize({ 80,0,65 }, { 10,4,15 }, 0);
	uiBoxes[2].Initialize({ 30,0,80 }, { 30,4,40 }, 0);

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

	uiBoxes[0].Initialize({ -33,9,-60 }, { 12,10,15 }, (UINT16)UIType::Tutorial::tutorialText1);	//移動＆視点移動
	uiBoxes[1].Initialize({ -60,9,-60 }, { 15,10,15 }, (UINT16)UIType::Tutorial::tutorialText2);	//近くの燭台を灯そう
	uiBoxes[2].Initialize({ -60,9,-21 }, { 15,10,3 }, (UINT16)UIType::Tutorial::tutorialText3);		//ジャンプ
	uiBoxes[3].Initialize({ -60,9,-21 }, { 15,10,12 }, (UINT16)UIType::Tutorial::tutorialText4);	//HPが減るよ
	uiBoxes[4].Initialize({ -60,9,0 }, { 15,10,9 }, (UINT16)UIType::Tutorial::tutorialText5);		//新たな燭台を灯そう
	uiBoxes[5].Initialize({ -60,9,21 }, { 15,10,12 }, (UINT16)UIType::Tutorial::tutorialText6);		//火を切り替える
	uiBoxes[6].Initialize({ -48,9,60 }, { 9,10,15 }, (UINT16)UIType::Tutorial::tutorialText7);		//火の色によって変わるよ
	uiBoxes[7].Initialize({ -24,9,60 }, { 9,10,15 }, (UINT16)UIType::Tutorial::tutorialText8);		//スイッチを移動しよう
	uiBoxes[8].Initialize({ 18,9,60 }, { 9,10,15 }, (UINT16)UIType::Tutorial::tutorialText9);		//スイッチをは一つだけじゃない
	uiBoxes[9].Initialize({ 60,9,0 }, { 15,10,9 }, (UINT16)UIType::Tutorial::tutorialText10);		//上へ行きたい
	uiBoxes[10].Initialize({ -6,31,60 }, { 9,10,15 }, (UINT16)UIType::Tutorial::tutorialText11);	//鍵を取得してみよう
	uiBoxes[11].Initialize({ 18,31,60 }, { 9,10,15 }, (UINT16)UIType::Tutorial::tutorialText12);	//鍵の欠片もあるよ
	uiBoxes[12].Initialize({ 42,31,60 }, { 9,10,15 }, (UINT16)UIType::Tutorial::tutorialText13);	//鍵を使ってみよう
	uiBoxes[13].Initialize({ 36,31,24 }, { 21,10,15 }, (UINT16)UIType::Tutorial::tutorialText14);	//あのドアがゴールだ

}

void TutorialScene::Update()
{
	stage.Update();
	// UIの調整
	if (UIUpdate) { (this->*UIUpdate)(); }
	for (auto& uiSphere : uiBoxes) { uiSphere.Update(); }
}

// 火を変える操作説明
void TutorialScene::UI_Dark()
{
	Player* pPlayer = stage.GetPlayer();
	// プレイヤーが一定より手前ならスキップ
	if (pPlayer->GetWorldPosition().z < -8.0f) { return; }
	// スプライトの取得
	ui = UIDrawer::GetUI((size_t)UIType::Tutorial::LightChange + input->IsConnectGamePad());
	// スプライトの設定
	ui->SetPosition(WindowsAPI::WIN_SIZE / 2.0f);
	ui->SetColor({ 0,0,0,1 });
	ui->SetIsInvisible(false);
	// プレイヤーの火が青炎なら
	if (pPlayer->IsBlueFire())
	{
		// UIを消して操作説明を終わる
		ui->SetIsInvisible(true);
		UIUpdate = nullptr;
	}
}

void TutorialScene::Draw()
{
	Model::PreDraw();
	stage.Draw();
	Model::PostDraw();
}

void UIBox::Initialize(Vector3 pos, Vector3 rad, UINT16 uiIndex)
{
	if (Stage::GetStageNum() != (int)Stage::StageNum::Tutorial) { return; }
	worldTransform.Initialize();
	worldTransform.translation = pos;
	worldTransform.scale = rad;
	worldTransform.Update();
	ui = UIDrawer::GetUI(uiIndex + Input::GetInstance()->IsConnectGamePad());
	collisionAttribute = CollisionAttribute::UI;
	collisionMask = CollisionMask::UI;
	index = uiBoxNum++;
}

void UIBox::Update()
{
	if (Stage::GetStageNum() != (int)Stage::StageNum::Tutorial) { return; }
	ui->SetIsInvisible(true);
}

void UIBox::OnCollision(BoxCollider* collider)
{
	if (Stage::GetStageNum() != (int)Stage::StageNum::Tutorial) { return; }
	Player* pPlayer = dynamic_cast<Player*>(collider);
	if (!pPlayer) { return; }
	if (pPlayer->IsBlueFire()) { return; }
	if (index == 2)
	{
		if (Input::GetInstance()->IsTrigger(Key::Return)) { isOpeEnd = true; }
	}
	if (isOpeEnd) { return; }
	ui->SetIsInvisible(false);
	ui->SetPosition(To2DVector(worldTransform.GetWorldPosition() + Vector3(0, -10, 0)));
}