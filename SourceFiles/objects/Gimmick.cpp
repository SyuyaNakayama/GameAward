#include "Gimmick.h"
#include "ImGuiManager.h"
#include "Input.h"
#include <imgui.h>

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
	particleProp =
	{
		lightPos,{0,0.01f,0},{0,0.0005f,0},
		0.025f,0.001f,0,40,0.4f
	};
	lightPos = worldTransform.translation + Vector3(0, worldTransform.scale.y + 1.2f);
}

void Candle::Update()
{
	ImGuiManager::DragVector("candlePos", worldTransform.translation);
	worldTransform.Update();
	//if (Input::GetInstance()->IsTrigger(Mouse::Left)) { isLight = !isLight; }
	lightGroup->SetPointLightActive(lightIndex, isLight);
	if (isLight)
	{
		model->SetAnbient({ 0.7f,0.3f,0.3f });
		lightPos = worldTransform.translation + Vector3(0, worldTransform.scale.y + 1.2f);

		particleProp.posOffset = lightPos;
		ImGuiManager::DragVector("particleVelOffset", particleProp.velOffset, 0.001f);
		ImGuiManager::DragVector("particleAccOffset", particleProp.accOffset, 0.00001f);
		ImGui::DragFloat("particlePosRange", &particleProp.posRange, 0.002f, 0.002f, 2.0f);
		ImGui::DragFloat("particleVelRange", &particleProp.velRange, 0.0001f, 0.0001f, 0.1f);
		ImGui::DragFloat("particleAccRange", &particleProp.accRange, 0.00001f, 0.00001f, 0.01f);
		ImGui::DragInt("particleLifeTime", &particleProp.lifeTime, 1, 1, 1000);
		ImGui::DragFloat("particleStartScale", &particleProp.start_scale, 0.05f, 0.05f, 10.0f);

		lightGroup->SetPointLightPos(lightIndex, lightPos);
		lightGroup->SetPointLightAtten(lightIndex, { 0.2f, 0.01f });
		lightGroup->SetPointLightColor(lightIndex, { 1,0.5f,0.5f });
		ParticleManager::Add(particleProp);
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