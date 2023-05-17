#include "TutorialScene.h"
#include "SceneManager.h"
#include <imgui.h>

UINT16 UIBox::uiBoxNum = 0;

void UIReset();

void TutorialScene::Initialize()
{
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		Model::GetLightGroup()->SetDirLightActive(i, false);
	}

	Stage::SetStageNum((int)Stage::StageNum::Tutorial);
	stage.Initialize();

	UIReset();

	uiBoxes[0].Initialize({ -33,9,-60 }, { 10,10,15 }, (size_t)UIType::Tutorial::tutorialText1);	//移動＆視点移動
	uiBoxes[1].Initialize({ -60,9,-60 }, { 14,10,13 }, (size_t)UIType::Tutorial::tutorialText2);	//近くの燭台を灯そう
	uiBoxes[2].Initialize({ -60,9,-42 }, { 15,10,3 }, (size_t)UIType::Tutorial::tutorialText3);		//ジャンプ
	uiBoxes[3].Initialize({ -60,9,-21 }, { 15,10,11 }, (size_t)UIType::Tutorial::tutorialText4);	//HPが減るよ
	uiBoxes[4].Initialize({ -60,9,0 }, { 15,10,8 }, (size_t)UIType::Tutorial::tutorialText5);		//新たな燭台を灯そう
	uiBoxes[5].Initialize({ -60,9,21 }, { 15,10,11 }, (size_t)UIType::Tutorial::tutorialText6);	//火を切り替える
	uiBoxes[6].Initialize({ -48,9,60 }, { 9,10,15 }, (size_t)UIType::Tutorial::tutorialText7);		//火の色によって変わるよ
	uiBoxes[7].Initialize({ -24,9,60 }, { 9,10,15 }, (size_t)UIType::Tutorial::tutorialText8);		//スイッチを移動しよう
	uiBoxes[8].Initialize({ 18,9,60 }, { 9,10,15 }, (size_t)UIType::Tutorial::tutorialText9);		//スイッチをは一つだけじゃない
	uiBoxes[9].Initialize({ 60,9,0 }, { 15,10,9 }, (size_t)UIType::Tutorial::tutorialText10);		//上へ行きたい
	uiBoxes[10].Initialize({ -6,31,60 }, { 9,10,15 }, (size_t)UIType::Tutorial::tutorialText11);	//鍵を取得してみよう
	uiBoxes[11].Initialize({ 18,31,60 }, { 9,10,15 }, (size_t)UIType::Tutorial::tutorialText12);	//鍵の欠片もあるよ
	uiBoxes[12].Initialize({ 42,31,60 }, { 9,10,15 }, (size_t)UIType::Tutorial::tutorialText13);	//鍵を使ってみよう
	uiBoxes[13].Initialize({ 36,31,24 }, { 21,10,15 }, (size_t)UIType::Tutorial::tutorialText14);	//あのドアがゴールだ

}

void TutorialScene::Update()
{
	stage.Update();
	if (input->IsTrigger(Key::R)) { sceneManager_->ChangeScene(Scene::Play); } // リトライ
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
	ui->SetPosition({ 1940/2,100 });
}