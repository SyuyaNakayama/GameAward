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
		// Ú®
		LoadUI("UI_WASD.png", 1.0f, { 0.5f,0.5f }, true);
		LoadUI("UI_Lstick.png", 1.0f, { 0.5f,0.5f }, true);
		// ·
		LoadUI("UI_Lclick.png", 1.0f, { 0.5f,0.5f }, true);
		LoadUI("UI_Abutton.png", 1.0f, { 0.5f,0.5f }, true);
		// _Ú®
		LoadUI("UI_mouse.png", 1.0f, { 0.5f,0.5f }, true);
		LoadUI("UI_Rstick.png", 1.0f, { 0.5f,0.5f }, true);
		// vC[ÌHPQ[W
		LoadUI("white1x1.png");
		break;
	case Scene::Play:
		// ÎðÁ·
		LoadUI("UI_SPACE.png", 1.0f, { 0.5f,0.5f }, true);
		LoadUI("UI_LBtrigger.png", 1.0f, { 0.5f,0.5f }, true);
		// ·
		LoadUI("UI_Lclick.png", 1.0f, { 0.5f,0.5f }, true);
		LoadUI("UI_Abutton.png", 1.0f, { 0.5f,0.5f }, true);
		// vC[ÌHPQ[W
		LoadUI("white1x1.png");
		// CäÌUI
		for (size_t i = 0; i < 8; i++) { LoadUI("CandleUI.png", 1.0f / 6.0f, {}, true); }
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
	scene = SceneManager::GetInstance()->GetNowScene();
	return sceneUIs[(int)scene - 1].GetUI(index);
}
