#pragma once
#include "BaseScene.h"

class TitleScene : public BaseScene
{
private:

public:
	virtual void Initialize();
	virtual void Update();
	virtual void Draw();
	virtual void Finalize() {}
};