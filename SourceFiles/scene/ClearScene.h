#pragma once
#include "BaseScene.h"
#include "ViewProjection.h"

class ClearScene : public BaseScene
{
	ViewProjection viewProjection;
	LightGroup* lightGroup;

public:
	void Initialize();
	void Update();
	void Draw();

private:
	std::unique_ptr<Model> models_object[4];
	std::unique_ptr<Model> models_player[3];

	WorldTransform worldTransform_;
	WorldTransform playerTrans_[3];
};