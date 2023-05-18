#pragma once
#include "BaseScene.h"
#include "Collider.h"
#include "Stage.h"

// UIð\¦·éÍÍ
class UIBox : public BoxCollider
{
	static UINT16 uiBoxNum;
	Sprite* ui = nullptr;
	UINT16 index = 0;

public:
	void Initialize(Vector3 pos, Vector3 rad, UINT16 uiIndex);
	void Update();
	void OnCollision(BoxCollider* collider);
};

class TutorialScene : public BaseScene
{
	std::array<UIBox, 14> uiBoxes;
	Sprite* ui = nullptr;

public:
	void Initialize();
	void Update();
	void Draw();
};

