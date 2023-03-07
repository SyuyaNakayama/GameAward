#pragma once
#include "WorldTransform.h"
#include "LightGroup.h"
#include "Mesh.h"

class Model : public Mesh
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// ライト
	static LightGroup* lightGroup;
	// 読み込んだモデル情報
	static std::list<Model*> models;
	std::string name;		// モデル名
	bool isSmooth = false;	// スムージング

public:
	~Model() { models.remove(this); }
	// グラフィックパイプライン生成
	static void InitializeGraphicsPipeline();
	// 描画前処理
	static void PreDraw();
	// 描画後処理
	static void PostDraw() {}
	// モデル作成
	static std::unique_ptr<Model> Create(const std::string& modelName, bool smoothing = false);
	static void SetLightGroup(LightGroup* lightGroup) { Model::lightGroup = lightGroup; }
	static LightGroup* GetLightGroup() { return lightGroup; }
	void Draw(const WorldTransform& worldTransform);
};