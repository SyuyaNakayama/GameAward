#include "SelectScene.h"

void SelectScene::Initialize()
{
	lightGroup = Model::GetLightGroup();
	Stage::SetStageNum(0);
	stage.Initialize();
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

void SelectScene::Update()
{
	debugCamera.Update();
	stage.Update();
}

void SelectScene::Draw()
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