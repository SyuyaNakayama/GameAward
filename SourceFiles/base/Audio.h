#pragma once
#include <string>
#include <wrl.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfapi.h>
#include <vector>
#include <dshow.h>
#include <memory>
#include <wrl.h>

class Audio final
{
private:
	Microsoft::WRL::ComPtr<IGraphBuilder> graphBuilder;
	Microsoft::WRL::ComPtr<IMediaControl> mediaControl;
	Microsoft::WRL::ComPtr<IMediaPosition> mediaPosition;
	Microsoft::WRL::ComPtr<IBasicAudio> basicAudio;
public:
	static void StaticInitialize();
	static std::unique_ptr<Audio> Create(const std::wstring& fileName);
	void Play() { mediaControl->Run(); SetPlayPosition(0); }
	void Stop() { mediaControl->Stop(); }
	void SetSpeed(double playSpd) { mediaPosition->put_Rate(playSpd); }
	void SetPlayPosition(double playPosition) { mediaPosition->put_CurrentPosition(playPosition); }
	// -10000(無音)~0(最大音量) : 単位db(デシベル)
	void SetVolume(long volume) { basicAudio->put_Volume(volume); } 
	static void Finalize();
};