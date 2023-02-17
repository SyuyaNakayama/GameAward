#pragma once
#include "ViewProjection.h"
#include "LightGroup.h"

class WorldTransform
{
private:
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		Matrix4 viewproj; // �r���[�v���W�F�N�V�����s��
		Matrix4 world; // ���[���h�s��
		Vector3 cameraPos; // �J�������W(���[���h���W)
	};

	// �r���[�v���W�F�N�V�����̃|�C���^
	static ViewProjection* viewProjection_;
	// ���C�g
	static std::unique_ptr<LightGroup> lightGroup;
public:
	Matrix4 matWorld;
	ConstBufferData* constMap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;
	Vector3 scale = { 1,1,1 }, rotation, translation;
	WorldTransform* parent = nullptr;

	void Initialize();
	void Update();
	Vector3 GetWorldPosition() { return { matWorld.m[3][0],matWorld.m[3][1],matWorld.m[3][2] }; }
	static void SetViewProjection(ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
	static void SetLightGroup(LightGroup* lightGroup) { WorldTransform::lightGroup.reset(lightGroup); }
	static ViewProjection* GetViewProjection() { return viewProjection_; }
	static LightGroup* GetLightGroup() { return lightGroup.get(); }
};