#include "Jump.h"
#include "Input.h"

void Jump::Start(float jumpV0, float endHeight_)
{
	if (isJump) { return; }
	jumpSpd = jumpV0;
	endHeight = endHeight_;
	isJump = true;
}

void Jump::Update()
{
	if (!isJump) { return; }
	worldTransform.translation.y += jumpSpd;
	jumpSpd -= gravity;
	if (worldTransform.translation.y < endHeight && jumpSpd < 0)
	{
		//worldTransform.translation.y = endHeight;
		//isJump = false;
	}
	worldTransform.Update();
}

void Jump::OnCollision(BoxCollider* collider)
{
	// 落ちてる状態じゃなければ無視
	if (jumpSpd >= 0) { return; }
	// 相手コライダーの上底のy座標を取得
	float pairPosY = collider->GetWorldPosition().y + collider->GetRadius().y;
	// 自座標と相手座標のy軸の差を計算
	float disY = std::abs(worldTransform.translation.y - pairPosY);
	// それが自分のy軸方向の半径以下なら当たっている
	if (worldTransform.scale.y >= disY)
	{
		// ジャンプ状態解除
		worldTransform.translation.y = collider->GetWorldPosition().y;
		isJump = false;
	}
}
