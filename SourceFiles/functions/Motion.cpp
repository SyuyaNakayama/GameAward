#include "Motion.h"
#include "ImGuiManager.h"

void PlayerMotion::ResetTranslation()
{
	modelsTrans_[(int)PartId::Body].translation = { 0.0f,0.3f,0.0f };
	for (int i = (int)PartId::LegL; i <= (int)PartId::LegR; i++)
	{
		modelsTrans_[i].translation = { 0.0f,-0.15f,0.0f };
		modelsTrans_[i].rotation.x = 0;
	}
}

void PlayerMotion::StandbyMotion()
{
	Vector3 moveBody;
	Vector3 moveLeg;
	float rot = 0;
	if (timer.CountDown())
	{
		isUp = !isUp;
		if (!isUp) { timer = 25; }
		else { timer = 50; }
	}

	float time = timer.GetInterval();

	// �W��(1 or -1)
	int k = isUp ? 1 : -1;

	moveBody.y = 0.1f / time * k;
	moveLeg.y = 0.2f / time * k;
	moveLeg.z = 0.5f / time * k;
	rot = 20.0f / time * k;

	// ��
	modelsTrans_[(int)PartId::Body].translation += moveBody;
	// ��
	for (int i = (int)PartId::LegL; i <= (int)PartId::LegR; i++)
	{
		modelsTrans_[i].translation += moveLeg;
		modelsTrans_[i].rotation.x += rot * PI / 180;
	}

	if (input->IsTrigger(Key::A) || input->IsTrigger(Key::S) ||
		input->IsTrigger(Key::D) || input->IsTrigger(Key::W))
	{
		isUp = true;
		timer = 20;
		ResetTranslation();
		Phase = &PlayerMotion::WalkMotion;
	}
}

void PlayerMotion::WalkMotion()
{
	Vector3 moveBody;
	Vector3 moveLeg;
	float rotR = 0;
	float rotL = 0;
	float time = timer.GetInterval();
	if (timer.CountDown())
	{
		walkNum = NumberLoop(walkNum + 1, 3);
	}

	// fraction.x = moveLeg.y�̕��q
	// fraction.y = moveLeg.z�̕��q
	// fraction.z = rotR�̕��q
	auto& WalkMotionFunc = [&](Vector3 fraction)
	{
		moveLeg.y = fraction.x / time;
		moveLeg.z = fraction.y / time;
		rotR = fraction.z / time;
		rotL = -rotR;
	};

	switch (walkNum / 2)
	{
	case 0: // �O��
		WalkMotionFunc(Vector3(0.35f, 0.5f, -20.0f) * (walkNum % 2 ? -1 : 1));
		break;
	case 1: // ����
		WalkMotionFunc(Vector3(0.35f, -0.5f, 20.0f) * (walkNum % 2 ? -1 : 1));
		break;
	}

	//���f����
	////��
	//modelsTrans_[(int)PartId::Body].translation += moveBody;
	//����
	modelsTrans_[(int)PartId::LegL].translation += moveLeg;
	modelsTrans_[(int)PartId::LegL].rotation.x += rotR * PI / 180;
	//�E��
	modelsTrans_[(int)PartId::LegR].translation.y += moveLeg.y;
	modelsTrans_[(int)PartId::LegR].translation.z -= moveLeg.z;
	modelsTrans_[(int)PartId::LegR].rotation.x += rotL * PI / 180;

	if (input->IsUp(Key::A) || input->IsUp(Key::S) ||
		input->IsUp(Key::D) || input->IsUp(Key::W))
	{
		if (!input->IsInput(Key::A) && !input->IsInput(Key::S) &&
			!input->IsInput(Key::D) && !input->IsInput(Key::W))
		{
			walkNum = 0;
			timer = 50;
			ResetTranslation();
			Phase = &PlayerMotion::StandbyMotion;
		}
	}
}

void PlayerMotion::SetMotionTransTemps()
{
	motionLerpTimeRate = 0;
	motionTransTemps[0] = modelsTrans_[(int)PartId::Body].translation;
	motionTransTemps[1] = modelsTrans_[(int)PartId::LegR].translation;
	motionTransTemps[2] = modelsTrans_[(int)PartId::LegL].translation;
	motionTransTemps[3] = modelsTrans_[(int)PartId::LegR].rotation;
	motionTransTemps[4] = modelsTrans_[(int)PartId::LegL].rotation;
}

void PlayerMotion::LerpStandbyToWalk()
{
	motionLerpTimeRate += 1.0f / 30.0f;
	if (motionLerpTimeRate >= 1.0f)
	{
		motionLerpTimeRate = 1.0f;
	}
	LerpMotion();
}

void PlayerMotion::LerpWalkToStandby()
{
	motionLerpTimeRate += 1.0f / 30.0f;
	if (motionLerpTimeRate >= 1.0f)
	{
		motionLerpTimeRate = 1.0f;
	}
	LerpMotion();
}

void PlayerMotion::LerpMotion()
{
	//����
	modelsTrans_[(int)PartId::LegL].translation = Lerp(motionTransTemps[2], {}, motionLerpTimeRate);
	modelsTrans_[(int)PartId::LegL].rotation = Lerp(motionTransTemps[4], {}, motionLerpTimeRate);
	//�E��
	modelsTrans_[(int)PartId::LegR].translation = Lerp(motionTransTemps[1], {}, motionLerpTimeRate);
	modelsTrans_[(int)PartId::LegR].rotation = Lerp(motionTransTemps[3], {}, motionLerpTimeRate);
}

void PlayerMotion::Initialize(WorldTransform* parent)
{
	input = Input::GetInstance();
	// ���f���ǂݍ���
	model_[(int)PartId::Body] = Model::Create("player_body", true);		//��
	model_[(int)PartId::LegL] = Model::Create("player_shoesL", true);	//����
	model_[(int)PartId::LegR] = Model::Create("player_shoesR", true);	//�E��
	// ���[���h�g�����X�t�H�[��
	for (auto& w : modelsTrans_) { w.Initialize(); }
	// �e�q�֌W
	modelsTrans_[(int)PartId::Body].parent = parent;
	modelsTrans_[(int)PartId::Body].scale = { 0.5f,0.5f,0.5f };
	modelsTrans_[(int)PartId::Body].translation = { 0.0f,0.3f,0.0f };
	for (int i = (int)PartId::LegL; i <= (int)PartId::LegR; i++)
	{
		modelsTrans_[i].parent = &modelsTrans_[(int)PartId::Body];
		modelsTrans_[i].translation = { 0.0f,-0.15f,0.0f };
	}
	Phase = &PlayerMotion::StandbyMotion;
}

void PlayerMotion::TransformUpdate()
{
	for (auto& w : modelsTrans_) { w.Update(); }
}

void PlayerMotion::MotionUpdate()
{
	(this->*Phase)();
}

void PlayerMotion::Draw()
{
	for (size_t i = 0; i < modelsTrans_.size(); i++) { model_[i]->Draw(modelsTrans_[i]); }
}