#include "ClearScene.h"

void ClearScene::Initialize()
{
	viewProjection.Initialize();
	debugCamera.Initialize();
	Model::SetViewProjection(&debugCamera.GetViewProjection());
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
	// パーティクル
	TrackParticle::AddProp addProp =
	{
		&playerTrans_[0],
		playerTrans_[0].translation + Vector3(0,0.3f),
		{0,0.01f,0},{0,0.0005f,0},
		0.025f,0.001f,0,40,0.8f
	};

	ParticleManager::GetParticleGroup(0)->Add(addProp);
	debugCamera.Update();
	for(int i = 0;i < 4;i++){
		models_object[i]->Update();
	}
	for (int i = 0; i < 3; i++) {
		playerTrans_[i].Update();
	}
	worldTransform_.Update();
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

