#include "Gimmick.h"
#include <DirectXMath.h>
#include <imgui.h>

bool Gimmick::isStart_;
void Door::Initialize()
{
	model = Model::Create("door");
	worldTransform.Initialize();
	worldTransform.scale = { 2.0f,2.0f,2.0f };
	flip = worldTransform;
	flip.Initialize();
	flip.rotation.y = 180 * PI / 180;;//反転
	//ズレ調整	
	worldTransform.translation.x += 2.5;
	flip.translation.x -= 2.5;

	//開ける
	worldTransform.rotation.y = -90 * PI / 180;
	flip.rotation.y = 270 * PI / 180;

	input = Input::GetInstance();
}

void Door::Open()
{
	if (isOpen == true)
	{
		rot++;
		if (rot >= 90)
		{
			isOpen = false;
		}
	}

	worldTransform.rotation.y = -rot * PI / 180;
	flip.rotation.y = (rot+180) * PI / 180;
}

void Door::Close()
{
	if (isClose == true)
	{
		rot--;
		if (rot <= 0)
		{
			isClose = false;
			isStart_ = true;
		}
	}
	
	worldTransform.rotation.y = -rot * PI / 180;
	flip.rotation.y = (rot + 180) * PI / 180;
}

void Door::Update()
{
	//ドアを開く
	if(input->IsTrigger(Key::O)) {isOpen = true; }
	if (input->IsTrigger(Key::P)) {isClose = true; }

	Open();
	Close();

	ImGui::Text("iss:%d", isStart_);
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
	model = Model::Create("candle");
	worldTransform.Initialize();
	worldTransform.scale = { 2.0f,2.0f,2.0f };
}

void Candle::Update()
{
	worldTransform.Update();
}