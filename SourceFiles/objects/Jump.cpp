#include "Jump.h"
#include "Input.h"
#include "CollisionManager.h"
#include <imgui.h>

void Jump::Start(float jumpV0)
{
	if (isJump) { return; }
	jumpSpd = jumpV0;
	isJump = true;
	//collisionAttribute = CollisionAttribute::Player;
	//collisionMask = CollisionMask::Player;
}

void Jump::Update()
{
	ImGui::Text("isJump : %d", isJump);
	if (!isJump) { return; }
	pwt->translation.y += jumpSpd;
	pwt->Update();
	worldTransform = *pwt;
	jumpSpd -= gravity;
}

// �����̐^���̒n�ʂ����o
void Jump::OnCollision(BoxCollider* collider)
{
	// ����R���C�_�[�̏���y���W���擾
	float pairPosY = collider->GetWorldPosition().y + collider->GetRadius().y;
	// �����W�Ƒ�����W��y���̍����v�Z
	float disY = std::abs(worldTransform.translation.y - pairPosY);
	// ���ꂪ������y�������̔��a�����Ȃ璅�n���Ă���
	if (worldTransform.scale.y >= disY)
	{
		// �����Ă��Ԃ���Ȃ���Ζ���
		if (jumpSpd >= 0 || !isJump) { return; }
		// �W�����v��ԉ���
		worldTransform.translation.y = collider->GetWorldPosition().y;
		isJump = false;
	}
	else
	{
		Start(0);
	}

	//// �����Ă��Ԃ���Ȃ���Ζ���
	//if (jumpSpd >= 0 || !isJump) { return; }
	//// ����R���C�_�[�̏���y���W���擾
	//float pairPosY = collider->GetWorldPosition().y + collider->GetRadius().y;
	//// �߂荞�݂�����
	//// �n�ʂɂ߂肱��ł��邩�H
	//if (CollisionManager::CheckCollision2Boxes(this, collider))
	//{
	//	pwt->translation.y = pairPosY;
	//	// �W�����v��ԉ���
	//	isJump = false;
	//}
	//// �߂荞��łȂ��ăW�����v���łȂ��Ȃ痎��
	//else if (jumpSpd < 0)
	//{
	//	Start(0);
	//	isJump = true;
	//}
}
