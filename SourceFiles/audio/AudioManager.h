#pragma once
#include "PointAudio.h"
#include <unordered_map>

enum class SEName {};
enum class BGMName { Select, Tutorial, Play };

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
	static void Play(BGMName bgmName) { bgm[bgmName].Play(); }
	static void Play(SEName seName) { se[seName].Play(); }
};