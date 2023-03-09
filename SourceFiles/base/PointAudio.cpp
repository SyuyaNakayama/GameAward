#include "PointAudio.h"
#include "WorldTransform.h"
#include <cmath>

std::unique_ptr<PointAudio> PointAudio::Create(const std::wstring& fileName, Vector3 audioPos, bool useCameraMic)
{
	std::unique_ptr<PointAudio> newAudio = std::make_unique<PointAudio>();
	newAudio->audio = Audio::Create(fileName);
	newAudio->useCameraMic = useCameraMic;
	newAudio->audioPos = audioPos;
	newAudio->audio->Play();
	return newAudio;
}

void PointAudio::Update()
{
	if (useCameraMic) { micPos = WorldTransform::GetViewProjection()->eye; }
	// �}�C�N���特���̋���
	float dis = Length(audioPos - micPos);
	// ������1�������ƁA�����̉��ʂ��傫���Ȃ�̂Œ���
	if (dis < 1.0f) { dis = 1.0f; }
	// ��������
	float dic = 20.0f * std::log10f(dis);
	if (dic <= -10000) { dic = -10000.0f; }
	audio->SetVolume(-(long)dic*100.0f);
}
