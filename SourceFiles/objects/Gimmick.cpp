#include "Gimmick.h"

void Door::Initialize()
{
	model = Model::Create("cube");
	worldTransform.Initialize();
}

void Door::Update()
{
	worldTransform.Update();
}

void Key::Initialize()
{
	model = Model::Create("cube");
	worldTransform.Initialize();
}

void Key::Update()
{
	worldTransform.Update();
}

void Candle::Initialize()
{
	model = Model::Create("cube");
	worldTransform.Initialize();
}

void Candle::Update()
{
	worldTransform.Update();
}