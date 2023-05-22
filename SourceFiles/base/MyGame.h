#pragma once
#include "Framework.h"
#include "ParticleManager.h"

class MyGame : public Framework
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
};