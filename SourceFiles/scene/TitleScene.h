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
	void ToPlay(); // プレイシーンへ
	void ToTutorial(); // チュートリアルへ
	void PreFadeToPlay(); // フェード前処理(プレイシーン)
	void PreFadeToTutorial(); // フェード前処理(チュートリアル)
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