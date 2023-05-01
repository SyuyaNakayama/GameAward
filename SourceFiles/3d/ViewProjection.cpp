#include "ViewProjection.h"
#include "D3D12Common.h"
#include <cmath>
#include <array>

void ViewProjection::Initialize()
{
	CreateBuffer(constBuffer.GetAddressOf(), &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
}

void ViewProjection::Update()
{
	// プロジェクション行列を求める
	matProjection = Matrix4::Zero();
	float fovAngleYRadian = fovAngleY * PI / 180.0f;
	matProjection.m[0][0] = 1.0f / (aspectRatio * std::tan(fovAngleYRadian / 2.0f));
	matProjection.m[1][1] = 1.0f / std::tan(fovAngleYRadian / 2.0f);
	matProjection.m[2][2] = farZ / (farZ - nearZ);
	matProjection.m[2][3] = 1.0f;
	matProjection.m[3][2] = -nearZ * farZ / (farZ - nearZ);

	// ビュー行列を求める
	std::array<Vector3, 3> axis;
	axis[(int)Axis::Z] = Normalize(target - eye);
	axis[(int)Axis::X] = Normalize(Cross(up, axis[(int)Axis::Z]));
	axis[(int)Axis::Y] = Normalize(Cross(axis[(int)Axis::Z], axis[(int)Axis::X]));
	Vector3 cameraMove;
	for (size_t i = 0; i < 3; i++) { cameraMove[i] = Dot(eye, axis[i]); }
	matView = Matrix4::CreateFromVector(axis[(int)Axis::X], axis[(int)Axis::Y], axis[(int)Axis::Z]);
	matView = Matrix4::Inverse(matView);
	for (size_t i = 0; i < 3; i++) { matView.m[3][i] = -cameraMove[i]; }

	constMap->viewproj = GetViewProjectionMatrix();
	constMap->cameraPos = eye;
}

void ViewProjection::CameraMove(const Vector3& move)
{
	eye += move;
	target += move;
}