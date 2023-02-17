#include "BaseScene.h"
#include "Skydome.h"
#include "DebugCamera.h"

class GamePlayScene : public BaseScene
{
private:
	Skydome skydome;
	ViewProjection viewProjection;
	std::unique_ptr<LightGroup> lightGroup;
	std::array<WorldTransform, 4> p;
	WorldTransform box;
	std::unique_ptr<Model> model;
	std::unique_ptr<DebugCamera> debugCamera;
	float t = 0;
	int startIndex = 1;

public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize() { lightGroup.release(); }
};