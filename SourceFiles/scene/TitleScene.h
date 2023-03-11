#pragma once
#include "BaseScene.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Player.h"
#include "DirectionalParticle.h"

class TitleScene : public BaseScene
{
private:
	ViewProjection viewProjection;
	std::unique_ptr<Model> cubeModel;
	WorldTransform floorTrans;
	std::unique_ptr<LightGroup> lightGroup;
	Player player;
	DirectionalParticle particle;

public:
	virtual void Initialize();
	virtual void Update();
	virtual void Draw();
	virtual void Finalize() {}
};