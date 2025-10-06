#pragma once

/// <summary>
/// シーンの基底クラス
/// </summary>
/// <author>N.Hanai</author>

#include <list>
#include "SceneManager.h"
#include "GameObject.h"

class SceneBase
{
public:
	SceneBase() {}
	virtual ~SceneBase() {}

	virtual void Update() {}
	virtual void Draw() {}

	// === Scene専用のKeyCode ===
	int keyCounter[256];

	void UpdateKey() {
		char keys[256]; // 今のキー状態
		GetHitKeyStateAll(keys); // 今のキー状態を取得

		for (int i = 0; i < 256; i++) {
			if (keys[i] != 0) { keyCounter[i]++; }
			else { keyCounter[i] = 0; }
		}
	}
};
