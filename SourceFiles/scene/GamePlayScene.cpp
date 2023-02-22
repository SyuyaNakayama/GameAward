#include "GamePlayScene.h"
#include "PostEffect.h"

void GamePlayScene::Initialize()
{
	uiDrawer.SetScene(Scene::Play);
	lightGroup = LightGroup::Create();
	debugCamera = std::make_unique<DebugCamera>();
	debugCamera->Initialize();
	WorldTransform::SetViewProjection(&debugCamera->GetViewProjection());
	WorldTransform::SetLightGroup(lightGroup.get());
	viewProjection.eye = { 0,300,0 };
	viewProjection.up = { 0,0,1 };
	viewProjection.farZ = 1500.0f;
	model = Model::Create("cube");

	skydome.Initialize(500.0f);
	Sprite* skydomeModelSprite = skydome.GetModelSprite();
	skydomeModelSprite->SetColor({ 0,0,0,1 });
	
	stage = std::make_unique<Stage>();
	stage->Initialize();
}

void GamePlayScene::Update()
{
	skydome.Update();
	uiDrawer.Update();
	viewProjection.Update();
	debugCamera->Update();

	stage->Update();
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	//skydome.Draw();
	stage->Draw();
	Model::PostDraw();

	uiDrawer.Draw();
}