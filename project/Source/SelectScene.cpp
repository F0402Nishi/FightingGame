#include "SelectScene.h"
#include <DxLib.h>
#include <fstream>
#include <assert.h>
#include "PlayScene.h"
#include "MiniWindow.h"

int SelectScene::gameType = 0;

SelectScene::SelectScene()
{
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 0.0f, -1000.0f), VGet(0.0f, 0.0f, 0));

	OperationImage = LoadGraph("data/2D/Ki-Bo-Do_4.png");
	assert(OperationImage >= 0);
	CommandImage = LoadGraph("data/2D/command.png");
	assert(CommandImage >= 0);
	BattleImage = LoadGraph("data/2D/battle.png");
	assert(BattleImage >= 0);
	SelectionArrowImage = LoadGraph("data/2D/selectionArrow.png");
	assert(SelectionArrowImage >= 0);
	SelectBackImage = LoadGraph("data/2D/Select_Back.png");
	assert(SelectBackImage >= 0);

	operation = false;
	SelectKeyInput = false;
	isSetting = false;
	atInit = true;
	InputPossible = true;
	OpponentSelection = false;
	isWindowOpen = false;
	
	COMMAND_X = -850;
	COMMAND_Y = -200;
	BATTLE_X = 400;
	BATTLE_Y = 200;

	Xkey = COMMAND_X;
	Ykey = COMMAND_Y;

	YInit = 0;
	YPosition[0] = 200;
	YPosition[1] = 75;
	YPosition[2] = -85;
	YPosition[3] = -250;


	memset(keyCounter, 0, sizeof(keyCounter));
	UpdateKey();

	new MiniWindow(this);
}

SelectScene::~SelectScene()
{
}

void SelectScene::Update()
{
	KeyMovement();

	if (CheckHitKey(KEY_INPUT_TAB) && !isSetting) {
		SceneManager::ChangeScene("TITLE");
	}
}

void SelectScene::Draw()
{
	SetBackgroundColor(0, 0, 0); //※背景の色変更に使用
	// DrawBox(0, 0, 640, 240, GetColor(0, 0, 255), TRUE); // 上半分を青で塗りつぶし
	// DrawBox(0, 240, 640, 480, GetColor(255, 0, 0), TRUE); // 下半分を赤で塗りつぶし
	
	DrawRotaGraph3D(0, 0, 0, 2.8f, 0, SelectBackImage, TRUE);
	DrawRotaGraph3D(-600, -200, 0, 1.5f, 0, CommandImage, TRUE);
	DrawRotaGraph3D(600, 200, 0, 1.5f, 0, BattleImage, TRUE);
	DrawRotaGraph3D(Xkey, Ykey, 0, 1.0f, 0, SelectionArrowImage, TRUE);

	DrawExtendString(30, 30, 2, 2, "[Tab] 戻る", GetColor(255, 255, 255));
	// DrawString(0, 0, "SELECT SCENE", GetColor(255, 255, 255));  //※Sceneの確認に使用
	// DrawString(520, 600, "Push [Tab]Key To Setting", GetColor(255, 255, 255));

	// if (operation) { DrawRotaGraph3D(0, 0, 0, 1.5f, 0, OperationImage, TRUE); isSetting = true; }
	if (OpponentSelection) {
		DrawExtendString(930, 300, 2, 2, "TRINING", GetColor(192, 192, 192));
		DrawExtendString(930, 400, 2, 2, "PLAYER vs CPU", GetColor(255, 255, 255));
		DrawExtendString(930, 500, 2, 2, "PLAYER vs PLAYER", GetColor(192, 192, 192));
	}
}

void SelectScene::KeyMovement()
{
	UpdateKey();

	if (atInit && keyCounter[KEY_INPUT_RETURN] == 1 && !SelectKeyInput) {
		operation = !operation;
		isWindowOpen = !isWindowOpen;
		isSetting = !isSetting;
		InputPossible = !InputPossible;
		SelectKeyInput = true;
	}
	//if (!atInit && keyCounter[KEY_INPUT_RETURN] == 1 && !SelectKeyInput) {
	//}
	if (!atInit && keyCounter[KEY_INPUT_RETURN] == 1 && !SelectKeyInput) {
		switch (YInit) {
		case 0:
			OpponentSelection = !OpponentSelection;
			InputPossible = !InputPossible;
			isSetting = !isSetting;
			break;
		case 1:
			gameType = YInit;
			SceneManager::ChangeScene("PLAY");
			break;
		case 2:
			gameType = YInit;
			SceneManager::ChangeScene("PLAY");
			break;
		case 3:
			gameType = YInit;
			// SceneManager::ChangeScene("PLAY");
			break;
		}
	}
	if (keyCounter[KEY_INPUT_RETURN] == 0 && atInit) { SelectKeyInput = false; }

	if (InputPossible) {
		if (keyCounter[KEY_INPUT_RIGHT] == 1 || keyCounter[KEY_INPUT_LEFT] == 1) {
			if (atInit) { // 初期位置にいたなら → 移動先へ
				Xkey = BATTLE_X;
				Ykey = BATTLE_Y;
				atInit = false;
			}
			else { // 移動先にいたなら → 初期位置へ
				Xkey = COMMAND_X;
				Ykey = COMMAND_Y;
				atInit = true;
			}
		}
	}

	if (OpponentSelection) {
		if (keyCounter[KEY_INPUT_DOWN] == 1) {
			YInit++; // 次の段階へ
			if (YInit > 3) { YInit = 0; } // 4回目で戻る
			Ykey = YPosition[YInit];
		}
		if (keyCounter[KEY_INPUT_UP] == 1) {
			YInit--; // 前の段階へ
			if (YInit < 0) { YInit = 3; } // 最初より前なら最後に戻る
			Ykey = YPosition[YInit];
		}
	}
}

void SelectScene::UpdateKey()
{
	char keys[256]; // 今のキー状態
	GetHitKeyStateAll(keys); // 今のキー状態を取得

	for (int i = 0; i < 256; i++) {
		if (keys[i] != 0) { keyCounter[i]++; }
		else { keyCounter[i] = 0; }
	}
}
