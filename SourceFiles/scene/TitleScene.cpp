#include "TitleScene.h"
#include "ImGuiManager.h"
#include <random>
#include "ParticleManager.h"
#include "SceneManager.h"

void TitleScene::Initialize()
{
	uiDrawer.SetScene(Scene::Title);
	debugCamera.Initialize();
	WorldTransform::SetViewProjection(&viewProjection);
	//WorldTransform::SetViewProjection(&debugCamera.GetViewProjection());
	cubeModel = Model::Create("cube");
	std::unique_ptr<Sprite> sprite_ = Sprite::Create("stages/floor.png");
	sprite_->SetSize(sprite_->GetSize() / 5.0f);
	cubeModel->SetSprite(std::move(sprite_));
	cubeModel->Update();
	floorTrans.Initialize();
	floorTrans.translation.y = -2.0f;
	floorTrans.scale = { 60,1,32 };
	lightGroup = LightGroup::Create();
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		lightGroup->SetDirLightActive(i, false);
	}
	Model::SetLightGroup(lightGroup.get());
	viewProjection.eye = { 2.5f,1,-7 };
	viewProjection.target = { 2.5f,1 };
	player.Initialize();
}

void TitleScene::Update()
{
	ImGuiManager::InputVector("viewProjection.eye", viewProjection.eye);
	ImGuiManager::InputVector("viewProjection.target", viewProjection.target);
	floorTrans.Update();
	uiDrawer.Update();
	debugCamera.Update();
	viewProjection.Update();
	player.Update();

	std::random_device rnd;
	std::mt19937 rnddev(rnd());
	std::uniform_real_distribution<float> randRadius(0.5f, 2.5f);
	std::uniform_real_distribution<float> randAngle(0, 2.0f * PI);
	DirectionalParticle::AddProp particleProp =
	{
		player.GetWorldTransform().translation,{0,2,0},2,randAngle(rnddev),randRadius(rnddev)
	};
	ParticleManager::Add(particleProp);

	if (input->IsTrigger(Key::K)) 
	{
	ParticleManager::Clear();
		sceneManager_->SetNextScene(Scene::Play); 
	}
}

void TitleScene::Draw()
{
	Model::PreDraw();
	//cubeModel->Draw(floorTrans);
	player.Draw();
	Model::PostDraw();

	uiDrawer.Draw();
}