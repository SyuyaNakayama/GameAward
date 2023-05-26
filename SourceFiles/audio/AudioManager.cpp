#include "AudioManager.h"

std::unordered_map<BGMName, Audio> AudioManager::bgm;
std::unordered_map<SEName, PointAudio> AudioManager::se;

void AudioManager::LoadSound(BGMName bgmName, const std::string& fileName)
{
	Audio newBgm;
	newBgm.Initialize("bgm/" + fileName);
	newBgm.SetVolume(-10000);
	bgm[bgmName] = newBgm;
}

void AudioManager::LoadSound(SEName seName, const std::string& fileName)
{
	PointAudio newBgm;
	newBgm.Initialize("se/" + fileName);
	newBgm.SetVolume(-10000);
	se[seName] = newBgm;
}

void AudioManager::LoadAll()
{
	// BGMì«Ç›çûÇ›
	LoadSound(BGMName::Select, "select.mp3");
	LoadSound(BGMName::Tutorial, "Tutorial.mp3");
	LoadSound(BGMName::Play, "Play.mp3");
	// SEì«Ç›çûÇ›
	LoadSound(SEName::PlayerJump, "PlayerJump.mp3");
	LoadSound(SEName::PlayerLanding, "PlayerLanding.mp3");
	LoadSound(SEName::PlayerWalk, "PlayerWalk.mp3");
	LoadSound(SEName::DoorOpen, "DoorOpen.mp3");
	LoadSound(SEName::CandleIgnition, "CandleIgnition.mp3");
	LoadSound(SEName::Lever, "Lever.mp3");
	LoadSound(SEName::BlockMove, "BlockMove.mp3");
	LoadSound(SEName::KeyGet, "KeyGet.wav");
	LoadSound(SEName::KeyOpen, "KeyOpen.wav");
	LoadSound(SEName::Clear, "Clear.mp3");
}

void AudioManager::Play(BGMName bgmName)
{
	if (bgm[bgmName].GetState() != Audio::State::Running) { bgm[bgmName].SetPlayPosition(0); }
	bgm[bgmName].Play();
}

void AudioManager::Play(SEName seName, const Vector3& audioPos, double startPlayPos, bool isUseJudgeFlag)
{
	if (isUseJudgeFlag && se[seName].GetState() == Audio::State::Running) { return; }
	se[seName].SetPlayPosition(startPlayPos);
	se[seName].SetAudioPos(audioPos);
	se[seName].Play();
}