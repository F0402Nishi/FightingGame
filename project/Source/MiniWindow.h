#pragma once
#include <DxLib.h>
#include <vector>
#include "Object3D.h"

class SelectScene;
class PlayScene;

class MiniWindow : public Object3D
{
public:
	MiniWindow();
	~MiniWindow();
	void Update() override;
	void Draw() override;
	void CommandWindow();
	void SetLayout(const std::vector<int>& _postionY);
	void MoveBox(int _box);
	void DrawResultBox(int _rnumbers);

	int GetMenuOption() const;

	void ToggleCommand() { commandwindowOpen = !commandwindowOpen; }
	void ToggleMenu() { menuwindowOpen = !menuwindowOpen; }
	void ToggleReslut(int _result) { resultwindowOpen = _result; }
	void TogglePad(int _pad) { padwindowOpen = _pad; }
	bool IsCommandNow() { return commandwindowOpen; }
	bool IsMenuNow() { return menuwindowOpen; }
	bool IsResultNow() { return resultwindowOpen; }

	bool stickUp;
	bool stickDown;
private:
	SelectScene* selectScene;
	PlayScene* playScene;

	std::vector<int> Yposition;

	XINPUT_STATE inputCommand;

	int scrollOffset;
	int commandCount; // コマンドリストの項目数
	int itemHeight; // 1項目の高さ
	int windowHeight; // 枠の高さから余白を引く
	int KeyCount; // コマンドキーリストの項目数
	int optionBack;
	int optionArrow;
	int arrowX;
	int boxY;
	int r = 0, g = 80, b = 255;
	int ry;
	int Mx;
	int My;

	bool commandwindowOpen;
	bool menuwindowOpen;
	bool resultwindowOpen;
	bool padwindowOpen;
	bool windowUpKeyInput;
	bool windowDownKeyInput;
	bool listStat;
	bool listLast;

	bool firstOptionFrame;
	bool commandUpPressed;
	bool commandDownPressed;
	bool nowCUp;
	bool nowCDown;
	bool upCHit;
	bool downCHit;
};
