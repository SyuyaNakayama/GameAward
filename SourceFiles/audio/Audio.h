#pragma once
#include <string>
#include <wrl.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfapi.h>
#include <dshow.h>
#include <memory>
#include <wrl.h>

class Audio
{
private:
	Microsoft::WRL::ComPtr<IGraphBuilder> graphBuilder;
	Microsoft::WRL::ComPtr<IMediaControl> mediaControl;
	Microsoft::WRL::ComPtr<IMediaPosition> mediaPosition;
	Microsoft::WRL::ComPtr<IBasicAudio> basicAudio;
	bool isLoop = false;

public:
	enum class State { Stopped, Paused, Running };

	virtual ~Audio() = default;
	static void StaticInitialize();
	void Initialize(const std::string& fileName, bool isLoop = false);
	void Play();
	void Stop() { mediaControl->Stop(); }
	void SetSpeed(double playSpd) { mediaPosition->put_Rate(playSpd); }
	void SetPlayPosition(double playPosition) { mediaPosition->put_CurrentPosition(playPosition); }
	// -10000(無音)~0(最大音量) : 単位 デシベル
	void SetVolume(long volume) { basicAudio->put_Volume(volume); }
	// -10000(左)〜10000(右)
	void SetBalance(long balance) { basicAudio->put_Balance(balance); }
	State GetState(); // 状態を得る
	bool IsEnd(); // 再生が終わったか
	static void Finalize();
};