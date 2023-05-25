#pragma once
#include "PointAudio.h"
#include <unordered_map>

enum class SEName
{
	PlayerJump, PlayerLanding, PlayerWalk,
	DoorOpen, CandleIgnition,
	Lever, BlockMove, KeyGet, KeyOpen
};
enum class BGMName { Select, Tutorial, Play, Clear };

class AudioManager
{
private:
	static std::unordered_map<BGMName, Audio> bgm;
	static std::unordered_map<SEName, PointAudio> se;

	static void LoadBGM(BGMName bgmName, const std::string& fileName);
	static void LoadSE(SEName seName, const std::string& fileName);

public:
	static void LoadAll();
	static Audio* GetAudio(BGMName bgmName) { return &bgm[bgmName]; }
	static PointAudio* GetAudio(SEName seName) { return &se[seName]; }
	/// <param name="audioPos">音源の設置場所</param>
	/// <param name="startPlayPos">再生時の再生位置(秒単位で指定)</param>
	/// <param name="isUseJudgeFlag">関数を呼ぶたびに再生中判定を行うか</param>
	static void Play(SEName seName, Vector3 audioPos, double startPlayPos = 0, bool isUseJudgeFlag = false);
	static void Play(BGMName bgmName);
	static void Stop(BGMName bgmName) { bgm[bgmName].Stop(); }
	static void Stop(SEName seName) { se[seName].Stop(); }
};