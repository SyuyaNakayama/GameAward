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
	// UI‚Ì‰Šú‰»
	Sprite* ui = UIDrawer::GetUI((size_t)UIType::Select::Move + input->IsConnectGamePad());
	ui->SetIsInvisible(true);
	ui = UIDrawer::GetUI((size_t)UIType::Select::Camera + input->IsConnectGamePad());
	ui->SetIsInvisible(true);

	std::string stageName[(size_t)(Stage::StageNum::StageNum)] = { "Tutorial", "1", "2", "3", "4" };
	for (size_t i = 0; i < stagePlanes.size(); i++)
	{
		stagePlanes[i].Initialize("Stage" + stageName[i] + ".png", { -50 + 25 * (float)i,7.5f,32 }, { 5,1,0.01f });
	}
}

void TitleScene::Update()
{
	debugCamera.Update();
	stage.Update();
	if (UIUpdate) { (this->*UIUpdate)(); }
}

void TitleScene::UI_Move()
{
	Sprite* ui = UIDrawer::GetUI((size_t)UIType::Select::Move + input->IsConnectGamePad());
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
	Sprite* ui = UIDrawer::GetUI((size_t)UIType::Select::Camera + input->IsConnectGamePad());
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
	for (auto& stagePlane : stagePlanes) { stagePlane.Draw(); }
	Model::PostDraw();
}

void StagePlane::Initialize(const std::string& textureName, const Vector3& pos, const Vector3& scale)
{
	model = Model::Create("cube");
	model->SetAnbient({ 0.5f,0.5f,0.5f });
	std::unique_ptr<Sprite> texture = Sprite::Create("UI/stage/" + textureName);
	Vector2 textureSize = texture->GetSize();
	textureSize.x = -textureSize.x;
	texture->SetSize(textureSize);
	model->SetSprite(std::move(texture));
	worldTransform.Initialize();
	worldTransform.translation = pos;
	worldTransform.scale = scale;
	model->Update();
	worldTransform.Update();
}