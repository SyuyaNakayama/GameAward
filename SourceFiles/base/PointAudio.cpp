#include "PointAudio.h"
#include "WorldTransform.h"
#include <cmath>

std::unique_ptr<PointAudio> PointAudio::Create(const std::wstring& fileName, Vector3 audioPos, bool useCamera, bool usePan)
{
	std::unique_ptr<PointAudio> newAudio = std::make_unique<PointAudio>();
	newAudio->audio = Audio::Create(fileName);
	newAudio->useCamera = useCamera;
	newAudio->audioPos = audioPos;
	newAudio->usePan = usePan;
	newAudio->audio->Play();
	return newAudio;
}

void PointAudio::Update()
{
	if (useCamera)
	{
		ViewProjection* vp = WorldTransform::GetViewProjection();
		micPos = vp->eye;
		forward = Normalize(vp->target - vp->eye);
	}
	// �}�C�N���特���̋���
	Vector3 toMic = micPos - audioPos;
	float dis = toMic.Length();
	// ������1�������ƁA�����̉��ʂ��傫���Ȃ�̂Œ���
	if (dis < 1.0f) { dis = 1.0f; }
	// ��������
	float dic = 20.0f * std::log10f(dis);
	if (dic <= -10000) { dic = -10000.0f; }
	audio->SetVolume(-(long)dic * 100);
	// �����̍��E�ړ�(�p��)
	if (!usePan) { return; }
	// y��������������2�������ŊO�ς����
	Vector2 dVec[2];
	dVec[0] = { forward.x,forward.z };
	toMic.Normalize();
	dVec[1] = { toMic.x,toMic.z };
	float cross = Cross(dVec[0], dVec[1]);
	audio->SetBalance((long)(cross * panStrength));
}