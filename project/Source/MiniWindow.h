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

/// <summary>
/// バトル中のコマンドウィンドウを描画する。
/// commandList、KeyList、PadList の各項目をスクロール対応で表示し、
/// ウィンドウ枠と背景も描画する。
/// </summary>
	void CommandWindow();

/// <summary>
/// メニュー項目のY座標リストを設定し、
/// 選択バーの初期位置を最初の項目に合わせる。
/// </summary>
	void SetLayout(const std::vector<int>& _postionY);

/// <summary>
/// メニュー選択バーを上下に移動させる。
/// 範囲外の場合は循環させる。
/// </summary>
/// <param name="_box"></param>
	void MoveBox(int _box);

/// <summary>
/// 勝敗結果に応じてリザルト用バーの色を設定する。
/// </summary>
	void DrawResultBox(int _rnumbers);

/// <summary>
/// 現在の選択バー位置に対応するメニュー項目のインデックスを返す。
/// </summary>
/// <returns></returns>
	int GetMenuOption() const;

	void ToggleCommand() { commandwindowOpen = !commandwindowOpen; }
	void ToggleMenu() { menuwindowOpen = !menuwindowOpen; }
	void ToggleReslut(int _result) { resultwindowOpen = _result; }
	void TogglePad(int _pad) { padwindowOpen = _pad; }
	bool IsCommandNow() { return commandwindowOpen; }
	bool IsMenuNow() { return menuwindowOpen; }
	bool IsResultNow() { return resultwindowOpen; }

private:
	SelectScene* selectScene;
	PlayScene* playScene;

	std::vector<int> Yposition;

	DxLib::XINPUT_STATE inputCommand;

	int scrollOffset;
	int commandCount; // コマンドリストの項目数
	int KeyCount; // コマンドキーリストの項目数
	int PadCount; // ゲームパッドキーリストの項目数
	int itemHeight; // 1項目の高さ
	int windowHeight; // 枠の高さから余白を引く
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
	bool stickUp;
	bool stickDown;
	bool downCHit;
	bool changefont;
};
