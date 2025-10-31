#pragma once

struct PadInput {
    int lx, ly;          // 左スティック
    int rx, ry;          // 右スティック
    int lt, rt;          // トリガー
    bool buttons[16];    // A,B,X,Y,L,R,Back,Start,DPAD etc.
};

class InputManager {
public:
    // 2Pの入力を取得
    static bool GetPad2Input(PadInput& input);

    // 接続されているPadの数を返す
    static int GetConnectedPadCount();
};

