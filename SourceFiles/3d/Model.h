#pragma once
#include "WorldTransform.h"
#include "LightGroup.h"
#include "Mesh.h"

class Model : public Mesh
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// ���C�g
	static std::unique_ptr<LightGroup> lightGroup;
	// �ǂݍ��񂾃��f�����
	static std::list<Model*> models;
	std::string name;		// ���f����
	bool isSmooth = false;	// �X���[�W���O

public:
	~Model() { models.remove(this); }
	// �ÓI������
	static void StaticInitialize();
	// ���C�g�̍X�V
	static void LightUpdate() { lightGroup->Update(); }
	// �`��O����
	static void PreDraw();
	// �`��㏈��
	static void PostDraw() {}
	// ���f���쐬
	static std::unique_ptr<Model> Create(const std::string& modelName, bool smoothing = false);
	static LightGroup* GetLightGroup() { return lightGroup.get(); }
	void Draw(const WorldTransform& worldTransform);
};