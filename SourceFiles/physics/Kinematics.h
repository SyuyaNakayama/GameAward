#pragma once
// �^���w

// �^���̖@��
struct Motion
{
	float px = 0; // �ʒu
	float vx = 0; // ���x
	float ax = 0; // �����x
	float fx = 0; // ���̂ɓ�����
	float m = 1;  // ����

	void Initialize(float px, float vx, float ax = 0, float fx = 0, float m = 1);
	void Update();
};