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
	float objUISizeRate = 1.0f / 8.0f;

	SpriteAnimation* spriteAnime = nullptr;
	switch (scene)
	{
	case Scene::Title:
		LoadUI("stage/stageTutorial.png", 3.0f, { 0.5f,0.5f }); // タイトル
		LoadUI("stage/stageTutorial.png", 1.5f, { 0.5f,0.5f }); // ゲームプレイ
		LoadUI("stage/stageTutorial.png", 1.5f, { 0.5f,0.5f }); // チュートリアル
		break;
	case Scene::Select:
		// 移動
		//LoadUI("keyboard/move.png", 1.0f, { 0.5f,0.0f }, true);
		//LoadUI("controller/move.png", 1.0f, { 0.5f,0.0f }, true);
		// 灯す
		LoadUI("keyboard/light.png", 0.6f, { 0.5f,0.5f }, true);
		LoadUI("controller/light.png", 0.5f, { 0.5f,0.5f }, true);
		// 視点移動
		//LoadUI("keyboard/cameraMove.png", 1.0f, { 0.5f,0.0f }, true);
		//LoadUI("controller/cameraMove.png", 1.0f, { 0.5f,0.0f }, true);
		break;
	case Scene::Tutorial:
	case Scene::Play:
		// プレイヤーのHPゲージの背景
		LoadUI("white1x1.png");
		LoadUI("white1x1.png");
		// プレイヤーのHPゲージの枠
		LoadUI("hpUI.png", 1.0f, { 0.0f,0.0f }, true);
		// 燭台のUI
		for (size_t i = 0; i < 8; i++) { LoadUI("CandleUI.png", objUISizeRate, {}, true); }
		// 鍵
		LoadUI("key/key.png", objUISizeRate, {}, true);
		// 鍵パーツ
		LoadUI("key/parts1.png", objUISizeRate * 2.0f, {}, true);
		LoadUI("key/parts2.png", objUISizeRate * 2.0f, {}, true);
		LoadUI("key/parts3.png", objUISizeRate * 2.0f, {}, true);
		LoadUI("key/parts4.png", objUISizeRate * 2.0f, {}, true);
		LoadUI("key/parts4.png", objUISizeRate * 2.0f, {}, true);
		LoadUI("key/parts5.png", objUISizeRate * 2.0f, {}, true);
		// 灯す
		LoadUI("keyboard/light.png", 0.6f, { 0.5f,0.5f }, true);
		LoadUI("controller/light.png", 0.5f, { 0.5f,0.5f }, true);
		// レバー切り替え
		LoadUI("keyboard/lever.png", 0.8f, { 0.5f,0.0f }, true);
		LoadUI("controller/lever.png", 0.8f, { 0.5f,0.0f }, true);
		// 鍵を開ける
		LoadUI("keyboard/keyOpen.png", 0.8f, { 0.5f,0.0f }, true);
		LoadUI("controller/keyOpen.png", 0.8f, { 0.5f,0.0f }, true);
		// リセットUI
		LoadUI("keyboard/reset.png", 0.5f, { -5.5f, -0.5f });
		LoadUI("controller/reset.png", 0.8f, { -5.5f, -0.15f });
		if (scene == Scene::Play) { break; }
		/* -----チュートリアルの説明UI----- */
		// 火を変える
		LoadUI("keyboard/lightChange.png", 1.0f, { 0.5f,0.5f }, true);
		LoadUI("controller/lightChange.png", 1.0f, { 0.5f,0.5f }, true);
		// ジャンプ
		LoadUI("keyboard/jump.png", 0.8f, { 0.5f,0.0f }, true);
		LoadUI("controller/jump.png", 0.8f, { 0.5f,0.0f }, true);
		//チュートリアルテキスト
		LoadUI("tutorial/tutorial1.png", 1.0f, { 0.5f,0.5f }, true);	// 移動＆視点移動
		LoadUI("tutorial/tutorial2.png", 1.0f, { 0.5f,0.5f }, true);	// 近くの燭台を灯そう
		LoadUI("tutorial/tutorial3.png", 1.0f, { 0.5f,0.5f }, true);	// ジャンプ
		LoadUI("tutorial/tutorial4.png", 1.0f, { 0.5f,0.5f }, true);	// HPが減るよ
		LoadUI("tutorial/tutorial5.png", 1.0f, { 0.5f,0.5f }, true);	// 新たな燭台を灯そう
		LoadUI("tutorial/tutorial6.png", 1.0f, { 0.5f,0.5f }, true);	// 火を切り替える
		LoadUI("tutorial/tutorial7.png", 1.0f, { 0.5f,0.5f }, true);	// 火の色によって変わるよ
		LoadUI("tutorial/tutorial8.png", 1.0f, { 0.5f,0.5f }, true);	// スイッチを移動しよう
		LoadUI("tutorial/tutorial9.png", 1.0f, { 0.5f,0.5f }, true);	// スイッチをは一つだけじゃない
		LoadUI("tutorial/tutorial10.png", 1.0f, { 0.5f,0.5f }, true);	// 上へ行きたい
		LoadUI("tutorial/tutorial11.png", 1.0f, { 0.5f,0.5f }, true);	// 鍵を取得してみよう
		LoadUI("tutorial/tutorial12.png", 1.0f, { 0.5f,0.5f }, true);	// 鍵の欠片もあるよ
		LoadUI("tutorial/tutorial13.png", 1.0f, { 0.5f,0.5f }, true);	// 鍵を使ってみよう
		LoadUI("tutorial/tutorial14.png", 1.0f, { 0.5f,0.5f }, true);	// あのドアがゴールだ
		//チュートリアルテキスト(キーボード)
		LoadUI("keyboard/move.png", 1.0f, { 0.5f,0.5f }, true);
		LoadUI("keyboard/jump.png", 1.0f, { 0.5f,0.5f }, true);
		LoadUI("keyboard/lightChange.png", 1.0f, { 0.5f,0.5f }, true);
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
	sceneUIs[(size_t)Scene::Title - 1].SetScene(Scene::Title);
	sceneUIs[(size_t)Scene::Select - 1].SetScene(Scene::Select);
	sceneUIs[(size_t)Scene::Tutorial - 1].SetScene(Scene::Tutorial);
	sceneUIs[(size_t)Scene::Play - 1].SetScene(Scene::Play);
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
	return sceneUIs[(size_t)scene - 1].GetUI(index);
}