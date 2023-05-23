#include "Audio.h"
#include "D3D12Common.h"
#include <fstream>
#include <cassert>

void Audio::StaticInitialize()
{
	// COMを初期化
	Result result = CoInitialize(NULL);
}

void Audio::Initialize(const std::string& fileName)
{
	Result result;
	// FilterGraphを生成
	result = CoCreateInstance(CLSID_FilterGraph,
		NULL, CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID*)&graphBuilder);

	// MediaControlインターフェース取得
	result = graphBuilder->QueryInterface(IID_IMediaControl, (LPVOID*)&mediaControl);
	result = graphBuilder->QueryInterface(IID_IMediaPosition, (LPVOID*)&mediaPosition);
	result = graphBuilder->QueryInterface(IID_IBasicAudio, (LPVOID*)&basicAudio);

	std::string fullPath = "Resources/audios/" + fileName;

	// ワイド文字列に変換した際の文字列バッファサイズを計算
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, nullptr, 0);
	// ワイド文字列に変換
	std::vector<wchar_t> wfilePath(filePathBufferSize);
	MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, wfilePath.data(), filePathBufferSize);

	// Graphを生成
	result = mediaControl->RenderFile((BSTR)wfilePath.data());
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

void Audio::Finalize()
{
	CoUninitialize(); // COM終了
}