#include "PipelineManager.h"
#include "Functions.h"
#include "DirectXCommon.h"

void PipelineManager::CreatePipeline(ComPtr<ID3D12PipelineState>& pipelinestate, ComPtr<ID3D12RootSignature>& rootsignature)
{
	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	pipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	pipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	if (gsBlob) { pipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get()); }

	// �T���v���}�X�N
	pipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	pipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	pipeline.BlendState.RenderTarget[0] = blenddesc;

	// ���_���C�A�E�g�̐ݒ�
	pipeline.InputLayout.pInputElementDescs = inputLayout.data();
	pipeline.InputLayout.NumElements = (UINT)inputLayout.size();

	pipeline.NumRenderTargets = 1;	// �`��Ώۂ�1��
	pipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	pipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Init_1_0(rootParams.size(), rootParams.data(), 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob, errorBlob;
	// �o�[�W������������̃V���A���C�Y
	Result result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));

	pipeline.pRootSignature = rootsignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&pipeline, IID_PPV_ARGS(&pipelinestate));
}

void PipelineManager::AddRootParameter(RootParamType paramType)
{
	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER rootParam{};
	switch (paramType)
	{
	case RootParamType::CBV:
		rootParam.InitAsConstantBufferView(shaderRegister++);
		break;
	case RootParamType::DescriptorTable:
		rootParam.InitAsDescriptorTable(1, &descriptorRange);
		break;
	}
	rootParams.push_back(rootParam);
}

void PipelineManager::AddInputLayout(LPCSTR semanticName, DXGI_FORMAT format)
{
	inputLayout.push_back(SetInputLayout(semanticName, format));
}

void PipelineManager::SetBlendDesc(D3D12_BLEND_OP blendOp, D3D12_BLEND SrcBlend, D3D12_BLEND DestBlend)
{
	blenddesc.BlendOp = blendOp;
	blenddesc.SrcBlend = SrcBlend;
	blenddesc.DestBlend = DestBlend;
}

void PipelineManager::LoadShaders(std::wstring vsShaderName, std::wstring psShaderName, std::wstring gsShaderName)
{
	LoadShader(&vsBlob, vsShaderName, "vs_5_0");
	LoadShader(&psBlob, psShaderName, "ps_5_0");
	if (!gsShaderName.empty()) { LoadShader(&gsBlob, gsShaderName, "gs_5_0"); }
}