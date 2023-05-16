#pragma once
#include <vector>
#include "SpriteAnimation.h"
#include "Scene.h"

namespace UIType
{
	// ステージ選択のUI
	enum class Select
	{
		Move = 0, // 移動
		Light = 2, // 灯す
		Camera = 4 // 視点移動
	};

	// チュートリアルのUI
	enum class Tutorial
	{
		LightChange = 23, // ライト切り替え
		Jump = 25, // ジャンプ
		Heal = 26, // 回復エリア
		tutorialText1 = 27,
		tutorialText2 = 28,
		tutorialText3 = 29,
		tutorialText4 = 30,
		tutorialText5 = 31,
		tutorialText6 = 32,
		tutorialText7 = 33,
		tutorialText8 = 34,
		tutorialText9 = 35,
		tutorialText10 = 36,
		tutorialText11 = 37,
		tutorialText12 = 38,
		tutorialText13 = 39,
		tutorialText14 = 40,
	};

	// プレイシーンのUI(チュートリアルシーンと兼用)
	enum class Play
	{
		PlayerGuage = 0, // HPゲージ
		PlayerGuageFrame = 2, // HPゲージの枠
		Candle = 3, // 燭台
		Key = 11, // 鍵
		KeyParts = 12, // 鍵パーツ
		Light = 18,
		Lever = 20, // レバー
		KeyOpen = 21, // 鍵を開ける
		Reset = 22, // リセット
	};
}

class UIDrawer
{
private:
	class SceneUI
	{
	private:
		std::vector<std::unique_ptr<Sprite>> uis;
		std::vector<SpriteAnimation> animationUIs;
		Scene scene = Scene::Null;

		void LoadUI(const std::string& fileName, float sizeRate = 1.0f, Vector2 anchorPoint = {}, bool isInvisible = false);

	public:
		void SetScene(Scene nowScene) { scene = nowScene; }
		Scene GetScene() { return scene; }
		void Load();
		void Update();
		void Draw();
		Sprite* GetUI(size_t index) { return uis[index].get(); }
	};

	static Scene scene;
	static std::array<SceneUI, (size_t)Scene::SceneNum - 1> sceneUIs;

public:
	UIDrawer() = delete;
	~UIDrawer() = delete;

	static void LoadAll();
	static void Update();
	static void Draw();
	static Sprite* GetUI(size_t index);
};