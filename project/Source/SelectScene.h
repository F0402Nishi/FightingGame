#pragma once
#include "../Library/SceneBase.h"

/// <summary>
/// タイトルシーン
/// 
/// タイトルを表示して、キーを押したらプレイシーンに移行する。
/// </summary>
class SelectScene : public SceneBase
{
public:
	SelectScene();
	~SelectScene();
	void Update() override;
	void Draw() override;
	void KeyMovement();

private:
	int OperationImage;
	int CommandImage;
	int BattleImage;
	int SelectionArrowImage;
	int SelectBackImage;
	int Xkey;
	int Ykey;

	bool operation;
	bool TitleKeyInput;
	bool isSetting;
	bool atInit;
};