#include "PostEffect.h"
#include "SpriteCommon.h"
#include "D3D12Common.h"
#include "WindowsAPI.h"

void PostEffect::Initialize()
{
	Result result;
	const Vector2 WIN_SIZE = WindowsAPI::WIN_SIZE;
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	Sprite::Initialize(SpriteCommon::GetInstance()->LoadTexture("white1x1.png"));

	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM, WIN_SIZE.x, WIN_SIZE.y,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	const UINT PIXEL_COUNT = WIN_SIZE.x * WIN_SIZE.y;
	const UINT ROW_PITCH = sizeof(UINT) * WIN_SIZE.x;
	const UINT DEPTH_PITCH = ROW_PITCH * WIN_SIZE.y;
	UINT* img = new UINT[PIXEL_COUNT];
	for (size_t i = 0; i < PIXEL_COUNT; i++) { img[i] = 0xff0000ff; }

	result = texBuff->WriteToSubresource(0, nullptr, img, ROW_PITCH, DEPTH_PITCH);
	delete[] img;

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = 1;
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&descHeapSRV));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	device->CreateShaderResourceView(texBuff.Get(), &srvDesc, descHeapSRV->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::Draw()
{
	if (isInvisible_) { return; }
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	// パイプラインステートとルートシグネチャの設定コマンド
	cmdList->SetPipelineState(spriteCommon->GetPipelineState());
	cmdList->SetGraphicsRootSignature(spriteCommon->GetRootSignature());
	// プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト
	// デスクリプタヒープの設定コマンド
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	
	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	// 描画コマンド
	cmdList->DrawInstanced((UINT)vertices.size(), 1, 0, 0); // 全ての頂点を使って描画
}
