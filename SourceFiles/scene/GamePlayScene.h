#include "BaseScene.h"
#include "Stage.h"
#include "Collider.h"

// UI‚ð•\Ž¦‚·‚é”ÍˆÍ
class UISphere : public BoxCollider
{
public:
	void Initialize(Vector3 pos, Vector3 rad);
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
	std::array<UISphere, 2> uiSpheres;

	void (GamePlayScene::* UIUpdate)() = nullptr;
	void UI_Dark();

public:
	void Initialize();
	void Update();
	void Draw();
};