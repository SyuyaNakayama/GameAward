#pragma once
#include "Sprite.h"

// マテリアル
class Material
{
protected:
	std::string materialName;
	Vector3 ambient = { 0.3f,0.3f,0.3f };
	Vector3 diffuse;
	Vector3 specular;
	std::string textureFilename;

	struct ConstBufferData
	{
		Vector3 ambient;
		float pad1;
		Vector3 diffuse;
		float pad2;
		Vector3 specular;
		float alpha;
	};

	std::unique_ptr<Sprite> sprite;		// テクスチャ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;	// 定数バッファ
	ConstBufferData* constMap = nullptr;
	
public:
	void SetAnbient(Vector3 anbient_) { ambient = anbient_; }
	void LoadMaterial(const std::string& DIRECTORY_PATH, const std::string& FILENAME); // マテリアル読み込み
	void SetSprite(std::unique_ptr<Sprite> sprite_) { sprite = move(sprite_); }
	Sprite* GetSprite() { return sprite.get(); }
	void Initialize();
	void Update();
	void SetMaterial(Material* material);
};