#pragma once
#include "Framework.h"
#include "PostEffect.h"

class MyGame : public Framework
{
private:
	std::unique_ptr<PostEffect> postEffect;

public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};