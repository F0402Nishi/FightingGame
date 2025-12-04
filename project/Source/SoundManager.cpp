#include "SoundManager.h"

/// <summary>
/// 音データを保持するマップ
/// </summary>
std::map<std::string, int> SoundManager::soundHandles;

void SoundManager::Load(const std::string& key, const std::string& path)
{
    // 既にロード済みなら何もしない
    if (soundHandles.count(key) > 0) return;

    int handle = LoadSoundMem(path.c_str());
    if (handle != -1) { soundHandles[key] = handle; }
}

void SoundManager::Play(const std::string& key, int playType)
{
    if (soundHandles.count(key) > 0) { PlaySoundMem(soundHandles[key], playType); }
}

void SoundManager::Stop(const std::string& key)
{
    if (soundHandles.count(key) > 0) { StopSoundMem(soundHandles[key]); }
}

void SoundManager::ChangeVolume(const std::string& key, int volume)
{
    if (soundHandles.count(key) > 0) { ChangeVolumeSoundMem(volume, soundHandles[key]); }
}

bool SoundManager::IsPlaying(const std::string& key)
{
    return soundHandles.count(key) > 0 && CheckSoundMem(soundHandles[key]) == 1;
}

void SoundManager::DeleteAll()
{
    for (auto& s : soundHandles) { DeleteSoundMem(s.second); }
    soundHandles.clear();
}
