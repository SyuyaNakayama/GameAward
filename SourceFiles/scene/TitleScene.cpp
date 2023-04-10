#include "TitleScene.h"
#include "ImGuiManager.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "CollisionManager.h"

void TitleScene::Initialize()
{
	debugCamera.Initialize();
	WorldTransform::SetViewProjection(&viewProjection);
	WorldTransform::SetViewProjection(&debugCamera.GetViewProjection());
	lightGroup = Model::GetLightGroup();
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		//lightGroup->SetDirLightActive(i, false);
	}
	viewProjection.eye = { 2.5f,1,-7 };
	viewProjection.target = { 2.5f,1 };
	viewProjection.Initialize();
	stage.Initialize();
	player.Initialize(stage.GetStartPos());
	UIUpdate = &TitleScene::UI_Move;
}

void TitleScene::Update()
{
	debugCamera.Update();
	player.Update();
	stage.Update();
	if (UIUpdate) { (this->*UIUpdate)(); }

	if (input->IsTrigger(Key::K))
	{
		ParticleManager::Clear();
		sceneManager_->SetNextScene(Scene::Play);
	}
}

void TitleScene::UI_Move()
{
	Sprite* ui = UIDrawer::GetUI((size_t)0 + input->IsConnectGamePad());
	ui->SetIsInvisible(false);
	ui->SetPosition(WindowsAPI::WIN_SIZE / 2.0f);

	if (input->IsInput(Key::W) || input->IsInput(Key::A) || input->IsInput(Key::S) || input->IsInput(Key::D))
	{
		if (uiMoveTimer.CountDown())
		{
			ui->SetIsInvisible(true);
			UIUpdate = &TitleScene::UI_Camera;
		}
	}
}

void TitleScene::UI_Camera()
{
	Sprite* ui = UIDrawer::GetUI((size_t)4 + input->IsConnectGamePad());
	ui->SetIsInvisible(false);
	ui->SetPosition(WindowsAPI::WIN_SIZE / 2.0f);

	mouseMoveX += std::abs(input->GetMouseMove().lX);
	if (mouseMoveX >= 5000)
	{
		ui->SetIsInvisible(true);
		UIUpdate = &TitleScene::UI_Camera;
	}
}

void TitleScene::Draw()
{
	Model::PreDraw();
	player.Draw();
	stage.Draw();
	Model::PostDraw();
}