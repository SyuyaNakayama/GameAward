#pragma once
#include <wrl.h>
#include <d3dx12.h>
#include "Matrix4.h"
#include "DiffuseParticle.h"
#include "DirectionalParticle.h"

class ParticleManager final
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// 頂点データ構造体
	struct VertexPos
	{
		Vector3 pos; // xyz座標
		float scale; // スケール
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Matrix4 mat;	// ３Ｄ変換行列
		Matrix4 matBillboard; // ビルボード行列
	};

	static const int VERTEX_COUNT = 2048; // 頂点数

	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	static VertexPos* vertMap;
	// ビルボード行列
	static Matrix4 matBillboard;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// 定数バッファ
	static ComPtr<ID3D12Resource> constBuff;
	static ConstBufferData* constMap;
	static uint32_t textureIndex;
	// 拡散するパーティクル
	static DiffuseParticle diffuseParticle;
	// 始点から終点へ向かうパーティクル
	static DirectionalParticle directionalParticle;

	// グラフィックパイプライン生成
	static void InitializeGraphicsPipeline();

	// モデル作成
	static void CreateBuffers();

	// ビュー行列を更新
	static void UpdateViewMatrix();

	static bool IsParticleMax() { return diffuseParticle.GetParticles().size() + directionalParticle.GetParticles().size() >= VERTEX_COUNT / sizeof(VertexPos); }

	ParticleManager() = delete;
	ParticleManager(const ParticleManager& obj) = delete;
public: // メンバ関数

	// 静的初期化
	static void Initialize();

	// 毎フレーム処理
	static void Update();

	// 描画
	static void Draw();

	// パーティクルの削除
	static void Clear();

	static void Add(const DiffuseParticle::AddProp& particleProp);
	static void Add(const DirectionalParticle::AddProp& particleProp);
};

