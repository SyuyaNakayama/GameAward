#include "GamePlayScene.h"
#include "SceneManager.h"

void GamePlayScene::Initialize()
{
	AudioManager::Stop(SEName::BlockMove);
	AudioManager::GetAudio(SEName::BlockMove)->SetPlayPosition(0);
	stage.Initialize();
	UIReset();
	UIDrawer::GetUI((size_t)UIType::Play::Reset + !input->IsConnectGamePad())->SetIsInvisible(true);
	AudioManager::Stop(SEName::DoorOpen);
}

void GamePlayScene::Update()
{
	AudioManager::Play(BGMName::Play);
	stage.Update();
	if (input->IsTrigger(Key::R) || input->IsTrigger(JoyPad::X)) { sceneManager_->ChangeScene(Scene::Play); } // リトライ
	if (input->IsTrigger(Key::T)) { sceneManager_->ChangeScene(Scene::Select); } // セレクトへ
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	stage.Draw();
	Model::PostDraw();
}