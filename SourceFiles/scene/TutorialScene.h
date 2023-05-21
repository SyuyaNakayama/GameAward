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
	Vector3 uipos;

public:
	void Initialize(const Vector3& pos, const Vector3& rad, size_t uiIndex, const Vector3& uipos = { 1920.0f / 2.0f,100 });
	void Update();
	void OnCollision(BoxCollider* collider);
};

class TutorialScene : public BaseScene
{
	std::array<UIBox, 16> uiBoxes;
	Sprite* ui = nullptr;

public:
	void Initialize();
	void Update();
	void Draw();
};

