#pragma once
#include "BaseScene.h"
#include "Stage.h"

class TitleScene : public BaseScene
{
private:

	ViewProjection viewProjection;
	LightGroup* lightGroup;
	Player player;
	Stage stage;
	Timer uiMoveTimer = 60;
	long mouseMoveX = 0;

	void UI_Move(), UI_Camera();
	void (TitleScene::* UIUpdate)() = nullptr;

public:
	void Initialize();
	void Update();
	void Draw();
};