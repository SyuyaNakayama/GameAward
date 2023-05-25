#pragma once
#include "BaseScene.h"
#include "Collider.h"
#include "Stage.h"
#include "WindowsAPI.h"

// UI��\������͈�
class UIBox : public BoxCollider
{
	Sprite* ui = nullptr;
	Vector3 uipos;

public:
	void Initialize(const Vector3& pos, const Vector3& rad, size_t uiIndex,
		const Vector3& uipos = { WindowsAPI::WIN_SIZE.x / 2.0f,WindowsAPI::WIN_SIZE.y - 200 });
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

