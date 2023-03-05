#include "Gimmick.h"

void Door::Initialize()
{
	model = Model::Create("door");
	worldTransform.Initialize();
	worldTransform.scale = { 2.0f,2.0f,2.0f };
	worldTransform.translation.y = -1;//•‚‚¢‚Ä‚¢‚é‚Ì‚Å’²®
	flip = worldTransform;
	flip.Initialize();
	flip.translation.x += -5;//ƒYƒŒ’²®
	flip.scale.x *= -1.0f;
}

void Door::Update()
{
	worldTransform.Update();
	flip.Update();
}

void Door::Draw()
{
	model->Draw(worldTransform);
	model->Draw(flip);
}

void Candle::Initialize()
{
	model = Model::Create("candle", true);
	worldTransform.Initialize();
	worldTransform.scale = { 2.0f,2.0f,2.0f };
}

void Candle::Update()
{
	worldTransform.Update();
}