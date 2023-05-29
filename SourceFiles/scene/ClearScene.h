#pragma once
#include "BaseScene.h"
#include "ViewProjection.h"

class ClearScene : public BaseScene
{
	ViewProjection viewProjection;

public:
	void Initialize();
	void Update();
	void Draw();

private:
	std::unique_ptr<Model> models[4];
	std::unique_ptr<Model> model_yaka;
	std::unique_ptr<Model> model_kabe;
	std::unique_ptr<Model> model_2door;
	std::unique_ptr<Model> model_doorkabe;

	WorldTransform worldTransform_;
};