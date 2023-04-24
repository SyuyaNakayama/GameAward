#include "BaseScene.h"
#include "Stage.h"

class GamePlayScene : public BaseScene
{
private:
	ViewProjection viewProjection;
	Stage stage;
	Player player;
	float timer = 0;
	Sprite* ui = nullptr;
	std::array<Sprite*, 8> candleUIs;

	void (GamePlayScene::* UIUpdate)() = nullptr;
	void UI_Dark();

public:
	void Initialize();
	void Update();
	void Draw();
};