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
	/// セレクト音
	/// </summary>
	int SelectSe;

	/// <summary>
	/// セレクト決定音
	/// </summary>
	int DecideSe;

	bool changeScene;

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
