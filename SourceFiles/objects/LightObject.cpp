#include "LightObject.h"

void NormalLight::Initialize(LightGroup* lightGroup_)
{
	worldTransform.Initialize();
	model = Model::Create("cube");
	lightGroup = lightGroup_;
	lightGroup->SetSpotLightActive(0, true);
	lightGroup->SetSpotLightColor(0, { 1,1,0.5f });
}

void NormalLight::Update()
{
	worldTransform.Update();
	lightGroup->SetSpotLightDir(0, GetLightDir());
	lightGroup->SetSpotLightPos(0, worldTransform.GetWorldPosition());
}

void BlackLight::Initialize(LightGroup* lightGroup_)
{
	worldTransform.Initialize();
	model = Model::Create("cube");
	lightGroup = lightGroup_;
	lightGroup->SetSpotLightActive(1, true);
	lightGroup->SetSpotLightColor(1, { 0.1f,0.1f,0.5f });
}

void BlackLight::Update()
{
	worldTransform.Update();
	lightGroup->SetSpotLightDir(1, GetLightDir());
	lightGroup->SetSpotLightPos(1, worldTransform.GetWorldPosition());
}