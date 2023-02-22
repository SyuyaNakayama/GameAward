#include "Stage.h"

void Stage::Initialize()
{
	model_ = Model::Create("cube");
	worldTransform_.Initialize();
	worldTransform_.translation = { 0,-5,0 };
	worldTransform_.scale = { 20,1,20 };
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
