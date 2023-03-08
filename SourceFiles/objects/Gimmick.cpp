#include "Gimmick.h"
#include "ImGuiManager.h"
#include "Input.h"

void Door::Initialize()
{
	model = Model::Create("door", true);
	worldTransform.Initialize();
	worldTransform.scale = { 2.0f,2.0f,2.0f };
	worldTransform.translation.y = -1; // •‚‚¢‚Ä‚¢‚é‚Ì‚Å’²®
	flip = worldTransform;
	flip.Initialize();
	flip.translation.x += -5; // ƒYƒŒ’²®
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
	lightGroup = Model::GetLightGroup();
}

void Candle::Update()
{
	ImGuiManager::DragVector("candlePos", worldTransform.translation);
	worldTransform.Update();
	if (Input::GetInstance()->IsTrigger(Mouse::Left)) { isLight = !isLight; }
	lightGroup->SetPointLightActive(lightIndex, isLight);
	if (isLight)
	{
		model->SetAnbient({ 0.7f,0.3f,0.3f });
		lightGroup->SetPointLightPos(lightIndex, worldTransform.translation + Vector3(0, worldTransform.scale.y + 1.2f));
		lightGroup->SetPointLightAtten(lightIndex, { 0.2f, 0.01f });
		lightGroup->SetPointLightColor(lightIndex, { 1,0.5f,0.5f });
	}
	else
	{
		model->SetAnbient({ 0,0,0 });
	}
	model->Update();
}

void Candle::Draw()
{
	model->Draw(worldTransform);
}