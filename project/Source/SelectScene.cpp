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

	// OperationImage = LoadGraph("data/2D/Ki-Bo-Do_4.png");
	// assert(OperationImage >= 0);
	CommandImage = LoadGraph("data/2D/command.png");
	assert(CommandImage >= 0);
	BattleImage = LoadGraph("data/2D/battle.png");
	assert(BattleImage >= 0);
	SelectionArrowImage = LoadGraph("data/2D/selectionArrow.png");
	assert(SelectionArrowImage >= 0);
	SelectBackImage = LoadGraph("data/2D/Title_Back01.png");
	assert(SelectBackImage >= 0);

	operation = false;
	SelectKeyInput = false;
	atInit = true;
	InputPossible = true;
	OpponentSelection = false;
	isSetting = false;
	changeScene = false;

	firstFrame = true;
	wasBPressed = false;
	wasBackPressed = false;
	wasRightPressed = false;
	wasLeftPressed = false;
	wasUpPressed = false;
	wasDownPressed = false;
	
	COMMAND_X = -850;
	COMMAND_Y = -200;
	BATTLE_X = 400;
	BATTLE_Y = 200;
	Gx = 0;
	Gy = 0;

	Xkey = COMMAND_X;
	Ykey = COMMAND_Y;

	YInit = 0;
	padCount = 0;

	memset(keyCounter, 0, sizeof(keyCounter));
	UpdateKey();

	miniwindow = new MiniWindow();
	fade = new Fade();
}

SelectScene::~SelectScene()
{
}

void SelectScene::Update()
{
	padCount = GetConnectedPadCount();

	KeyMovement();
	GetJoypadXInputState(DX_INPUT_PAD1, &inputScene);
	GetJoypadAnalogInput(&Gx, &Gy, DX_INPUT_PAD1);
	fade->Update();

	// === コントローラー用のKey一覧 ===
	bPressed = inputScene.Buttons[XINPUT_BUTTON_B];
	backPressed = inputScene.Buttons[XINPUT_BUTTON_BACK];
	nowRight = keyCounter[KEY_INPUT_RIGHT] == 1 || Gx > 200 || inputScene.Buttons[XINPUT_BUTTON_DPAD_RIGHT];
	nowLeft = keyCounter[KEY_INPUT_LEFT] == 1 || Gx < -200 || inputScene.Buttons[XINPUT_BUTTON_DPAD_LEFT];
	nowUp = keyCounter[KEY_INPUT_UP] == 1 || Gy < -200 || inputScene.Buttons[XINPUT_BUTTON_DPAD_UP]; // 上キー or スティック上
	nowDown = keyCounter[KEY_INPUT_DOWN] == 1 || Gy > 200 || inputScene.Buttons[XINPUT_BUTTON_DPAD_DOWN];

	bHit = bPressed && !wasBPressed;
	backHit = backPressed && !wasBackPressed;
	rightHit = nowRight && !wasRightPressed;
	leftHit = nowLeft && !wasLeftPressed;
	upHit = nowUp && !wasUpPressed;
	downHit = nowDown && !wasDownPressed;
	
	// シーン開始直後は押した瞬間判定を無効化
	if (firstFrame) {
		bHit = false;
		backHit = false;
		firstFrame = false;
	}


	if ((keyCounter[KEY_INPUT_TAB] == 1 || backHit) && !isSetting) {
		SceneManager::ChangeScene("TITLE");
	}

	wasBackPressed = backPressed;
}

void SelectScene::Draw()
{
	SetBackgroundColor(0, 0, 0); //※背景の色変更に使用
	
	DrawRotaGraph3D(0, 0, 0, 1.2f, 0, SelectBackImage, TRUE);
	//DrawRotaGraph3D(0, 0, 0, 2.8f, 0, SelectBackImage, TRUE);
	DrawRotaGraph3D(-600, -200, 0, 1.5f, 0, CommandImage, TRUE);
	DrawRotaGraph3D(600, 200, 0, 1.5f, 0, BattleImage, TRUE);
	DrawRotaGraph3D(Xkey, Ykey, 0, 1.0f, 0, SelectionArrowImage, TRUE);

	// === タイトルシーンに移動 ===
	DrawExtendString(30, 30, 2, 2, "[Tab] 戻る", GetColor(255, 255, 255));

	// === ゲームタイプの種類 ===
	if (OpponentSelection) {
		DrawExtendString(930, 300, 2, 2, "TRAINING", GetColor(255, 255, 255));
		DrawExtendString(930, 400, 2, 2, "PLAYER vs CPU", GetColor(255, 255, 255));
		DrawExtendString(930, 500, 2, 2, "PLAYER vs PLAYER", GetColor(192, 192, 192));
		if (padCount > 2) {
			DrawExtendString(930, 500, 2, 2, "PLAYER vs PLAYER", GetColor(255, 255, 255));
		}
	}

	fade->Draw();
	miniwindow->Draw();

	// === グラデーション付き赤ボックス（外側ほど濃い） ===
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 15); // 透明度50%
	//DrawBox(50, 530, 450, 430, GetColor(255, 0, 0), TRUE);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 30); // 透明度50%
	//DrawBox(50, 530, 100, 430, GetColor(255, 0, 0), TRUE);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//DrawBox(50, 530, 450, 430, GetColor(255, 255, 255), FALSE);

	// DrawBox(0, 0, 640, 240, GetColor(0, 0, 255), TRUE); // 上半分を青で塗りつぶし
	// DrawBox(0, 240, 640, 480, GetColor(255, 0, 0), TRUE); // 下半分を赤で塗りつぶし

	// DrawString(0, 0, "SELECT SCENE", GetColor(255, 255, 255));  //※Sceneの確認に使用
	// DrawString(520, 600, "Push [Tab]Key To Setting", GetColor(255, 255, 255));

	// if (operation) { DrawRotaGraph3D(0, 0, 0, 1.5f, 0, OperationImage, TRUE); isSetting = true; }
}

void SelectScene::KeyMovement()
{
	UpdateKey();

	// === コマンド表示用 ===
	if (atInit && (keyCounter[KEY_INPUT_RETURN] == 1 || bHit) && !SelectKeyInput) {
		operation = !operation;
		isSetting = !isSetting;
		InputPossible = !InputPossible;

		miniwindow->ToggleCommand();

		SelectKeyInput = true;
	}

	// === ゲームタイプ選択用 ===
	if (!atInit && (keyCounter[KEY_INPUT_RETURN] == 1 || bHit) && !SelectKeyInput) {
		switch (YInit) {
		case 0:
			OpponentSelection = !OpponentSelection;
			InputPossible = !InputPossible;
			isSetting = !isSetting;
			break;
		case 1:
			gameType = YInit;
			fade->FadeOut();
			changeScene = true;
			break;
		case 2:
			gameType = YInit;
			fade->FadeOut();
			changeScene = true;
			break;
		case 3:
			if (padCount >= 2) {
				gameType = YInit;
				fade->FadeOut();
				changeScene = true;
			}
			else {
				miniwindow->TogglePad(true);
			}
			break;
		}
	}
	if (keyCounter[KEY_INPUT_RETURN] == 0 && atInit) { SelectKeyInput = false; }

	// === フェードアウト完了したらシーン切り替え ===
	if (changeScene && fade->IsFadeOutEnd()) { SceneManager::ChangeScene("PLAY"); }

	// === 矢印移動(左右) ===
	if (InputPossible) {
		if (rightHit || leftHit) {
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

	// === 矢印移動(上下) ===
	if (OpponentSelection) {
		if (downHit) {
			YInit++; // 次の段階へ
			if (YInit > 3) { YInit = 0; } // 4回目で戻る
			Ykey = YPosition[YInit];
		}
		if (upHit) {
			YInit--; // 前の段階へ
			if (YInit < 0) { YInit = 3; } // 最初より前なら最後に戻る
			Ykey = YPosition[YInit];
		}
	}

	// === 前フレーム状態を更新 ===
	wasBPressed = bPressed;
	wasRightPressed = nowRight;
	wasLeftPressed = nowLeft;
	wasUpPressed = nowUp;
	wasDownPressed = nowDown;
}

int SelectScene::GetConnectedPadCount()
{
	int count = 0;
	XINPUT_STATE state;

	// 最大4台分チェック
	for (int i = 0; i < 4; i++) {
		if (GetJoypadXInputState(DX_INPUT_PAD1 + i, &state) == 0) {
			count++;
		}
	}

	return count;
}
