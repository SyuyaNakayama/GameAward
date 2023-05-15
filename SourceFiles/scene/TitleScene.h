#pragma once
#include "BaseScene.h"
#include "Stage.h"

class TitleScene : public BaseScene
{
private:
	static UINT16 isReturnTitleScene; // �^�C�g���V�[���ɖ߂�����
	LightGroup* lightGroup;
	Stage stage;
	Timer uiMoveTimer = 80;

	void UI_Move(), UI_Camera();
	void (TitleScene::* UIUpdate)() = nullptr;

public:
	void Initialize();
	void Update();
	void Draw();
};