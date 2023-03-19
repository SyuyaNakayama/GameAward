#include "UIDrawer.h"
#include "SpriteCommon.h"
#include "SceneManager.h"

std::array<UIDrawer::SceneUI, (size_t)Scene::SceneNum - 1> UIDrawer::sceneUIs;
Scene UIDrawer::scene = Scene::Null;

void UIDrawer::SceneUI::LoadUI(const std::string& fileName, float sizeRate, Vector2 anchorPoint, bool isInvisible)
{
	std::unique_ptr<Sprite> sprite = Sprite::Create("ui/" + fileName);
	sprite->SetSize(sprite->GetSize() * sizeRate);
	sprite->SetAnchorPoint(anchorPoint);
	sprite->SetIsInvisible(isInvisible);
	uis.push_back(std::move(sprite));
}

void UIDrawer::SceneUI::Load()
{
	SpriteAnimation* spriteAnime = nullptr;
	switch (scene)
	{
	case Scene::Title:
		break;
	case Scene::Play:
		LoadUI("UI_Abutton.png", 0.05f, { 0.5f,0.5f });
		//LoadUI("UI_LBtrigger.png", 0.05f, { 0.5f,0.5f }, true);
		//LoadUI("UI_Lclick.png", 0.05f, { 0.5f,0.5f }, true);
		//LoadUI("UI_Lstick.png", 0.05f, { 0.5f,0.5f }, true);
		//LoadUI("UI_mouse.png", 0.05f, { 0.5f,0.5f }, true);
		//LoadUI("UI_RBtrigger.png", 0.05f, { 0.5f,0.5f }, true);
		//LoadUI("UI_Rclick.png", 0.05f, { 0.5f,0.5f }, true);
		//LoadUI("UI_Rstick.png", 0.05f, { 0.5f,0.5f }, true);
		//LoadUI("UI_SPACE.png", 0.05f, { 0.5f,0.5f }, true);
		//LoadUI("UI_WASD.png", 0.05f, { 0.5f,0.5f }, true);
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
	for (auto& sceneUI : sceneUIs) { sceneUI.Load(); }
}

void UIDrawer::Update()
{
	scene = SceneManager::GetInstance()->GetNowScene();
	for (auto& sceneUI : sceneUIs) { if (sceneUI.GetScene() == scene) { sceneUI.Update(); } }
}

void UIDrawer::Draw()
{
	SpriteCommon::PreDraw();
	for (auto& sceneUI : sceneUIs) { if (sceneUI.GetScene() == scene) { sceneUI.Draw(); } }
	SpriteCommon::PostDraw();
}

Sprite* UIDrawer::GetUI(size_t index)
{
	if (scene == Scene::Null) { scene = SceneManager::GetInstance()->GetNowScene(); }
	return sceneUIs[(int)scene - 1].GetUI(index);
}
