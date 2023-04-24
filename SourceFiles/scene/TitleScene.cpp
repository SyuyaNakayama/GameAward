#include "TitleScene.h"
#include "ImGuiManager.h"
#include "ParticleManager.h"
#include "SceneManager.h"

void TitleScene::Initialize()
{
	debugCamera.Initialize();
	WorldTransform::SetViewProjection(&viewProjection);
	WorldTransform::SetViewProjection(&debugCamera.GetViewProjection());
	lightGroup = Model::GetLightGroup();
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		lightGroup->SetDirLightActive(i, false);
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
}

void TitleScene::UI_Move()
{
	Sprite* ui = UIDrawer::GetUI((size_t)0 + input->IsConnectGamePad());
	ui->SetIsInvisible(false);
	ui->SetPosition({ WindowsAPI::WIN_SIZE.x / 2.0f,0 });

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
	ui->SetPosition({ WindowsAPI::WIN_SIZE.x / 2.0f,0 });

	mouseMoveX += std::abs(input->GetMouseMove().lX);
	if (input->IsInput(Key::Up) || input->IsInput(Key::Down) || input->IsInput(Key::Left) || input->IsInput(Key::Right))
	{
		if (uiMoveTimer.CountDown())
		{
			ui->SetIsInvisible(true);
			UIUpdate = nullptr;
		}
	}
}

void TitleScene::Draw()
{
	Model::PreDraw();
	player.Draw();
	stage.Draw();
	Model::PostDraw();
}