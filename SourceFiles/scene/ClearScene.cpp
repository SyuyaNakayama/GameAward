#include "ClearScene.h"

void ClearScene::Initialize()
{
	viewProjection.Initialize();
	debugCamera.Initialize();
	Model::SetViewProjection(&debugCamera.GetViewProjection());
	model_yaka = Model::Create("yuka");
	model_kabe = Model::Create("kabe");
	model_doorkabe = Model::Create("doorkabe");
	model_2door = Model::Create("2door");

	worldTransform_.Initialize();

}

void ClearScene::Update()
{
	debugCamera.Update();
	model_yaka->Update();
	model_kabe->Update();
	model_doorkabe->Update();
	model_2door->Update();
}

void ClearScene::Draw()
{
	model_yaka->Draw(worldTransform_);
	model_kabe->Draw(worldTransform_);
	model_doorkabe->Draw(worldTransform_);
	model_2door->Draw(worldTransform_);
}

