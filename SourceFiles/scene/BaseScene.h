#pragma once
#include "Input.h"
#include "UIDrawer.h"
class SceneManager;

class BaseScene
{
protected:
	SceneManager* sceneManager_ = nullptr;
	Input* input = Input::GetInstance();
	UIDrawer uiDrawer;

public:
	virtual ~BaseScene() = default;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() {};
	void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }
};