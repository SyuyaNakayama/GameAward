#include "SceneManager.h"
#include "SpriteCommon.h"
#include "CollisionManager.h"

SceneManager* SceneManager::GetInstance()
{
	static SceneManager sceneManager;
	return &sceneManager;
}

void SceneManager::Initialize()
{
	fadeManager.Initialize();
}

void SceneManager::Update()
{
	fadeManager.Update();

	bool isChangeScene = fadeManager.IsChange() || !fadeManager.IsFade();
	isChangeScene &= nextScene != Scene::Null;
	if (isChangeScene)
	{
		if (scene)
		{
			scene->Finalize();
			delete scene;
		}

		scene = sceneFactory->CreateScene(nextScene);
		nowScene = nextScene;
		nextScene = Scene::Null;
		scene->Initialize();
	}

	if (!fadeManager.IsFade())
	{
		scene->Update();
		CollisionManager::CheckAllCollisions();
	}
}

void SceneManager::Draw()
{
	scene->Draw();
	if (!fadeManager.IsFade()) { return; }
	SpriteCommon::PreDraw();
	fadeManager.Draw();
	SpriteCommon::PostDraw();
}

void SceneManager::ChangeScene(Scene nextScene_, bool isUseFade)
{
	nextScene = nextScene_;
	if (isUseFade) { fadeManager.FadeScene(); }
}
