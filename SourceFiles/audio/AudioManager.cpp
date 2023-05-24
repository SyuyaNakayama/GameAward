#include "AudioManager.h"

std::unordered_map<BGMName, Audio> AudioManager::bgm;
std::unordered_map<SEName, PointAudio> AudioManager::se;

void AudioManager::LoadBGM(BGMName bgmName, const std::string& fileName)
{
	Audio newBgm;
	newBgm.Initialize("bgm/" + fileName);
	newBgm.SetVolume(-10000);
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
	// BGM読み込み
	LoadBGM(BGMName::Select, "select.mp3");
	LoadBGM(BGMName::Tutorial, "Tutorial.mp3");
	LoadBGM(BGMName::Play, "Play.mp3");
	LoadBGM(BGMName::Clear, "Clear.mp3");
	// SE読み込み
	LoadSE(SEName::PlayerJump, "PlayerJump.mp3");
	LoadSE(SEName::PlayerLanding, "PlayerLanding.mp3");
	LoadSE(SEName::PlayerWalk, "PlayerWalk.mp3");
	LoadSE(SEName::DoorOpen, "DoorOpen.mp3");
	LoadSE(SEName::CandleIgnition, "CandleIgnition.mp3");
	LoadSE(SEName::Lever, "Lever.mp3");
	LoadSE(SEName::BlockMove, "BlockMove.mp3");
	LoadSE(SEName::KeyGet, "KeyGet.mp3");
	LoadSE(SEName::KeyOpen, "KeyOpen.mp3");
}

void AudioManager::Play(BGMName bgmName)
{
	if (bgm[bgmName].GetState() != Audio::State::Running) { bgm[bgmName].SetPlayPosition(0); }
	bgm[bgmName].Play();
}

void AudioManager::Play(SEName seName, Vector3 audioPos, double startPlayPos)
{
	se[seName].SetPlayPosition(startPlayPos);
	se[seName].SetAudioPos(audioPos);
	se[seName].Play();
}