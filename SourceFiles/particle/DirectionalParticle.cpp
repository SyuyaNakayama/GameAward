#include "DirectionalParticle.h"
#include "Quaternion.h"
#include "DirectXCommon.h"
#include "SpriteCommon.h"

void DirectionalParticle::Particle::Update()
{
	position = BezierCurve(controlPoints, frame.GetRemainTimeRate());
}

void DirectionalParticle::Particle::ComputeControlPoints()
{
	// ��]�������߂�
	Vector3 axis = Normalize(end - start);
	// Quaternion����
	Quaternion rotQ = Quaternion::MakeAxisAngle(axis, angle);
	// ����_�̒ǉ�
	controlPoints.push_back(start);
	// ����_����`��ԂŌv�Z
	for (UINT16 i = 0; i < splitNum; i++)
	{
		// axis�̕���
		Vector3 controlPoint = Lerp(start, end, (float)(i + 1) / (float)(splitNum + 1));
		// ����_��y��������radius�����ړ�������
		controlPoint.y += radius;
		// Quaternion�͌��_���S�ɉ�]������̂ŁA��]���̎n�_�����_�Ƃ���
		controlPoint -= start;
		// ����_��Quaternion�ŉ�]������
		controlPoint = Quaternion::RotateVector(controlPoint, rotQ);
		// ���s�ړ��𑊎E
		controlPoint += start;
		controlPoints.push_back(controlPoint);
	}
	controlPoints.push_back(end);
}

void DirectionalParticle::Add(const DirectionalParticle::AddProp& particleProp)
{
	particles.emplace_front();
	Particle& p = particles.front();
	p.start = particleProp.start;
	p.end = particleProp.end;
	p.scale = particleProp.scale;
	p.splitNum = particleProp.splitNum;
	p.angle = particleProp.angle;
	p.radius = particleProp.radius;
	p.frame = particleProp.lifeTime;
	p.ComputeControlPoints();
}
#include <imgui.h>
#include "ImGuiManager.h"
void DirectionalParticle::Update()
{
	particles.remove_if([](Particle& particle) { return particle.frame.CountDown(); });
	for (auto& particle : particles) 
	{
		particle.Update(); 
		ImGuiManager::PrintVector("particle.position", particle.position);
		//ImGui::Text("particle = %d", particle.position);
	}
}