﻿#include <random>
#include "ParticleManager.h"
#include "SpriteCommon.h"
#include "D3D12Common.h"
#include "WorldTransform.h"
using namespace Microsoft::WRL;
#include <imgui.h>
// 静的メンバ変数の実体
ComPtr<ID3D12RootSignature> ParticleManager::rootsignature;
ComPtr<ID3D12PipelineState> ParticleManager::pipelinestate;
ComPtr<ID3D12Resource> ParticleManager::vertBuff;
ParticleManager::VertexPos* ParticleManager::vertMap = nullptr;
ComPtr<ID3D12Resource> ParticleManager::constBuff;
ParticleManager::ConstBufferData* ParticleManager::constMap = nullptr;
Matrix4 ParticleManager::matBillboard;
D3D12_VERTEX_BUFFER_VIEW ParticleManager::vbView{};
uint32_t ParticleManager::textureIndex = 0;
DiffuseParticle ParticleManager::diffuseParticle;
DirectionalParticle ParticleManager::directionalParticle;

void ParticleManager::Initialize()
{
	// パイプライン初期化
	InitializeGraphicsPipeline();
	// テクスチャ読み込み
	textureIndex = SpriteCommon::GetInstance()->LoadTexture("Particle.png", 1);
	// モデル生成
	CreateBuffers();
}

void ParticleManager::InitializeGraphicsPipeline()
{
	PipelineManager pipelineManager;
	pipelineManager.LoadShaders(L"ParticleVS", L"ParticlePS", L"ParticleGS");
	pipelineManager.AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	pipelineManager.AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32B32_FLOAT);
	pipelineManager.InitDepthStencilState();
	pipelineManager.SetBlendDesc(D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE, D3D12_BLEND_ONE); // 光パーティクル
	//pipelineManager.SetBlendDesc(D3D12_BLEND_OP_REV_SUBTRACT, D3D12_BLEND_ONE, D3D12_BLEND_ONE); // 闇パーティクル
	pipelineManager.InitDSVFormat();
	pipelineManager.SetDepthWriteMask(D3D12_DEPTH_WRITE_MASK_ZERO);
	pipelineManager.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
	pipelineManager.AddRootParameter(PipelineManager::RootParamType::CBV);
	pipelineManager.AddRootParameter(PipelineManager::RootParamType::DescriptorTable);
	pipelineManager.CreatePipeline(pipelinestate, rootsignature);
}

void ParticleManager::CreateBuffers()
{
	CreateBuffer(&vertBuff, &vertMap, vertexCount);

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = vertexCount;
	vbView.StrideInBytes = sizeof(VertexPos);

	CreateBuffer(&constBuff, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
}

void ParticleManager::UpdateViewMatrix()
{
	ViewProjection* vp = WorldTransform::GetViewProjection();
	Vector3 cameraAxisZ = vp->target - vp->eye;
	// 0ベクトルの時
	assert(!(cameraAxisZ == Vector3(0, 0, 0)));
	assert(!(vp->up == Vector3(0, 0, 0)));

	cameraAxisZ.Normalize();

	Vector3 cameraAxisX = Normalize(Cross(vp->up, cameraAxisZ));
	Vector3 cameraAxisY = Normalize(Cross(cameraAxisZ, cameraAxisX));
	matBillboard = Matrix4::CreateFromVector(cameraAxisX, cameraAxisY, cameraAxisZ);
}

void ParticleManager::Update()
{
	diffuseParticle.Update();
	directionalParticle.Update();

	// 定数バッファへデータ転送
	std::list<DiffuseParticle::Particle> diffuse = diffuseParticle.GetParticles();
	std::list<DirectionalParticle::Particle> directional = directionalParticle.GetParticles();
	int i = 0;

	for (auto& dif : diffuse)
	{
		vertMap[i].pos = dif.position;
		vertMap[i++].scale = dif.scale;
	}
	for (auto& dir : directional)
	{
		vertMap[i].pos = dir.position;
		vertMap[i++].scale = 1.0f;
	}
	ImGui::Text("directionalParticleNum = %d", directionalParticle.GetParticles().size());
	UpdateViewMatrix();

	// 定数バッファへデータ転送
	constMap->mat = WorldTransform::GetViewProjection()->GetViewProjectionMatrix();
	constMap->matBillboard = matBillboard;
}

void ParticleManager::Draw()
{
	// コマンドリストをセット
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	// デスクリプタヒープの配列
	SpriteCommon* spCommon = SpriteCommon::GetInstance();
	ID3D12DescriptorHeap* ppHeaps[] = { spCommon->GetDescriptorHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, spCommon->GetGpuHandle(textureIndex));
	// 描画コマンド
	diffuseParticle.Draw();
	directionalParticle.Draw();
}

void ParticleManager::Clear()
{
	diffuseParticle.Clear();
	directionalParticle.Clear();
}