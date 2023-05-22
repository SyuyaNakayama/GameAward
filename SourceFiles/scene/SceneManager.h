#pragma once
#include "BaseScene.h"
#include "FadeManager.h"
#include "SceneFactory.h"

class SceneManager final
{
private:
	BaseScene* scene = nullptr;
	Scene nowScene = Scene::Null;
	Scene nextScene = Scene::Null;
	FadeManager fadeManager;
	SceneFactory* sceneFactory = SceneFactory::GetInstance();

	SceneManager() = default;
public:
	static SceneManager* GetInstance();
	SceneManager(const SceneManager& obj) = delete;
	void Initialize();
	void Finalize() { scene->Finalize(); delete scene; }
	void Update();
	void Draw();
	void ChangeScene(Scene nextScene, bool isUseFade = true);
	Scene GetNowScene() { return nowScene; }
};