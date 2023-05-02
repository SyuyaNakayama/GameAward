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
	matProjection.m[0][0] = 1.0f / (aspectRatio * std::tan(fovAngleY / 2.0f));
	matProjection.m[1][1] = 1.0f / std::tan(fovAngleY / 2.0f);
	matProjection.m[2][2] = farZ / (farZ - nearZ);
	matProjection.m[2][3] = 1.0f;
	matProjection.m[3][2] = -nearZ * farZ / (farZ - nearZ);

	// ビュー行列を求める
	matView = Matrix4::GetBillboard();
	std::array<Vector3, 3> axis = matView.Get3Vectors();
	matView = Matrix4::Inverse(matView);
	for (size_t i = 0; i < 3; i++) { matView.m[3][i] = -Dot(eye, axis[i]); }

	constMap->viewproj = GetViewProjectionMatrix();
	constMap->cameraPos = eye;
}

void ViewProjection::CameraMove(const Vector3& move)
{
	eye += move;
	target += move;
}