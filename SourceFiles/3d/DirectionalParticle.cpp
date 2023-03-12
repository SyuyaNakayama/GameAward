#include "DirectionalParticle.h"
#include "Quaternion.h"
#include "DirectXCommon.h"

void DirectionalParticle::Particle::Update()
{
	position = BezierCurve(controlPoints, frame.GetRemainTimeRate());
}

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

void DirectionalParticle::Add(const DirectionalParticle::AddProp& particleProp)
{
	particles.emplace_front();
	Particle& p = particles.front();
	p.start = particleProp.start;
	p.end = particleProp.end;
	p.splitNum = particleProp.splitNum;
	p.angle = particleProp.angle;
	p.radius = particleProp.radius;
	p.frame = particleProp.lifeTime;
	p.ComputeControlPoints();
}

void DirectionalParticle::Update()
{
	particles.remove_if([](Particle& particle) { return particle.frame.CountDown(); });
	for (auto& particle : particles) { particle.Update(); }
}

void DirectionalParticle::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	cmdList->DrawInstanced((UINT)particles.size(), 1, 0, 0);
}
