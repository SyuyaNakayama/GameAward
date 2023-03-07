#include "Model.h"
#include "D3D12Common.h"

using namespace Microsoft::WRL;
using namespace std;

// 静的メンバ変数の実体
ComPtr<ID3D12PipelineState> Model::pipelinestate = nullptr;
ComPtr<ID3D12RootSignature> Model::rootsignature = nullptr;
LightGroup* Model::lightGroup;
std::list<Model*> Model::models;

void Model::InitializeGraphicsPipeline()
{
	PipelineManager pipelineManager;
	pipelineManager.LoadShaders(L"OBJVertexShader", L"OBJPixelShader");
	pipelineManager.AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	pipelineManager.AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	pipelineManager.AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	pipelineManager.AddInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
	pipelineManager.InitDepthStencilState();
	pipelineManager.InitDSVFormat();
	pipelineManager.SetBlendDesc(D3D12_BLEND_OP_ADD, D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_INV_SRC_ALPHA);
	pipelineManager.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	pipelineManager.AddRootParameter(PipelineManager::RootParamType::DescriptorTable);
	for (size_t i = 0; i < 3; i++) { pipelineManager.AddRootParameter(PipelineManager::RootParamType::CBV); }
	pipelineManager.CreatePipeline(pipelinestate, rootsignature);
}

std::unique_ptr<Model> Model::Create(const string& modelName, bool smoothing)
{
	unique_ptr<Model> newModel = make_unique<Model>();

	//for (auto& model : models)
	//{
	//	if (model->name.find(modelName) == string::npos) { continue; }
	//	if (model->isSmooth != smoothing) { continue; }
	//	unique_ptr<Sprite> newSprite = Sprite::Create(model->material.textureFilename);
	//	newModel->sprite = move(newSprite);
	//	newModel->mesh = model->mesh;
	//	newModel->material = model->material;
	//	newModel->CreateBuffers();
	//	return newModel;
	//}

	newModel->mesh.LoadOBJ(modelName, smoothing);
	newModel->mesh.CreateBuffers();
	models.push_back(newModel.get());
	return newModel;
}

void Model::PreDraw()
{
	// コマンドリストをセット
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// ライトの描画
	assert(lightGroup);
	lightGroup->Draw();
}

void Model::Draw(const WorldTransform& worldTransform)
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	cmdList->SetGraphicsRootConstantBufferView(1, worldTransform.constBuffer->GetGPUVirtualAddress());

	mesh.Draw();
}