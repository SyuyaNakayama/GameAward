#include "BaseScene.h"
#include "Skydome.h"
#include "DebugCamera.h"
#include "Stage.h"
#include "Player.h"
#include "Input.h"
#include "Gimmick.h"

class GamePlayScene : public BaseScene
{
private:
	Skydome skydome;
	ViewProjection viewProjection;
	DebugCamera debugCamera;
	std::unique_ptr<LightGroup> lightGroup;
	Stage stage;
	Player player;
	Input* input;
	std::unique_ptr<Gimmick> gimmick;

	bool isStart = false;
	float timer = 0;
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize() { lightGroup.release(); }
	void StartScene();
};