#include "Gimmick.h"
#include "ImGuiManager.h"

void Door::Initialize()
{
	model = Model::Create("door", true);
	worldTransform.Initialize();
	worldTransform.scale = { 2.0f,2.0f,2.0f };
	worldTransform.translation.y = -1; // 浮いているので調整
	flip = worldTransform;
	flip.Initialize();
	flip.translation.x += -5; // ズレ調整
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
	ImGuiManager::DragVector("candlePos", worldTransform.translation);
	worldTransform.Update();
}