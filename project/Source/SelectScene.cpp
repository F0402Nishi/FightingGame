#include "SelectScene.h"
#include <DxLib.h>
#include <fstream>
#include <assert.h>

SelectScene::SelectScene()
{
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
	TitleKeyInput = false;
	isSetting = false;
	atInit = true;
	Xkey = 0;
	Ykey = 0;
}

SelectScene::~SelectScene()
{
}

void SelectScene::Update()
{
	if (CheckHitKey(KEY_INPUT_E) && !isSetting) {
		SceneManager::ChangeScene("PLAY");
	}
	if (CheckHitKey(KEY_INPUT_TAB) && !isSetting) {
		SceneManager::ChangeScene("TITLE");
	}
	if (CheckHitKey(KEY_INPUT_Q) && TitleKeyInput == false) {
		operation = !operation;
		isSetting = !isSetting;
		TitleKeyInput = true;
	}
	if (!CheckHitKey(KEY_INPUT_Q)) {
		TitleKeyInput = false;
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
	DrawRotaGraph3D(0, 0, 0, 1.5f, 0, SelectionArrowImage, TRUE);

	DrawString(0, 0, "SELECT SCENE", GetColor(255, 255, 255));  //※Sceneの確認に使用
	DrawString(520, 600, "Push [Tab]Key To Setting", GetColor(255, 255, 255));

	if (operation == true) { DrawRotaGraph3D(0, 0, 0, 1.5f, 0, OperationImage, TRUE); isSetting = true; }
}

void SelectScene::KeyMovement()
{
	const int INTI_X = 0;
	const int INTI_Y = 0;

	const int TARGET_X = 0;
	const int TARGET_Y = 0;

	Xkey = INTI_X;
	Ykey = INTI_Y;

	char keys[256] = { 0 }; // 今のキー状態
	char oldkeys[256] = { 0 }; // 1フレーム前のキー状態

	while ( ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		memcpy(oldkeys, keys, 256);
		GetHitKeyStateAll(keys);
	}
}
