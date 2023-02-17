#pragma once
#include "Matrix4.h"
#include "WindowsAPI.h"

class ViewProjection
{
private:
	Matrix4 matView, matProjection;

public:
	Vector3 eye = { 0, 0, -50.0f }, target, up = Vector3::MakeYAxis();
	float fovAngleY = 45.0f; // ��p(�x)
	// �A�X�y�N�g��(��{��WindowsAPI::WIN_SIZE�ɏ���)
	float aspectRatio = WindowsAPI::WIN_SIZE.x / WindowsAPI::WIN_SIZE.y;
	float nearZ = 0.1f, farZ = 1000.0f; // �f��͈�

	void Update();
	void CameraMove(const Vector3& move);
	Matrix4 GetViewProjectionMatrix() const { return matView * matProjection; }
};
