#pragma once

#include <list>
#include "SceneManager.h"
#include "GameObject.h"

/// <summary>
/// シーンの基底クラス
/// </summary>
/// <author>N.Hanai</author>
class SceneBase
{
public:
	SceneBase() {}
	virtual ~SceneBase() {}

	virtual void Update() {}
	virtual void Draw() {}

	// === Scene専用のKeyCode ===
	DxLib::XINPUT_STATE inputScene;

	int keyCounter[256];
	int Gx;
	int Gy;

	/// <summary>
	/// カウントアップ方式でシーン移動
	/// </summary>
	int decideTimer;

	/// <summary>
	/// シーンの移動承認用
	/// </summary>
	bool changeScene;

	/// <summary>
	/// ゲームパッドの接続状況取得用
	/// </summary>
	bool padNow;

	/// <summary>
	/// 決定音がなっているか
	/// </summary>
	bool decideSoundPlayed;

	// === 前フレームの状態を保持 ===
	bool firstFrame;
	bool wasBPressed;
	bool wasBackPressed;
	bool wasRightPressed;
	bool wasLeftPressed;
	bool wasUpPressed;
	bool wasDownPressed;

	// === 今フレームの状態 ===
	bool bPressed;
	bool backPressed;
	bool nowRight;
	bool nowLeft;
	bool nowUp;
	bool nowDown;

	// === 押した瞬間だけ ===
	bool bHit;
	bool backHit;
	bool rightHit;
	bool leftHit;
	bool upHit;
	bool downHit;

	/// <summary>
	/// Key状態のリセット＆取得
	/// </summary>
	void UpdateKey() {
		char keys[256]; // 今のキー状態
		GetHitKeyStateAll(keys); // 今のキー状態を取得

		for (int i = 0; i < 256; i++) {
			if (keys[i] != 0) { keyCounter[i]++; }
			else { keyCounter[i] = 0; }
		}
	}
};
