#include "Audio.h"
#include "D3D12Common.h"
#include <fstream>
#include <cassert>
using namespace std;

void Audio::StaticInitialize()
{
	// COM��������
	Result result = CoInitialize(NULL);
}

std::unique_ptr<Audio> Audio::Create(const wstring& fileName)
{
	std::unique_ptr<Audio> newAudio = std::make_unique<Audio>();

	Result result;
	// FilterGraph�𐶐�
	result = CoCreateInstance(CLSID_FilterGraph,
		NULL, CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID*)&newAudio->graphBuilder);

	// MediaControl�C���^�[�t�F�[�X�擾
	result = newAudio->graphBuilder->QueryInterface(IID_IMediaControl, (LPVOID*)&newAudio->mediaControl);
	result = newAudio->graphBuilder->QueryInterface(IID_IMediaPosition, (LPVOID*)&newAudio->mediaPosition);
	result = newAudio->graphBuilder->QueryInterface(IID_IBasicAudio, (LPVOID*)&newAudio->basicAudio);

	wstring fullpath = L"Resources/audios/" + fileName;
	// Graph�𐶐�
	result = newAudio->mediaControl->RenderFile((BSTR)fullpath.c_str());

	return newAudio;
}

void Audio::Finalize()
{
	CoUninitialize(); // COM�I��
}