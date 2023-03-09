#pragma once
#include "Audio.h"
#include "ViewProjection.h"

class PointAudio
{
private:
	std::unique_ptr<Audio> audio;
	Vector3 audioPos, micPos;
	bool useCameraMic;
	float sPressRate = 1.0f; // 原曲との音圧の比率

public:
	// useCameraMicをtrueにするとカメラの位置がマイク座標になる
	static std::unique_ptr<PointAudio> Create(const std::wstring& fileName, Vector3 audioPos = {}, bool useCameraMic = true);
	void Update();
	void SetAudioPos(Vector3 pos) { audioPos = pos; }
	void SetMicPos(Vector3 pos) { micPos = pos; }
};