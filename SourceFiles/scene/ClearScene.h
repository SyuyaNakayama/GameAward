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
	std::array<std::unique_ptr<Model>, 4> models_object;
	std::array<std::unique_ptr<Model>, 3> models_player;

	WorldTransform worldTransform_;
	std::array <WorldTransform, 3> playerTrans_;

	Sprite* clearUI = nullptr;
};