#include "BaseScene.h"
#include "Skydome.h"
#include "DebugCamera.h"
#include "Stage.h"
#include "Player.h"
#include "PointAudio.h"

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
	float timer = 0;
	PointAudio audio;

public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize() { lightGroup.release(); }
	void StartScene();
};