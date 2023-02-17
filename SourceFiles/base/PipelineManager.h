#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include <vector>
#include <string>

struct PipelineManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline{};
	ComPtr<ID3DBlob> vsBlob, gsBlob, psBlob;
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	UINT shaderRegister = 0;
	std::vector<CD3DX12_ROOT_PARAMETER> rootParams;

public:
	enum class RootParamType { CBV, DescriptorTable };

	void LoadShaders(std::wstring vsShaderName, std::wstring psShaderName, std::wstring gsShaderName = L"");
	void AddInputLayout(LPCSTR semanticName, DXGI_FORMAT format);
	void AddRootParameter(RootParamType paramType);
	void SetBlendDesc(D3D12_BLEND_OP blendOp, D3D12_BLEND srcBlend, D3D12_BLEND destBlend);
	// �}�`�̌`��ݒ�
	void SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveTopologyType) { pipeline.PrimitiveTopologyType = primitiveTopologyType; }
	// �f�v�X�X�e���V���X�e�[�g
	void InitDepthStencilState() { pipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT); }
	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	void InitDSVFormat() { pipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; }
	void SetDepthWriteMask(D3D12_DEPTH_WRITE_MASK depthWriteMask) { pipeline.DepthStencilState.DepthWriteMask = depthWriteMask; }
	void CreatePipeline(ComPtr<ID3D12PipelineState>& pipelinestate, ComPtr<ID3D12RootSignature>& rootsignature);
};