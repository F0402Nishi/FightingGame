#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#pragma comment(lib, "Xinput.lib")
#include <Xinput.h>
#include <DxLib.h>
#include "InputManager.h"

bool InputManager::GetPad2Input(PadInput& input)
{
    ::XINPUT_STATE state;
    if (::XInputGetState(1, &state) != ERROR_SUCCESS) return false; // Pad2Ç™ê⁄ë±Ç≥ÇÍÇƒÇ¢Ç»Ç¢

    input.lx = state.Gamepad.sThumbLX;
    input.ly = state.Gamepad.sThumbLY;
    input.rx = state.Gamepad.sThumbRX;
    input.ry = state.Gamepad.sThumbRY;
    input.lt = state.Gamepad.bLeftTrigger;
    input.rt = state.Gamepad.bRightTrigger;

    input.buttons[0] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
    input.buttons[1] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
    input.buttons[2] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
    input.buttons[3] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
    input.buttons[4] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
    input.buttons[5] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
    input.buttons[6] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
    input.buttons[7] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
    input.buttons[8] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
    input.buttons[9] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
    input.buttons[10] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
    input.buttons[11] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;

    return true;
}

int InputManager::GetConnectedPadCount()
{
    int count = 0;
    for (int i = 0; i < 4; i++) {
        ::XINPUT_STATE state;
        if (::XInputGetState(i, &state) == ERROR_SUCCESS) count++;
    }
    return count;
}
