#pragma once
#include "BaseScene.h"
#include "Scene.h"

class SceneFactory
{
private:
	SceneFactory() = default;

public:
	SceneFactory(const SceneFactory& obj) = delete;
	static SceneFactory* GetInstance();

	BaseScene* CreateScene(Scene scene);
};