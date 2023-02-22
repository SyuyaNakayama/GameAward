#pragma once
#include <Model.h>

class Stage
{
public:
	void Initialize();
	void Update();
	void Draw();
	void LoadMap(UINT16 stage);

private:
	std::unique_ptr<Model> model_;
	WorldTransform worldTransform_;
};

