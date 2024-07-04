#pragma once
#include "BaseScene.h"
#include "Stage.h"

struct StagePlane
{
	WorldTransform worldTransform;
	std::unique_ptr<Model> model;

	void Initialize(const std::string& textureName, const Vector3& pos, const Vector3& scale);
	void Draw() { model->Draw(worldTransform); }
};

class SelectScene : public BaseScene
{
private:
	static const int STAGE_NUM = 4;

	// ステージプレーン
	std::array<StagePlane, STAGE_NUM> stagePlanes;
	// クリアプレーン
	std::array<StagePlane, STAGE_NUM> clearPlanes;

public:
	void Initialize();
	void Update();
	void Draw();
};

