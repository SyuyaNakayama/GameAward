#pragma once
#include "BaseScene.h"
#include "Player.h"
#include "Stage.h"

class TitleScene : public BaseScene
{
private:
	ViewProjection viewProjection;
	std::unique_ptr<LightGroup> lightGroup;
	Player player;
	Stage stage;

public:
	void Initialize();
	void Update();
	void Draw();
};