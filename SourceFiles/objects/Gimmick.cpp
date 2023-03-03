#include "Gimmick.h"

void Door::Initialize()
{
	model = Model::Create("door");
	worldTransform.Initialize();
	worldTransform.scale = { 2.0f,2.0f,2.0f };
}

void Door::Update()
{
	worldTransform.Update();
}

//void Key::Initialize()
//{
//	model = Model::Create("cube");
//	worldTransform.Initialize();
//}
//
//void Key::Update()
//{
//	worldTransform.Update();
//}

void Candle::Initialize()
{
	model = Model::Create("candle");
	worldTransform.Initialize();
}

void Candle::Update()
{
	worldTransform.Update();
}