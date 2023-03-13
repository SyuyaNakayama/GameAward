#include "BaseScene.h"
#include "Skydome.h"
#include "Stage.h"
#include "Player.h"
#include "PointAudio.h"

class GamePlayScene : public BaseScene
{
private:
	Skydome skydome;
	ViewProjection viewProjection;
	std::unique_ptr<LightGroup> lightGroup;
	Stage stage;
	Player player;
	float timer = 0;

public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize() { lightGroup.release(); }
	void StartScene();
};