#pragma once
#include "BaseScene.h"
#include "Stage.h"

class StagePlane
{
private:
	WorldTransform worldTransform;
	std::unique_ptr<Model> model;

public:
	void Initialize(const std::string& textureName, const Vector3& pos, const Vector3& scale);
	void Draw() { model->Draw(worldTransform); }
};

class TitleScene : public BaseScene
{
private:
	static UINT16 isReturnTitleScene; // タイトルシーンに戻った回数
	LightGroup* lightGroup;
	Stage stage;
	Timer uiMoveTimer = 80;
	// ステージプレーン
	std::array<StagePlane, 5> stagePlanes;

	void UI_Move(), UI_Camera();
	void (TitleScene::* UIUpdate)() = nullptr;

public:
	void Initialize();
	void Update();
	void Draw();
};