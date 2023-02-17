#pragma once
#include <string>
#include <memory>
#include <cassert>
#include "DirectXCommon.h"

template<class T> void CreateBuffer(ID3D12Resource** buff, T** map, UINT64 width)
{
	// �o�b�t�@�̐���
	Result result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(width),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(buff));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = (*buff)->Map(0, nullptr, (void**)map);
	// �q���������
	(*buff)->Unmap(0, nullptr);
}

D3D12_INPUT_ELEMENT_DESC SetInputLayout(LPCSTR semanticName, DXGI_FORMAT format);
void LoadShader(ID3DBlob** shaderBlob, std::wstring shaderName, LPCSTR target);
void Clamp(float& num, float min, float max);

// result�l���G���[�Ȃ玩���Ōx�����o��
struct Result
{
private:
	HRESULT result = S_OK;

public:
	Result() {}
	Result(const HRESULT& result) { operator=(result); }

	void operator=(HRESULT result_)
	{
		result = result_;
		assert(SUCCEEDED(result));
	}
};