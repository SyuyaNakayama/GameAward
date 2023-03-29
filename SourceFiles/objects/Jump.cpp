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
	// �����Ă��Ԃ���Ȃ���Ζ���
	if (jumpSpd >= 0) { return; }
	// ����R���C�_�[�̏���y���W���擾
	float pairPosY = collider->GetWorldPosition().y + collider->GetRadius().y;
	// �����W�Ƒ�����W��y���̍����v�Z
	float disY = std::abs(worldTransform.translation.y - pairPosY);
	// ���ꂪ������y�������̔��a�ȉ��Ȃ瓖�����Ă���
	if (worldTransform.scale.y >= disY)
	{
		// �W�����v��ԉ���
		worldTransform.translation.y = collider->GetWorldPosition().y;
		isJump = false;
	}
}
