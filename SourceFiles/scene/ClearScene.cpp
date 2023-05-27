#include "ClearScene.h"

void ClearScene::Initialize()
{
	viewProjection.Initialize();
	debugCamera.Initialize();
	Model::SetViewProjection(&debugCamera.GetViewProjection());
}

void ClearScene::Update()
{
	debugCamera.Update();
}

void ClearScene::Draw()
{
}
