#pragma once
#include <unordered_map>
#include "Scene.h"
#include "Sprite.h"

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
		Move = 26,
		CameraMove = 28,
		LightChange = 30, // ライト切り替え
		Jump = 32, // ジャンプ
		TutorialText1 = 34,	// 近くの燭台を灯そう
		TutorialText2,	   	// HPが減るよ
		TutorialText3,	   	// 新たな燭台を灯そう
		TutorialText4,	   	// 火を切り替える
		TutorialText5,	   	// 火の色によって変わるよ
		TutorialText6,	   	// スイッチを移動しよう
		TutorialText7,	   	// スイッチは一つだけじゃない
		TutorialText8,	   	// 上へ行きたい
		TutorialText9,	   	// 鍵を取得してみよう
		TutorialText10,	   	// 鍵の欠片もあるよ
		TutorialText11,	   	// 鍵を使ってみよう
		TutorialText12,	   	// あのドアがゴールだ
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
		Scene scene = Scene::Null;

		void LoadUI(const std::string& fileName, float sizeRate = 1.0f, const Vector2& anchorPoint = {}, bool isInvisible = false, const Vector2& pos = {});

	public:
		void SetScene(Scene nowScene) { scene = nowScene; }
		void Load();
		void Update();
		void Draw();
		Sprite* GetUI(size_t index) { return uis[index].get(); }
	};

	static Scene scene;
	static std::unordered_map<Scene, SceneUI> sceneUIs;

public:
	UIDrawer() = delete;
	~UIDrawer() = delete;

	static void LoadAll();
	static void Update();
	static void Draw();
	static Sprite* GetUI(size_t index);
};

void UIReset(); // 燭台と鍵のUIを消す