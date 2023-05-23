#include "AudioManager.h"

std::unordered_map<BGMName, Audio> AudioManager::bgm;
std::unordered_map<SEName, PointAudio> AudioManager::se;

void AudioManager::LoadBGM(BGMName bgmName, const std::string& fileName)
{
	Audio newBgm;
	newBgm.Initialize("bgm/" + fileName);
	bgm[bgmName] = newBgm;
}

void AudioManager::LoadSE(SEName seName, const std::string& fileName)
{
	PointAudio newBgm;
	newBgm.Initialize("se/" + fileName);
	se[seName] = newBgm;
}

void AudioManager::LoadAll()
{
}