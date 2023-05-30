#include "ClearScene.h"
#include "Random.h"

void ClearScene::Initialize()
{
	viewProjection.Initialize();
	debugCamera.Initialize();
	Model::SetViewProjection(&debugCamera.GetViewProjection());

	// ライトグループの設定
	lightGroup = Model::GetLightGroup();
	lightGroup->SetPointLightActive(0, true);
	lightGroup->SetPointLightAtten(0, { 0.2f,0.0f,0.001f });
	lightGroup->SetPointLightColor(0, { 1.0f,0.5f,0.5f });
	//部屋のモデル読み込み
	models_object[0] = Model::Create("yuka");
	models_object[1] = Model::Create("kabe");
	models_object[2] = Model::Create("doorkabe");
	models_object[3] = Model::Create("2door");
	//プレイヤーのモデル読み込み
	models_player[0] = Model::Create("player_body");	//頭
	models_player[1] = Model::Create("player_shoesR");	//右足
	models_player[2] = Model::Create("player_shoesL");	//左足

	for (int i = 0; i < 3; i++) {
		playerTrans_[i].Initialize();
	}
	worldTransform_.Initialize();
	worldTransform_.scale = { 3.0f,3.0f,3.0f };
	//プレイヤーのポーズ調整
	for (int i = 0; i < 3; i++) {
		playerTrans_[i].rotation.y += 90 * PI / 180;
		playerTrans_[i].translation += { 7.0f,-1.0f,0.0f };
	}
	playerTrans_[0].translation += {0.0f, 0.5f, 0.0f};
	playerTrans_[1].rotation.x += 30 * PI / 180;
	playerTrans_[2].rotation.x += -30 * PI / 180;
}

void ClearScene::Update()
{
	debugCamera.Update();
	for(int i = 0;i < 4;i++){
		models_object[i]->Update();
	}
	for (int i = 0; i < 3; i++) {
		playerTrans_[i].Update();
	}
	worldTransform_.Update();
	Random_Float rndRadius(-0.1f, 0.1f);
	lightGroup->SetPointLightPos(0, playerTrans_->GetWorldPosition() + Vector3(rndRadius(), 0, rndRadius()));
}

void ClearScene::Draw()
{
	Model::PreDraw();
	debugCamera.Update();
	for (int i = 0; i < 4; i++){
		models_object[i]->Draw(worldTransform_);
	}
	for (int i = 0; i < 3; i++) {
		models_player[i]->Draw(playerTrans_[i]);
	}
	Model::PostDraw();
}

