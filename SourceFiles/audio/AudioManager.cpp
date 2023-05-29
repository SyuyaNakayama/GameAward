#include "AudioManager.h"

std::unordered_map<BGMName, Audio> AudioManager::bgm;
std::unordered_map<SEName, PointAudio> AudioManager::se;

void AudioManager::LoadSound(BGMName bgmName, const std::string& fileName)
{
	Audio newBgm;
	newBgm.Initialize("bgm/" + fileName, true);
	//newBgm.SetVolume(-1000);
	newBgm.SetVolume(-10000);
	bgm[bgmName] = newBgm;
}

void AudioManager::LoadSound(SEName seName, const std::string& fileName)
{
	PointAudio newSe;
	newSe.Initialize("se/" + fileName);
	//newSe.SetVolume(-10000);
	se[seName] = newSe;
}

void AudioManager::LoadAll()
{
	// BGM読み込み
	LoadSound(BGMName::Select, "select.mp3");
	LoadSound(BGMName::Tutorial, "Tutorial.mp3");
	LoadSound(BGMName::Play, "Play.mp3");
	// SE読み込み
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
	LoadSound(SEName::Button, "button.mp3");
	LoadSound(SEName::Cursor, "cursor.mp3");
}

void AudioManager::Play(BGMName bgmName)
{
	bgm[bgmName].Play();
}

void AudioManager::Play(SEName seName, const Vector3& audioPos, double startPlayPos, bool isUseJudgeFlag)
{
	if (isUseJudgeFlag && se[seName].GetState() == Audio::State::Running) { return; }
	se[seName].SetPlayPosition(startPlayPos);
	se[seName].SetAudioPos(audioPos);
	se[seName].Play();
}