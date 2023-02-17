#include "TitleScene.h"

void TitleScene::Initialize()
{
	uiDrawer.SetScene(Scene::Title);
}

void TitleScene::Update()
{
	uiDrawer.Update();
}

void TitleScene::Draw()
{
	uiDrawer.Draw();
}