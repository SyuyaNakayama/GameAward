#include "GamePlayScene.h"
#include "SceneManager.h"

void GamePlayScene::Initialize()
{
	debugCamera.Initialize({ 0 }, 100.0f);
	Model::SetViewProjection(&debugCamera.GetViewProjection());
	stage.Initialize();
	UIReset();
	UIDrawer::GetUI((size_t)UIType::Play::Reset + !input->IsConnectGamePad())->SetIsInvisible(true);
	AudioManager::Stop(SEName::DoorOpen);
}

void GamePlayScene::Update()
{
	AudioManager::Play(BGMName::Play);
	debugCamera.Update();
	stage.Update();
	if (input->IsTrigger(Key::R) || input->IsTrigger(JoyPad::X)) { sceneManager_->ChangeScene(Scene::Play); } // ���g���C
	if (input->IsTrigger(Mouse::Right)) { Model::SetViewProjection(&debugCamera.GetViewProjection()); }
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	stage.Draw();
	Model::PostDraw();
}