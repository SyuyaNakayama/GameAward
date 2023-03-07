#pragma once
#include "WorldTransform.h"
#include "LightGroup.h"
#include "Mesh.h"

class Model
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	template <class T> using vector = std::vector<T>;

private:
	// ���b�V��
	Mesh mesh;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// ���C�g
	static LightGroup* lightGroup;
	// �ǂݍ��񂾃��f�����
	static std::list<Model*> models;


public:
	~Model() { models.remove(this); }
	// �O���t�B�b�N�p�C�v���C������
	static void InitializeGraphicsPipeline();
	// �`��O����
	static void PreDraw();
	// �`��㏈��
	static void PostDraw() {}
	// ���f���쐬
	static std::unique_ptr<Model> Create(const std::string& modelName, bool smoothing = false);
	static void SetLightGroup(LightGroup* lightGroup) { Model::lightGroup = lightGroup; }
	static LightGroup* GetLightGroup() { return lightGroup; }
	Mesh& GetMesh() { return mesh; }
	void Draw(const WorldTransform& worldTransform);
};