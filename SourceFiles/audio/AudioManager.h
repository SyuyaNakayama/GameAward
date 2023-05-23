#pragma once
#include "PointAudio.h"
#include <unordered_map>

enum class SEName {};
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
	static void Play(BGMName bgmName);
	static void Play(SEName seName) { se[seName].Play(); }
	static void Stop(BGMName bgmName) { bgm[bgmName].Stop(); }
	static void Stop(SEName seName) { se[seName].Stop(); }
};