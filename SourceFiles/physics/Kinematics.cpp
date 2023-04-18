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
	assert(m); // 0‚ÅŠ„‚é‚Ì‚ð‘jŽ~
	px += vx;
	vx += ax;
	ax += fx / m;
}
