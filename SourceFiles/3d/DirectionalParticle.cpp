#include "DirectionalParticle.h"
#include "Quaternion.h"

void DirectionalParticle::Particle::ComputeControlPoints()
{
	// 回転軸を求める
	Vector3 axis = Normalize(end - start);
	// Quaternion生成
	Quaternion rotQ = Quaternion::MakeAxisAngle(axis, angle);
	// 制御点の追加
	controlPoints.push_back(start);
	// 制御点を線形補間で計算
	for (UINT16 i = 0; i < splitNum; i++)
	{
		// axis軸の分割
		Vector3 controlPoint = Lerp(start, end, (float)(i + 1) / (float)(splitNum + 1));
		// 制御点をy軸方向にradiusだけ移動させる
		controlPoint.y += radius;
		// 制御点をQuaternionで回転させる
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
