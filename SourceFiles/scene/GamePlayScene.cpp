#include "GamePlayScene.h"
#include <imgui.h>
#include "ImGuiManager.h"
#include "WindowsAPI.h"

void GamePlayScene::Initialize()
{
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		Model::GetLightGroup()->SetDirLightActive(i, false);
	}
	input = Input::GetInstance();
	debugCamera.Initialize();
	//WorldTransform::SetViewProjection(&debugCamera.GetViewProjection());
	stage.Initialize();
	// 燭台のUI
	for (size_t i = 0; i < candleUIs.size(); i++)
	{
		candleUIs[i] = UIDrawer::GetUI(5 + i);
	}
	for (size_t i = 0; i < Candle::GetLightNum(); i++)
	{
		candleUIs[i]->SetIsInvisible(false);
		candleUIs[i]->SetPosition({ candleUIs[i]->GetSize().x * i,72 });
		candleUIs[i]->SetColor({ 1,1,1,0.5f });
	}

	player.Initialize(stage.GetStartPos());
	// BlockクラスにPlayerのポインタを送る
	Block::SetPlayerAddress(&player);
	//ステージ開始のカメラの初期位置
	viewProjection.target = stage.GetDoorPos();
	viewProjection.eye = stage.GetDoorPos() + Vector3{ 0,10,-15 };
	viewProjection.farZ = 1500.0f;
	viewProjection.Initialize();
}

void GamePlayScene::StartScene()
{
	if (!Gimmick::GetIsStart()) { return; }
	// カメラ補間開始
	float time = 100; // カメラが移動する時間
	float timeRate = ++timer / time;
	viewProjection.eye = Lerp(stage.GetDoorPos() + Vector3{ 0,10,-15 }, { 0,50,-75 }, timeRate);
	viewProjection.target = Lerp(stage.GetDoorPos(), {}, timeRate);
	if (timer < time) { return; }
	// カメラ補間終了
	timer = 0;
	Gimmick::SetIsStart(false);
	// チュートリアルなら操作説明を有効に
	//if (Stage::GetStageNum() == (UINT16)Stage::StageNum::Tutorial) { UIUpdate = &GamePlayScene::UI_Camera; }
}

void GamePlayScene::Update()
{
	//StartScene();
	player.Update();
	debugCamera.Update();
	stage.Update();
	// UIの調整
	if (UIUpdate) { (this->*UIUpdate)(); }
	// 燭台のUIの色変更
	size_t lightedNum = Candle::GetLightedNum();
	if (lightedNum != 0)
	{
		candleUIs[lightedNum - 1]->SetColor({ 1,1,1,1 });
	}

	if (WorldTransform::GetViewProjection() != &viewProjection && input->IsTrigger(Mouse::Right))
	{
		WorldTransform::SetViewProjection(&viewProjection);
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
