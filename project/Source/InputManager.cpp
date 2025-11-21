#include "InputManager.h"

// === Pad2“ü—Í‚Ìæ“¾ ===
bool InputManager::GetPad2Input(PadInput& input)
{
    DxLib::XINPUT_STATE states{};
    if (GetJoypadXInputState(DX_INPUT_PAD2, &states) != 0) return false; // Ú‘±‚È‚µ
    // if (::XInputGetState(1, &states) != ERROR_SUCCESS) return false; // Pad2‚ªÚ‘±‚³‚ê‚Ä‚¢‚È‚¢

    input.lx = states.ThumbLX;
    input.ly = states.ThumbLY;
    input.rx = 0;
    input.ry = 0;
    input.lt = states.LeftTrigger;
    input.rt = states.RightTrigger;

    for (int i = 0; i < 16; i++) {
        input.buttons[i] = states.Buttons[i];
    }

    return true;
#if 0
    input.lx = states.Gamepad.sThumbLX;
    input.ly = states.Gamepad.sThumbLY;
    input.rx = states.Gamepad.sThumbRX;
    input.ry = states.Gamepad.sThumbRY;
    input.lt = states.Gamepad.bLeftTrigger;
    input.rt = states.Gamepad.bRightTrigger;

    input.buttons[0] = (states.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
    input.buttons[1] = (states.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
    input.buttons[2] = (states.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
    input.buttons[3] = (states.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
    input.buttons[4] = (states.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
    input.buttons[5] = (states.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
    input.buttons[6] = (states.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
    input.buttons[7] = (states.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
    input.buttons[8] = (states.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
    input.buttons[9] = (states.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
    input.buttons[10] = (states.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
    input.buttons[11] = (states.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
#endif // 0
}

// === Ú‘±‚³‚ê‚Ä‚¢‚éPad”‚ğæ“¾ ===
int InputManager::GetConnectedPadCount()
{
    int count = 0;
    DxLib::XINPUT_STATE state{};

    if (GetJoypadXInputState(DX_INPUT_PAD1, &state) == 0) count++;
    if (GetJoypadXInputState(DX_INPUT_PAD2, &state) == 0) count++;
    if (GetJoypadXInputState(DX_INPUT_PAD3, &state) == 0) count++;
    if (GetJoypadXInputState(DX_INPUT_PAD4, &state) == 0) count++;

    return count;
}
