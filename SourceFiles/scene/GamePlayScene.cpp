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
		Model::GetLightGroup()->SetDirLightActive(i, false);
	}
	input = Input::GetInstance();
	//debugCamera.Initialize({ 180,50 });
	debugCamera.Initialize({ 350,30 });
	WorldTransform::SetViewProjection(&debugCamera.GetViewProjection());
	stage.Initialize();
	// 燭台のUI
	for (size_t i = 0; i < candleUIs.size(); i++)
	{
		// 描画状態初期化
		UIDrawer::GetUI(5 + i)->SetIsInvisible(true);
	}
	for (size_t i = 0; i < Candle::GetLightNum(); i++)
	{
		candleUIs[i] = UIDrawer::GetUI(5 + i);
		candleUIs[i]->SetIsInvisible(false);
		candleUIs[i]->SetPosition({ candleUIs[i]->GetSize().x * i,40 });
		candleUIs[i]->SetColor({ 1,1,1,0.5f });
	}
	// 鍵のUI
	for (size_t i = 0; i < keyUIs.size(); i++)
	{
		// 描画状態初期化
		UIDrawer::GetUI(16 + i)->SetIsInvisible(true);
	}
	for (size_t i = 0; i < KeyLock::GetKeyNum(); i++)
	{
		keyUIs[i] = UIDrawer::GetUI(16 + i);
		keyUIs[i]->SetIsInvisible(false);
		keyUIs[i]->SetPosition({ keyUIs[i]->GetSize().x - 75, 120 });
		keyUIs[i]->SetColor({ 1,1,1,0.5f });
	}

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
	if (input->IsTrigger(Key::R)) { sceneManager_->SetNextScene(Scene::Play); }
	// UIの調整
	if (UIUpdate) { (this->*UIUpdate)(); }
	// 燭台のUIの色変更
	size_t lightedNum = Candle::GetLightedNum();
	if (lightedNum != 0)
	{
		candleUIs[lightedNum - 1]->SetColor({ 1,1,1,1 });
	}
	// 鍵のUIの色変更
	size_t collectedKeyNum = KeyLock::GetCollectedKeyNum();
	if (collectedKeyNum != 0)
	{
		keyUIs[collectedKeyNum - 1]->SetColor({ 1,1,1,1 });
	}
	for (auto& uiSphere : uiBoxes) { uiSphere.Update(); }
	if (input->IsTrigger(Key::_0)) 
	{
		sceneManager_->SetNextScene(Scene::Title); 
		Stage::SetStageNum(0);
	}
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
