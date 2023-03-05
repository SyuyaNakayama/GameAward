#pragma once
#include "WorldTransform.h"
#include "LightGroup.h"
#include "Mesh.h"

class Model
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// std::���ȗ�
	using string = std::string;
	template <class T> using vector = std::vector<T>;

	// �}�e���A��
	struct Material
	{
		string name;
		Vector3 ambient = { 0.3f,0.3f,0.3f };
		Vector3 diffuse;
		Vector3 specular;
		float alpha = 1.0f;
		string textureFilename;
	};

	struct ConstBufferData
	{
		Vector3 ambient;
		float pad1;
		Vector3 diffuse;
		float pad2;
		Vector3 specular;
		float alpha;
	};
private:
	// �e�N�X�`��
	std::unique_ptr<Sprite> sprite;
	// ���f����
	string name;
	// ���b�V��
	Mesh mesh;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffer;
	// �}�e���A��
	Material material;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// ���C�g
	static LightGroup* lightGroup;
	// �ǂݍ��񂾃��f�����
	static std::list<Model*> models;
	bool isSmooth = false;

	void LoadFromOBJInternal(const string& modelName);
	void LoadMaterial(const string& DIRECTORY_PATH, const string& FILENAME); // �}�e���A���ǂݍ���
	void CreateBuffers();

public:
	~Model() { models.remove(this); }
	// �O���t�B�b�N�p�C�v���C������
	static void InitializeGraphicsPipeline();
	// �`��O����
	static void PreDraw();
	// �`��㏈��
	static void PostDraw() {}
	// ���f���쐬
	static std::unique_ptr<Model> Create(const string& modelName, bool smoothing = false);
	void SetSprite(Sprite* sprite_) { sprite.reset(sprite_); }
	static void SetLightGroup(LightGroup* lightGroup) { Model::lightGroup = lightGroup; }
	Sprite* GetSprite() { return sprite.get(); }
	static LightGroup* GetLightGroup() { return lightGroup; }
	const Mesh& GetMesh() const { return mesh; }
	void TextureUpdate() { TextureUpdate(sprite.get()); }
	void TextureUpdate(Sprite* sprite);
	void Draw(const WorldTransform& worldTransform){ Draw(worldTransform, sprite.get()); }
	void Draw(const WorldTransform& worldTransform, Sprite* sprite); // �e�N�X�`���̕ύX
};