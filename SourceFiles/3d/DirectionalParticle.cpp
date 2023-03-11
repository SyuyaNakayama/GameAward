#include "DirectionalParticle.h"
#include "Quaternion.h"

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
		// axis���̕���
		Vector3 controlPoint = Lerp(start, end, (float)(i + 1) / (float)(splitNum + 1));
		// ����_��y��������radius�����ړ�������
		controlPoint.y += radius;
		// ����_��Quaternion�ŉ�]������
		controlPoint = Quaternion::RotateVector(controlPoint, rotQ);
		controlPoints.push_back(controlPoint);
	}
	controlPoints.push_back(end);
}

void DirectionalParticle::Add(const AddDirectionalParticleProp& particleProp)
{
	//if (particles.size() >= 10) { return; }
	particles.emplace_front();
	Particle& p = particles.front();
	p.start = particleProp.start;
	p.end = particleProp.end;
	p.splitNum = particleProp.splitNum;
	p.angle = particleProp.angle;
	p.radius = particleProp.radius;
	p.frame = particleProp.lifeTime;
	p.ComputeControlPoints();
	p.worldTransform.Initialize();
	p.worldTransform.scale *= 0.2f;
}

void DirectionalParticle::Update()
{
	particles.remove_if([](Particle& particle) { return particle.frame.CountDown(); }); 
	for (auto& particle : particles)
	{
		particle.worldTransform.translation = BezierCurve(particle.controlPoints, particle.frame.GetRemainTimeRate());
		particle.worldTransform.Update();
	}
}

void DirectionalParticle::Draw()
{
	for (auto& particle : particles)
	{
		model->Draw(particle.worldTransform);
	}
}
