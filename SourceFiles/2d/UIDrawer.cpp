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
		// 移動
		LoadUI("UI_WASD.png", 1.0f, { 0.5f,0.0f }, true);
		LoadUI("UI_Lstick.png", 1.0f, { 0.5f,0.0f }, true);
		// 灯す
		LoadUI("UI_Shift3.png", 0.6f, { 0.5f,0.5f }, true);
		LoadUI("UI_Abutton.png", 0.5f, { 0.5f,0.5f }, true);
		// 視点移動
		LoadUI("UI_Arrow.png", 1.0f, { 0.5f,0.0f }, true);
		LoadUI("UI_Rstick.png", 1.0f, { 0.5f,0.0f }, true);
		// プレイヤーのHPゲージ
		LoadUI("white1x1.png");
		break;
	case Scene::Play:
		// 火を消す
		LoadUI("UI_SPACE.png", 1.0f, { 0.5f,0.5f }, true);
		LoadUI("UI_LBtrigger.png", 1.0f, { 0.5f,0.5f }, true);
		// 灯す
		LoadUI("UI_Shift3.png", 0.6f, { 0.5f,0.5f }, true);
		LoadUI("UI_Abutton.png", 0.5f, { 0.5f,0.5f }, true);
		// プレイヤーのHPゲージ
		LoadUI("white1x1.png");
		// 燭台のUI
		for (size_t i = 0; i < 8; i++) { LoadUI("CandleUI.png", 1.0f / 6.0f, {}, true); }
		// ジャンプ
		LoadUI("UI_Enter.png", 1.0f, { 0.5f,0.0f }, true);
		// アクション(SHIFTキー)
		LoadUI("UI_Shift1.png", 0.8f, { 0.5f,0.0f }, true);
		LoadUI("UI_Shift2.png", 1.0f, { 0.5f,0.0f }, true);
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
