#include "BaseScene.h"
#include "Skydome.h"
#include "DebugCamera.h"
#include "Stage.h"
#include "Player.h"

class GamePlayScene : public BaseScene
{
private:
	Skydome skydome;
	ViewProjection viewProjection;
	std::unique_ptr<LightGroup> lightGroup;
	
	std::unique_ptr<Model> model;
	std::unique_ptr<DebugCamera> debugCamera;
	
	std::unique_ptr<Stage> stage;
	std::unique_ptr<Player> player;

public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize() { lightGroup.release(); }
};