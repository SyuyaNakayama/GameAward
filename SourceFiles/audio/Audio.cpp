#include "Audio.h"
#include "D3D12Common.h"
#include <fstream>
#include <cassert>

void Audio::StaticInitialize()
{
	// COM��������
	Result result = CoInitialize(NULL);
}

void Audio::Initialize(const std::string& fileName, bool isLoop_)
{
	Result result;
	// FilterGraph�𐶐�
	result = CoCreateInstance(CLSID_FilterGraph,
		NULL, CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID*)&graphBuilder);

	// MediaControl�C���^�[�t�F�[�X�擾
	result = graphBuilder->QueryInterface(IID_IMediaControl, (LPVOID*)&mediaControl);
	result = graphBuilder->QueryInterface(IID_IMediaPosition, (LPVOID*)&mediaPosition);
	result = graphBuilder->QueryInterface(IID_IBasicAudio, (LPVOID*)&basicAudio);

	std::string fullPath = "Resources/audios/" + fileName;

	// ���C�h������ɕϊ������ۂ̕�����o�b�t�@�T�C�Y���v�Z
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, nullptr, 0);
	// ���C�h������ɕϊ�
	std::vector<wchar_t> wfilePath(filePathBufferSize);
	MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, wfilePath.data(), filePathBufferSize);

	// Graph�𐶐�
	result = mediaControl->RenderFile((BSTR)wfilePath.data());

	isLoop = isLoop_;
}

void Audio::Play()
{
	if (GetState() != State::Running) { mediaControl->Run(); }
	if (!isLoop) { return; }
	// ���[�v����ꍇ�͍Đ����ԂɒB������擪�ɖ߂�
	if (IsEnd()) { mediaPosition->put_CurrentPosition(0); }
}

Audio::State Audio::GetState()
{
	FILTER_STATE fs{};
	mediaControl->GetState(0, (OAFilterState*)&fs);
	switch (fs)
	{
	case FILTER_STATE::State_Stopped: return State::Stopped;
	case FILTER_STATE::State_Paused: return State::Paused;
	case FILTER_STATE::State_Running: return State::Running;
	default: assert(0);
	}
}

bool Audio::IsEnd()
{
	REFTIME duration = 0.0, current = 0.0;
	mediaPosition->get_Duration(&duration); // �Đ����Ԃ��擾
	mediaPosition->get_CurrentPosition(&current); // ���ݎ��Ԃ��擾
	return duration <= current;
}

void Audio::Finalize()
{
	CoUninitialize(); // COM�I��
}