#include "TitleScene.h"
#include "ImGuiManager.h"
#include "ParticleManager.h"
#include "SceneManager.h"

UINT16 TitleScene::isReturnTitleScene = false;

void TitleScene::Initialize()
{
	lightGroup = Model::GetLightGroup();
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		lightGroup->SetDirLightActive(i, false);
	}
	Stage::SetStageNum(0);
	stage.Initialize();
	if (!isReturnTitleScene) { UIUpdate = &TitleScene::UI_Move; }
	Sprite* ui = UIDrawer::GetUI((size_t)0 + input->IsConnectGamePad());
	ui->SetIsInvisible(true);
	ui = UIDrawer::GetUI((size_t)4 + input->IsConnectGamePad());
	ui->SetIsInvisible(true);
}

void TitleScene::Update()
{
	debugCamera.Update();
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
	stage.Draw();
	Model::PostDraw();
}