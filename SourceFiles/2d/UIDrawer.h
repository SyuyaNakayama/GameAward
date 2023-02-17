#pragma once
#include <vector>
#include "SpriteAnimation.h"
#include "Scene.h"

class UIDrawer
{
private:
	class SceneUI
	{
	private:
		std::vector<std::unique_ptr<Sprite>> uis;
		std::vector<SpriteAnimation> animationUIs;
		Scene scene = Scene::Null;

	public:
		void SetScene(Scene nowScene) { scene = nowScene; }
		Scene GetScene() { return scene; }
		void Load();
		void Update();
		void Draw();
	};

	static Scene scene;
	static std::array<SceneUI, (size_t)Scene::SceneNum - 1> sceneUIs;

public:
	void SetScene(Scene nowScene) { scene = nowScene; }
	static void LoadAll();
	void Update();
	void Draw();
};