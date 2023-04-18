#include "Kinematics.h"
#include <cassert>

void Motion::Initialize(float px_, float vx_, float ax_, float fx_, float m_)
{
	px = px_;
	vx = vx_;
	ax = ax_;
	fx = fx_;
	m = m_;
}

void Motion::Update()
{
	assert(m); // 0�Ŋ���̂�j�~
	px += vx;
	vx += ax;
	ax += fx / m;
}
