#pragma once
#include <DxLib.h>
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

	void ToggleCommand() { commandwindowOpen = !commandwindowOpen; }
	void ToggleMenu() { menuwindowOpen = !menuwindowOpen; }

private:
	SelectScene* selectScene;
	PlayScene* playScene;

	int scrollOffset;
	int commandCount; // コマンドリストの項目数
	int itemHeight; // 1項目の高さ
	int windowHeight; // 枠の高さから余白を引く
	int KeyCount; // コマンドキーリストの項目数
	int optionBack;

	bool commandwindowOpen;
	bool windowUpKeyInput;
	bool windowDownKeyInput;
	bool listStat;
	bool listLast;
	bool menuwindowOpen;
};
