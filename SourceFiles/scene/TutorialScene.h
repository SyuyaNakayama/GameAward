#pragma once
#include "BaseScene.h"
#include "Collider.h"
#include "Stage.h"

// UI��\������͈�
class UIBox : public BoxCollider
{
	static UINT16 uiBoxNum;
	Sprite* ui = nullptr;
	UINT16 index = 0;
	bool isOpeEnd = false; // ���삪�I�������

public:
	void Initialize(Vector3 pos, Vector3 rad, UINT16 uiIndex);
	void Update();
	void OnCollision(BoxCollider* collider);
};

class TutorialScene : public BaseScene
{
	std::array<UIBox, 9> uiBoxes;
	void (TutorialScene::* UIUpdate)() = nullptr;
	void UI_Dark();
	Stage stage;
	Sprite* ui = nullptr;

public:
	void Initialize();
	void Update();
	void Draw();
};

