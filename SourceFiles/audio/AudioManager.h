#pragma once
#include "PointAudio.h"
#include <unordered_map>

enum class SEName
{
	PlayerJump, PlayerLanding, PlayerWalk,
	DoorOpen, CandleIgnition, Lever,
	BlockMove, KeyGet, KeyOpen, Clear,
	Button, Cursor
};
enum class BGMName { Select, Tutorial, Play };

class AudioManager
{
private:
	static std::unordered_map<BGMName, Audio> bgm;
	static std::unordered_map<SEName, PointAudio> se;

	static void LoadSound(BGMName bgmName, const std::string& fileName);
	static void LoadSound(SEName bgmName, const std::string& fileName);

public:
	static void LoadAll();
	static Audio* GetAudio(BGMName bgmName) { return &bgm[bgmName]; }
	static PointAudio* GetAudio(SEName seName) { return &se[seName]; }
	/// <param name="audioPos">�����̐ݒu�ꏊ</param>
	/// <param name="startPlayPos">�Đ����̍Đ��ʒu(�b�P�ʂŎw��)</param>
	/// <param name="isUseJudgeFlag">�֐����ĂԂ��тɍĐ���������s����</param>
	static void Play(BGMName bgmName);
	static void Play(SEName seName, const Vector3& audioPos, double startPlayPos = 0, bool isUseJudgeFlag = false);
	static void Stop(BGMName bgmName) { bgm[bgmName].Stop(); }
	static void Stop(SEName seName) { se[seName].Stop(); }
};