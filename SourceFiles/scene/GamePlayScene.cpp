#include "GamePlayScene.h"
#include <algorithm>
#include "PostEffect.h"

void GamePlayScene::Initialize()
{
	uiDrawer.SetScene(Scene::Play);
	lightGroup = LightGroup::Create();
	debugCamera = std::make_unique<DebugCamera>();
	debugCamera->Initialize();
	WorldTransform::SetViewProjection(&debugCamera->GetViewProjection());
	WorldTransform::SetViewProjection(&viewProjection);
	WorldTransform::SetLightGroup(lightGroup.get());
	viewProjection.eye = { 0,300,0 };
	viewProjection.up = { 0,0,1 };

	model = Model::Create("cube");

	skydome.Initialize(100.0f);
	for (auto& point : p)
	{
		point.Initialize();
		point.scale *= 2.0f;
	}
	box.Initialize();
	box.scale *= 5.0f;

	p[0].translation = { -100.0f, 0, 0 };
	p[1].translation = { 100.0f, 0, 0 };
	p[2].translation = { 0, 0, 100.0f };
	p[3].translation = { 0, 0, -100.0f };
}

void GamePlayScene::Update()
{
	PostEffect pe;

	std::vector<Vector3> points;
	points.push_back(p[0].translation);
	for (auto& point : p) { points.push_back(point.translation); }
	points.push_back(p[3].translation);

	t += 0.005f;
	if (t >= 1.0f)
	{
		if (startIndex < points.size() - 3)
		{
			startIndex++;
			t -= 1.0f;
		}
		else { t = 1.0f; }
	}

	box.translation = SplineCurve(points, startIndex, t);

	skydome.Update();
	uiDrawer.Update();
	for (auto& point : p) { point.Update(); }
	box.Update();
	viewProjection.Update();
	debugCamera->Update();
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	//skydome.Draw();
	for (auto& point : p) { model->Draw(point); }
	model->Draw(box);
	Model::PostDraw();

	uiDrawer.Draw();
}