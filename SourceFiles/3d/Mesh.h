#pragma once
#include "Sprite.h"
#include <vector>
#include <unordered_map>

// 形状データ
class Mesh
{
public:
	// 頂点データ構造体
	struct VertexData
	{
		Vector3 pos; // xyz座標
		Vector3 normal; // 法線ベクトル
		Vector2 uv;  // uv座標
		ColorRGBA color; // 色
	};

private:
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// std::を省略
	using string = std::string;

	// マテリアル
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

	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffer;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};
	// モデル名
	string modelName;
	// 頂点データ配列
	std::vector<VertexData> vertices;
	// 頂点インデックス配列
	std::vector<UINT16> indices;
	// 頂点バッファのマップ
	VertexData* vertMap = nullptr;
	// 頂点座標スムージング用データ
	std::unordered_map<UINT16, std::vector<UINT16>> smoothData;
	void LoadMaterial(const string& DIRECTORY_PATH, const string& FILENAME); // マテリアル読み込み
	// マテリアル
	Material material;
	// テクスチャ
	std::unique_ptr<Sprite> sprite;
	bool isSmooth = false;

public:
	void LoadOBJ(const std::string& modelName_, bool smoothing);
	void Update();
	void Draw();
	// バッファの生成
	void CreateBuffers();
	void SetSprite(std::unique_ptr<Sprite> sprite_) { sprite = move(sprite_); }
	Sprite* GetSprite() { return sprite.get(); }
	void AddSmoothData(UINT16 indexPosition, UINT16 indexVertex) { smoothData[indexPosition].emplace_back(indexVertex); }
	void CalculateSmoothedVertexNormals();
};
