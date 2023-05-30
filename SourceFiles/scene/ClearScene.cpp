#include "ClearScene.h"
#include "Random.h"
#include "SceneManager.h"

void ClearScene::Initialize()
{
	AudioManager::GetAudio(BGMName::Clear)->SetPlayPosition(0);
	AudioManager::Play(BGMName::Clear);
	viewProjection.Initialize();
	viewProjection.eye = { 15,3.5f,-14.5f };
	viewProjection.target.x = 7.0f;
	Model::SetViewProjection(&viewProjection);
	//スプライト読み込み
	clearUI = UIDrawer::GetUI(0);
	clearUI->SetSize({ 1800 * 0.7,400 * 0.7 });
	clearUI->SetPosition({ WindowsAPI::WIN_SIZE.x / 2.0f,150.0f });

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
	models_player[0] = Model::Create("player_body", true);		//頭
	models_player[1] = Model::Create("player_shoesR", true);	//右足
	models_player[2] = Model::Create("player_shoesL", true);	//左足

	worldTransform_.Initialize();
	worldTransform_.scale = { 1.5f,1.5f,1.5f };
	worldTransform_.Update();
	//プレイヤーのポーズ調整
	playerTrans_[0].translation += {0.0f, 0.5f, 0.0f};
	playerTrans_[1].rotation.x += 30 * PI / 180;
	playerTrans_[2].rotation.x += -30 * PI / 180;
	for (auto& trans : playerTrans_)
	{
		trans.Initialize();
		trans.rotation.y += 90 * PI / 180;
		trans.translation += { 7.0f, -0.5f, 0.0f };
		trans.scale = { 0.5f,0.5f,0.5f };
		trans.Update();
	}

	nextUI = UIDrawer::GetUI((size_t)1 + input->IsConnectGamePad());
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

void ClearScene::Update()
{
	// タイトルへ
	if (input->IsTrigger(Key::Space) || input->IsTrigger(JoyPad::A)) 
	{
		sceneManager_->ChangeScene(Scene::Title); 
		AudioManager::Stop(BGMName::Clear);
	}
	// パーティクル
	TrackParticle::AddProp addProp =
	{
		&playerTrans_[0],
		playerTrans_[0].translation,
		{0,0.01f,0},{0,0.0005f,0},
		0.025f,0.001f,0,40,0.8f
	};
	// 次へ進めるボタン表示の点滅
	float alpha = (std::cos(nextUIAlphaRate) + 1.0f) * 0.5f;
	nextUIAlphaRate += 5;
	nextUI->SetColor({ 1,1,1,alpha });

	ParticleManager::GetParticleGroup(0)->Add(addProp);
	Random_Float rndRadius(-0.1f, 0.1f);
	lightGroup->SetPointLightPos(0, playerTrans_[0].GetWorldPosition() + Vector3(rndRadius(), 0, rndRadius()));
}

void ClearScene::Draw()
{
	Model::PreDraw();
	for (size_t i = 0; i < models_object.size(); i++) { models_object[i]->Draw(worldTransform_); }
	for (size_t i = 0; i < models_player.size(); i++) { models_player[i]->Draw(playerTrans_[i]); }
	Model::PostDraw();
}

