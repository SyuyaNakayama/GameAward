#pragma once
#include <Input.h>
#include <Model.h>

class Player
{
private:
	std::unique_ptr<Input> input_;

	WorldTransform worldTransform_;

	bool useLight;



public:
	void Initialize();
	void Update();
	void

};

