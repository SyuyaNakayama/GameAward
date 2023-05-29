#include "ClearScene.h"

void ClearScene::Initialize()
{
	viewProjection.Initialize();
	debugCamera.Initialize();
	Model::SetViewProjection(&debugCamera.GetViewProjection());

	models[0] = Model::Create("yuka");
	models[1] = Model::Create("kabe");
	models[2] = Model::Create("doorkabe");
	models[3] = Model::Create("2door");

	worldTransform_.Initialize();
}

void ClearScene::Update()
{
	debugCamera.Update();
	for(int i = 0;i < 4;i++)
	{
		models[i]->Update();
	}
	worldTransform_.Update();
}

void ClearScene::Draw()
{
	Model::PreDraw();
	debugCamera.Update();
	for (int i = 0; i < 4; i++)
	{
		models[i]->Draw(worldTransform_);
	}
	Model::PostDraw();
}

