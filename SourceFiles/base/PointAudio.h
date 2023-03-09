#pragma once
#include "Audio.h"
#include "ViewProjection.h"

class PointAudio
{
private:
	std::unique_ptr<Audio> audio;
	Vector3 audioPos;
	Vector3 micPos;
	Vector3 forward;
	bool useCamera = true;
	bool usePan = true;
	float sPressRate = 1.0f; // ���ȂƂ̉����̔䗦
	long panStrength = 2500; // �p���̋���(�傫���قǍ��E�̉��ʍ����傫���Ȃ�)

public:
	/*
		useCamera��true�ɂ���ƃJ�����̈ʒu���}�C�N���W�ɂȂ�
		usePan��true�ɂ���Ɖ��������E�ړ�����
	*/
	static std::unique_ptr<PointAudio> Create(const std::wstring& fileName, Vector3 audioPos = {}, bool useCamera = true, bool usePan = true);
	void Update();
	void SetAudioPos(Vector3 pos) { audioPos = pos; }
	void SetMicPos(Vector3 pos) { micPos = pos; }
	void SetForward(Vector3 forward_) { forward = forward_; }
	void SetPanStrength(long panStrength_) { panStrength = panStrength_; }
};