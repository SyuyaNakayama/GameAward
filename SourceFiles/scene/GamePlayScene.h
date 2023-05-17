#include "BaseScene.h"
#include "Stage.h"

class GamePlayScene : public BaseScene
{
private:
	float timer = 0;

public:
	void Initialize();
	void Update();
	void Draw();
};