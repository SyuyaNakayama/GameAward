#include "Audio.h"
#include "Functions.h"
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
		(LPVOID*)&newAudio->pGraphBuilder);

	// MediaControl�C���^�[�t�F�[�X�擾
	result = newAudio->pGraphBuilder->QueryInterface(IID_IMediaControl,
		(LPVOID*)&newAudio->pMediaControl);

	result = newAudio->pGraphBuilder->QueryInterface(IID_IMediaPosition,
		(LPVOID*)&newAudio->pMediaPosition);

	wstring fullpath = L"Resources/audios/" + fileName;
	// Graph�𐶐�
	result = newAudio->pMediaControl->RenderFile((BSTR)fullpath.c_str());

	return newAudio;
}

void Audio::Finalize()
{
	CoUninitialize(); // COM�I��
}