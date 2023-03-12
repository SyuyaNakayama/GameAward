#pragma once
#include "BaseScene.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Player.h"

class TitleScene : public BaseScene
{
private:
	ViewProjection viewProjection;
	std::unique_ptr<Model> cubeModel;
	WorldTransform floorTrans;
	std::unique_ptr<LightGroup> lightGroup;
	Player player;

public:
	void Initialize();
	void Update();
	void Draw();
};