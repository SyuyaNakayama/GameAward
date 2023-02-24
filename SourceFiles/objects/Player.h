#pragma once
#include <Input.h>
#include <Model.h>

class Player
{
private:
	Input* input_;

	WorldTransform worldTransform_;

	bool useLight;

	std::unique_ptr<Model> model_;
	std::unique_ptr<Sprite> sprite_;

	void Move();
public:
	void Initialize();
	void Update();
	void Draw();
	~Player() { sprite_.release(); }
};

