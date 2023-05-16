#include "BaseScene.h"
#include "Stage.h"

class GamePlayScene : public BaseScene
{
private:
	Stage stage;
	float timer = 0;

public:
	void Initialize();
	void Update();
	void Draw();
};