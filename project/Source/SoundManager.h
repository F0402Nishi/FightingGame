#pragma once
#include "DxLib.h"
#include <map>
#include <string>

/// <summary>
/// 音の一括制御
/// </summary>
class SoundManager
{
private:
    static std::map<std::string, int> soundHandles;

public:
/// <summary>
/// 音を読み込む
/// </summary>
/// <param name="key"></param>
/// <param name="path"></param>
    static void Load(const std::string& key, const std::string& path);

/// <summary>
/// 音を再生する
/// </summary>
/// <param name="key"></param>
/// <param name="playType"></param>
    static void Play(const std::string& key, int playType = DX_PLAYTYPE_BACK);

/// <summary>
/// 音を停止する
/// </summary>
/// <param name="key"></param>
    static void Stop(const std::string& key);

/// <summary>
/// 音量を変更する (0〜255)
/// </summary>
/// <param name="key"></param>
/// <param name="volume"></param>
    static void ChangeVolume(const std::string& key, int volume);

/// <summary>
/// 再生中かどうか確認する
/// </summary>
/// <param name="key"></param>
/// <returns></returns>
    static bool IsPlaying(const std::string& key);

/// <summary>
/// すべての音を削除する
/// </summary>
    static void DeleteAll();
};
