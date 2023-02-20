#include "UIDrawer.h"
#include "SpriteCommon.h"

std::array<UIDrawer::SceneUI, (size_t)Scene::SceneNum - 1> UIDrawer::sceneUIs;
Scene UIDrawer::scene = Scene::Null;

void UIDrawer::SceneUI::Load()
{
	std::unique_ptr<Sprite> sprite;
	SpriteAnimation* spriteAnime = nullptr;
	switch (scene)
	{
	case Scene::Title:
		break;
	case Scene::Play:
		break;
	}
}

void UIDrawer::SceneUI::Update()
{
	for (auto& ui : uis) { ui->Update(); }
	for (auto& animationUI : animationUIs) { animationUI.Update(); }
}

void UIDrawer::SceneUI::Draw()
{
	for (auto& ui : uis) { ui->Draw(); }
	for (auto& animationUI : animationUIs) { animationUI.Draw(); }
}

void UIDrawer::LoadAll()
{
	sceneUIs[0].SetScene(Scene::Title);
	sceneUIs[1].SetScene(Scene::Play);
	for (auto& sceneUI : sceneUIs)
	{
		sceneUI.Load();
	}
}

void UIDrawer::Update()
{
	for (auto& sceneUI : sceneUIs)
	{
		if (sceneUI.GetScene() == scene) { sceneUI.Update(); }
	}
}

void UIDrawer::Draw()
{
	SpriteCommon::PreDraw();
	for (auto& sceneUI : sceneUIs)
	{
		if (sceneUI.GetScene() == scene) { sceneUI.Draw(); }
	}
	SpriteCommon::PostDraw();
}