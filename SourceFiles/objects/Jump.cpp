#include "Jump.h"
#include "Input.h"

void Jump::StartJump(float jumpV0, float endHeight)
{
	if (isJump_) { return; }
	jumpSpd_ = jumpV0;
	endHeight_ = endHeight;
	isJump_ = true;
	isFall = false;
}

void Jump::UpdateJump(float& y)
{
	if (!isJump_) { return; }
	y += jumpSpd_ + gravity_;
	jumpSpd_ -= gravity_;
	if (y < endHeight_ && jumpSpd_ < 0)
	{
		y = endHeight_;
		isJump_ = false;
	}

	// s—ñ‚ÌXV‚È‚Ç
	worldTransform.Update();
}

void Jump::StartFall()
{
	if (isFall) { return; }
	isFall = true;
	isJump_ = false;
	jumpSpd_ = 0;
}

void Jump::UpdateFall(float& y)
{
	if (!isFall) { return; }
	y += jumpSpd_;
	jumpSpd_ -= gravity_;
}