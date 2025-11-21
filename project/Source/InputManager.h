#pragma once
#include <DxLib.h>

// === ゲームパッド入力情報構造体 ===
struct PadInput {
    int lx, ly;          // 左スティック
    int rx, ry;          // 右スティック
    int lt, rt;          // トリガー
    bool buttons[16];    // A,B,X,Y,L,R,Back,Start,DPAD etc.
};

// === InputManager クラス ===
class InputManager {
public:
    // --- Pad2の入力を取得 ---
    static bool GetPad2Input(PadInput& input);

    // --- 接続されているPadの数を返す ---
    static int GetConnectedPadCount();
};

