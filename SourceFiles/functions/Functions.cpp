#include "Functions.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
using namespace std;

D3D12_INPUT_ELEMENT_DESC SetInputLayout(LPCSTR semanticName, DXGI_FORMAT format)
{
	D3D12_INPUT_ELEMENT_DESC inputLayout =
	{
		semanticName, 0, format, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	};
	return inputLayout;
}

void LoadShader(ID3DBlob** shaderBlob, wstring shaderName, LPCSTR target)
{
	ID3DBlob* errorBlob = nullptr;

	wstring fileName = L"Resources/shaders/" + shaderName + L".hlsl";
	//�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	HRESULT result = D3DCompileFromFile(
		fileName.c_str(), // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", target, // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		shaderBlob, &errorBlob);
	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		string error;
		error.resize(errorBlob->GetBufferSize());
		copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void Clamp(float& num, float min, float max)
{
	if (num < min) { num = min; }
	if (num > max) { num = max; }
}