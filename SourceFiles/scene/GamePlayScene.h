#include "BaseScene.h"
#include "Stage.h"
#include "Collider.h"

// UI‚ğ•\¦‚·‚é”ÍˆÍ
class UIBox : public BoxCollider
{
	static UINT16 uiBoxNum;
	Sprite* ui = nullptr;
	UINT16 index = 0;
	bool isOpeEnd = false; // ‘€ì‚ªI‚í‚Á‚½‚©

public:
	void Initialize(Vector3 pos, Vector3 rad, UINT16 uiIndex);
	void Update();
	void OnCollision(BoxCollider* collider);
};

class GamePlayScene : public BaseScene
{
private:
	ViewProjection viewProjection;
	Stage stage;
	Player player;
	float timer = 0;
	Sprite* ui = nullptr;
	std::array<Sprite*, 8> candleUIs;
	std::array<Sprite*, 6> keyUIs;
	std::array<UIBox, 3> uiBoxes;

	void (GamePlayScene::* UIUpdate)() = nullptr;
	void UI_Dark();

public:
	void Initialize();
	void Update();
	void Draw();
};