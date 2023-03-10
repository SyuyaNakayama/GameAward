#include "Gimmick.h"
#include "ImGuiManager.h"
#include "Input.h"
#include <imgui.h>

bool Gimmick::isStart_;
bool Gimmick::isGoal_;
LightGroup* Gimmick::lightGroup = nullptr;
size_t Candle::lightNum = 0;

/// <summary>
/// Door�N���X�̏���������
/// </summary>
void Door::Initialize()
{
	//���f���ǂݍ���
	model = Model::Create("door");
	model_back = Model::Create("door_back");

	//�e���f����worldTransform�������ƃ��f���̈ʒu����
	worldTransform.Initialize();
	worldTransform.translation.y += 2.5f;
	worldTransform.translation.z += 0.2f;
	worldTransform.scale = { 1.8f,1.9f,2.0f };
	doorL = worldTransform;
	doorR = worldTransform;
	doorR.Initialize();
	doorL.Initialize();
	doorL.scale = { 2.0f,2.0f,2.0f };			// �傫���𒲐�
	doorR.scale = { 2.0f,2.0f,2.0f };
	doorL.translation += {-2.5f, -2.5f, 0.0f};	// ���W�𒲐�
	doorR.translation += { 2.5f, -2.5f, 0.0f};

	//�J����
	doorR.rotation.y = -90 * PI / 180;
	doorL.rotation.y = 270 * PI / 180;

	input = Input::GetInstance();
}

/// <summary>
/// �h�A���J��
/// </summary>
void Door::Open()
{
	if (isOpen)
	{
		if (++rot >= 90)
		{
			isOpen = false;
			isOpened = true;
		}
		else
		{
			isOpened = false;
		}
	}

	doorL.rotation.y = -rot * PI / 180;
	doorL.rotation.y = (rot + 180) * PI / 180;
}

/// <summary>
/// �h�A�����
/// </summary>
void Door::Close()
{
	if (isClose)
	{
		if (--rot <= 0)
		{
			isClose = false;
			isStart_ = true;
		}
	}

	doorR.rotation.y = -rot * PI / 180;
	doorL.rotation.y = (rot + 180) * PI / 180;
}

/// <summary>
/// �h�A�ɓ���������
/// </summary>
void Door::OnCollision(BoxCollider* boxCollider)
{
	if (isOpened) { isGoal_ = true; }//�h�A���󂢂Ă��鎞�S�[��
}

/// <summary>
/// �h�A�̍X�V����
/// </summary>
void Door::Update()
{
	//�h�A���J��
	if (input->IsTrigger(Key::O)) { isOpen = true; }
	if (input->IsTrigger(Key::P)) { isClose = true; }

	Open();
	Close();

	// �S�[������
	// Player�̃��C�g�C���f�b�N�X�͔�΂�
	bool goalFlag = true;
	for (size_t i = 1; i <= Candle::GetLightNum(); i++)
	{
		goalFlag &= lightGroup->GetPointLightActive(i);
	}
	isOpen = goalFlag;

	ImGui::Text("isOpen : %d", isOpen);

	doorR.Update();
	doorL.Update();
	worldTransform.Update();
	ImGui::Text("isGoal = %d", isGoal_);
	ImGui::Text("isOpened = %d", isOpened);
}

/// <summary>
/// �h�A�̕`�揈��
/// </summary>
void Door::Draw()
{
	model->Draw(doorR);
	model->Draw(doorL);
	model_back->Draw(worldTransform);
}

void Candle::Initialize()
{
	model = Model::Create("candle", true);
	worldTransform.Initialize();
	worldTransform.scale = { 2.0f,2.0f,2.0f };
	lightGroup = Model::GetLightGroup();
	particleProp =
	{
		lightPos,{0,0.01f,0},{0,0.0005f,0},
		0.025f,0.001f,0,40,0.4f
	};
	lightPos = worldTransform.translation + Vector3(0, worldTransform.scale.y + 1.2f);
}

void Candle::Update()
{
	//ImGuiManager::DragVector("candlePos", worldTransform.translation);
	worldTransform.Update();

	lightGroup->SetPointLightActive(lightIndex, isLight);
	if (isLight)
	{
		model->SetAnbient({ 0.7f,0.3f,0.3f });
		lightPos = worldTransform.translation + Vector3(0, worldTransform.scale.y + 1.2f);

		particleProp.posOffset = lightPos;
		//ImGuiManager::DragVector("particleVelOffset", particleProp.velOffset, 0.001f);
		//ImGuiManager::DragVector("particleAccOffset", particleProp.accOffset, 0.00001f);
		//ImGui::DragFloat("particlePosRange", &particleProp.posRange, 0.002f, 0.002f, 2.0f);
		//ImGui::DragFloat("particleVelRange", &particleProp.velRange, 0.0001f, 0.0001f, 0.1f);
		//ImGui::DragFloat("particleAccRange", &particleProp.accRange, 0.00001f, 0.00001f, 0.01f);
		//ImGui::DragInt("particleLifeTime", &particleProp.lifeTime, 1, 1, 1000);
		//ImGui::DragFloat("particleStartScale", &particleProp.start_scale, 0.05f, 0.05f, 10.0f);

		lightGroup->SetPointLightPos(lightIndex, lightPos);
		lightGroup->SetPointLightAtten(lightIndex, { 0.2f, 0.01f });
		lightGroup->SetPointLightColor(lightIndex, { 1,0.5f,0.5f });
		ParticleManager::Add(particleProp);
	}
	else
	{
		model->SetAnbient({ 0,0,0 });
	}
	model->Update();
}

void Candle::Draw()
{
	model->Draw(worldTransform);
}

void Candle::OnCollision(RayCollider* rayCollider)
{
	if (!Input::GetInstance()->IsTrigger(Mouse::Left)) { return; }
	if (Length(rayCollider->GetWorldPosition() - worldTransform.GetWorldPosition()) < 8.0f)
	{
		isLight = !isLight;
	}
}

Wall::Wall(Vector3 scale)
{
	SetScale(scale);
}

void Wall::Initialize()
{
	model = Model::Create("cube", true);
	worldTransform.Initialize();
}

void Wall::Update()
{
	worldTransform.Update();
}