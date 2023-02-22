#include "Stage.h"

void Stage::Initialize()
{
	model_ = Model::Create("cube");
	worldTransform_.Initialize();
}

void Stage::Update()
{

	model_->TextureUpdate();
	worldTransform_.Update();
}

void Stage::Draw()
{
	model_->Draw(worldTransform_);
}


void Stage::LoadMap(UINT16 stage)
{
	
}
