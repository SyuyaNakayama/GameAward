#include "TitleScene.h"
#include "ImGuiManager.h"

void TitleScene::Initialize()
{
	Stage::SetStageNum(0);
	stage.Initialize();
}

void TitleScene::Update()
{
	stage.Update();
}

void TitleScene::Draw()
{
	Model::PreDraw();
	stage.Draw();
	Model::PostDraw();
}
