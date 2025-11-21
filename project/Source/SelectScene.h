#pragma once
#include "../Library/SceneBase.h"
#include "Fade.h"
#include "MiniWindow.h"

/// <summary>
/// セレクトシーン
/// CommandImageを表示して、キーを押したらOperationImageを表示。
/// BattleImageを表示して、キーを押したらプレイシーンに移行する。
/// </summary>
class SelectScene : public SceneBase
{
public:
	SelectScene();
	~SelectScene();
	void Update() override;
	void Draw() override;
	void KeyMovement();

	static int gameType;
	// bool GetMiniWindow() const { return isWindowOpen; }

private:
	Fade* fade;
	MiniWindow* miniwindow;

	// int OperationImage;
	int CommandImage;
	int BattleImage;
	int SelectionArrowImage;
	int SelectBackImage;
	int Xkey;
	int Ykey;
	int COMMAND_X;
	int COMMAND_Y;
	int BATTLE_X;
	int BATTLE_Y;
	int YInit;
	int YPosition[4] = {200, 75, -85, -250};
	int padCount;
	int padMiniwindowCount;

	bool operation;
	bool SelectKeyInput;
	bool isSetting;
	bool atInit;
	bool InputPossible;
	bool OpponentSelection;
	// bool isWindowOpen;
};