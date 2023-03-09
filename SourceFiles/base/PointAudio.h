#pragma once
#include "Audio.h"
#include "ViewProjection.h"

class PointAudio
{
private:
	std::unique_ptr<Audio> audio;
	Vector3 audioPos, micPos;
	bool useCameraMic;
	float sPressRate = 1.0f; // ���ȂƂ̉����̔䗦

public:
	// useCameraMic��true�ɂ���ƃJ�����̈ʒu���}�C�N���W�ɂȂ�
	static std::unique_ptr<PointAudio> Create(const std::wstring& fileName, Vector3 audioPos = {}, bool useCameraMic = true);
	void Update();
	void SetAudioPos(Vector3 pos) { audioPos = pos; }
	void SetMicPos(Vector3 pos) { micPos = pos; }
};