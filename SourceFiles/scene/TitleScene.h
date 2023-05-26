#pragma once
#include "BaseScene.h"

struct SelectUI
{
	Sprite* sprite;
	Vector2 pos;
	Vector2 offset;
	Angle lerpRateRad = 0;
	Timer fadeTimer = 45;

	void Initialize();
	void Update();
	void SetSpritePos() { sprite->SetPosition(pos); }
	bool PreFade();
};

class TitleScene : public BaseScene
{
private:
	void (TitleScene::* Select)() = &TitleScene::ToPlay;
	void ToPlay(); // �v���C�V�[����
	void ToTutorial(); // �`���[�g���A����
	void PreFadeToPlay(); // �t�F�[�h�O����(�v���C�V�[��)
	void PreFadeToTutorial(); // �t�F�[�h�O����(�`���[�g���A��)
	Sprite* titleUI = nullptr;
	std::array<SelectUI, 2> selectUIs;
	Vector2 padStick;
	Sprite* nextUI = nullptr;
	Angle nextUIAlphaRate = 0;

public:
	void Initialize();
	void Update();
	void Draw();
};