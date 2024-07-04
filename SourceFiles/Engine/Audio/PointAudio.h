#pragma once
#include "Audio.h"
#include "Vector.h"

class PointAudio : public Audio
{
private:
	Vector3 audioPos;
	Vector3 micPos;
	Vector3 forward;
	bool useCamera = true;
	bool usePan = true;
	float sPressRate = 1.0f; // 原曲との音圧の比率
	long panStrength = 2500; // パンの強さ(大きいほど左右の音量差が大きくなる)

public:
	/// <param name="useCamera">trueにするとカメラの位置がマイク座標になる</param>
	/// <param name="usePan">trueにすると音源が左右移動する</param>
	void Initialize(const std::string& fileName, Vector3 audioPos_ = {}, bool useCamera = true, bool usePan = true);
	void Update();
	void SetSoundPressRate(float soundPressRate) { sPressRate = soundPressRate; }
	void SetAudioPos(Vector3 pos) { audioPos = pos; }
	void SetMicPos(Vector3 pos) { micPos = pos; }
	void SetForward(Vector3 forward_) { forward = forward_; }
	void SetPanStrength(long panStrength_) { panStrength = panStrength_; }
};