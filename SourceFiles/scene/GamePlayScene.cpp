#include "GamePlayScene.h"
#include <imgui.h>
#include "ImGuiManager.h"
#include "WindowsAPI.h"
#include "SceneManager.h"

UINT16 UIBox::uiBoxNum = 0;

void GamePlayScene::Initialize()
{
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		//Model::GetLightGroup()->SetDirLightActive(i, false);
	}
	input = Input::GetInstance();
	debugCamera.Initialize({0},200.0f);
	stage.Initialize();
	// 燭台のUI
	// 描画状態初期化
	for (size_t i = 5; i < 13; i++) { UIDrawer::GetUI(i)->SetIsInvisible(true); }
	for (size_t i = 0; i < Candle::GetLightNum(); i++)
	{
		Sprite* candleUI = nullptr;
		candleUI = UIDrawer::GetUI(5 + i);
		candleUI->SetIsInvisible(false);
		candleUI->SetPosition({ candleUI->GetSize().x * i + 32,77 });
		candleUI->SetColor({ 1,1,1,0.5f });
	}
	// 鍵のUI
	// 描画状態初期化
	UIDrawer::GetUI(16)->SetIsInvisible(true);
	for (size_t i = 18; i < 24; i++) { UIDrawer::GetUI(i)->SetIsInvisible(true); }
	for (size_t i = 0; i < KeyLock::GetKeyNum(); i++)
	{
		Sprite* keyUI = nullptr;
		if (KeyLock::GetKeyNum() == 1) { keyUI = UIDrawer::GetUI(16); }
		else { keyUI = UIDrawer::GetUI(18 + i); }
		keyUI->SetIsInvisible(false);
		keyUI->SetPosition({ keyUI->GetSize().x * i - 75 + 32, 157 });
		keyUI->SetColor({ 1,1,1,0.5f });
	}

	Model::SetViewProjection(&debugCamera.GetViewProjection());
	player.Initialize(stage.GetStartPos(), stage.GetStartRot());
	// BlockクラスにPlayerのポインタを送る
	Block::SetPlayerAddress(&player);
	//ステージ開始のカメラの初期位置
	viewProjection.target = stage.GetDoorPos();
	viewProjection.eye = stage.GetDoorPos() + Vector3{ 0,10,-15 };
	viewProjection.farZ = 1500.0f;
	viewProjection.Initialize();

	uiBoxes[0].Initialize({ -69,4,85 }, { 15,5,15 }, 0);
	uiBoxes[1].Initialize({ 80,0,65 }, { 10,4,15 }, 0);
	uiBoxes[2].Initialize({ 30,0,80 }, { 30,4,40 }, 13);
}

void GamePlayScene::Update()
{
	player.Update();
	debugCamera.Update();
	stage.Update();
	// リトライ
	if (input->IsTrigger(Key::R) || input->IsTrigger(JoyPad::Menu)) { sceneManager_->ChangeScene(Scene::Play); }
	// UIの調整
	if (UIUpdate) { (this->*UIUpdate)(); }
	for (auto& uiSphere : uiBoxes) { uiSphere.Update(); }
	if (input->IsTrigger(Key::_0))
	{
		sceneManager_->ChangeScene(Scene::Title);
		Stage::SetStageNum(0);
	}
	if (input->IsTrigger(Mouse::Right)) { Model::SetViewProjection(&debugCamera.GetViewProjection()); }
}

// 火を変える操作説明
void GamePlayScene::UI_Dark()
{
	// プレイヤーが一定より手前ならスキップ
	if (player.GetWorldPosition().z < -8.0f) { return; }
	// スプライトの取得
	ui = UIDrawer::GetUI((size_t)0 + input->IsConnectGamePad());
	// スプライトの設定
	ui->SetPosition(WindowsAPI::WIN_SIZE / 2.0f);
	ui->SetColor({ 0,0,0,1 });
	ui->SetIsInvisible(false);
	// プレイヤーの火が青炎なら
	if (player.IsBlueFire())
	{
		// UIを消して操作説明を終わる
		ui->SetIsInvisible(true);
		UIUpdate = nullptr;
	}
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	player.Draw();
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
