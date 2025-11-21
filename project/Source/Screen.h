#pragma once

/// <summary>
/// Windowの定義をします
/// </summary>
namespace Screen
{
	// --- 実際の画面サイズ ---
	static int WIDTH = 0;
	static int HEIGHT = 0;

	static const int VIRTUAL_WIDTH = 1280;
	static const int VIRTUAL_HEIGHT = 720;
	static const BOOL WINDOW_MODE = FALSE;
	static const char* WINDOW_NAME = "project";
	static const float WINDOW_EXTEND = 1.0f;

	// --- 解像度取得関数 ---
	inline void InitScreenSize()
	{
		WIDTH = GetSystemMetrics(SM_CXSCREEN);  // 横幅
		HEIGHT = GetSystemMetrics(SM_CYSCREEN); // 高さ
	}

	// --- 仮想座標 → 実画面座標に変換 ---
	inline int ToScreenX(int x) { return x * WIDTH / VIRTUAL_WIDTH; }
	inline int ToScreenY(int y) { return y * HEIGHT / VIRTUAL_HEIGHT; }
};
