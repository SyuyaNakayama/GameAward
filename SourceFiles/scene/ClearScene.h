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
};