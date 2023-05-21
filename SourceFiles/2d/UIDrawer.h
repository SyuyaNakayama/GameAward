#pragma once
#include <vector>
#include "SpriteAnimation.h"
#include "Scene.h"

namespace UIType
{
	// ステージ選択のUI
	enum class Select
	{
		Light, // 灯す
	};

	// チュートリアルのUI
	enum class Tutorial
	{
		LightChange = 26, // ライト切り替え
		Jump = 28, // ジャンプ
		tutorialText1 = 30,
		tutorialText2,
		tutorialText3,
		tutorialText4,
		tutorialText5,
		tutorialText6,
		tutorialText7,
		tutorialText8,
		tutorialText9,
		tutorialText10,
		tutorialText11,
		tutorialText12,
		tutorialText13,
		tutorialText14,
		tutorialText1_2,
		tutorialText3_2,
		tutorialText6_2
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
		KeyOpen = 22, // 鍵を開ける
		Reset = 24, // リセット
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