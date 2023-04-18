#pragma once
#include <array>
#include "Timer.h"
#include "Model.h"
#include "Input.h"

//���[�V����
class PlayerMotion
{
private:
	enum class PartId { Body, LegL, LegR, Num };

	Input* input;
	std::array<WorldTransform, (size_t)PartId::Num> modelsTrans_;
	std::array<std::unique_ptr<Model>, (size_t)PartId::Num> model_;
	bool isUp = true;
	Timer timer = 50;
	int walkNum = 0;
	std::array<Vector3, 5> motionTransTemps;
	float motionLerpTimeRate = 0;

	void(PlayerMotion::* Phase)();
	void ResetTranslation();
	void StandbyMotion();
	void WalkMotion();
	void SetMotionTransTemps();
	void LerpStandbyToWalk();
	void LerpWalkToStandby();
	void LerpMotion();

public:
	void Initialize(WorldTransform* parent);
	void TransformUpdate();
	void MotionUpdate();
	void Draw();
	void SetBodyRotation(Vector3 rotation) { modelsTrans_[(int)PartId::Body].rotation = rotation; }
	const Vector3& GetBodyRotation() { return modelsTrans_[(int)PartId::Body].rotation; }
};