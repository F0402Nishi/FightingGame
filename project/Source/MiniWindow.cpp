#include "MiniWindow.h"
#include <assert.h>
#include "SelectScene.h"
#include "../ImGui/imgui.h"

// === コマンドリスト ===
const char* commandList[] = {
	"移動：右", 
	"移動：左", 
	"弱パンチ", 
	"中パンチ", 
	"強パンチ",
	"弱キック", 
	"中キック", 
	"強キック", 
	"ガード",
	"トレーニング専用",
	"オプション",
	"モード切り替え",
};

// === コマンドキーリスト ===
const char* KeyList[] = {
	"D", 
	"A", 
	"U", 
	"I", 
	"O",
	"J", 
	"K", 
	"L", 
	"H",
	"",
	"Tab",
	"Q",
};

// === ゲームパッドキーリスト ===
const char* PadList[] = {
	"→", 
	"←", 
	"B", 
	"RB", 
	"RT",
	"A", 
	"LB", 
	"LT", 
	"X",
	"",
	"メニューボタン",
	"Y",
};

MiniWindow::MiniWindow()
{
	// selectScene = scene;

	optionBack = LoadGraph("data/2D/BackGround.png");
	assert(optionBack >= 0);
	optionArrow = LoadGraph("data/2D/optionArrow01.png");
	assert(optionArrow >= 0);


	scrollOffset = 0;
	itemHeight = 100;
	windowHeight = 650 - 100; // 文字開始 y = 100 なので調整
	arrowX = 450;
	boxY = 200;

	commandCount = sizeof(commandList) / sizeof(commandList[0]);
	KeyCount = sizeof(KeyList) / sizeof(KeyList[0]);
	PadCount = sizeof(PadList) / sizeof(PadList[0]);

	commandwindowOpen = false;
	menuwindowOpen = false;
	resultwindowOpen = false;
	padwindowOpen = false;
	windowUpKeyInput = false;
	windowDownKeyInput = false;
	listLast = false;
	listStat = false;

	firstOptionFrame = true;
	commandUpPressed = false;
	commandDownPressed = false;

	Mx = 0;
	My = 0;
}

MiniWindow::~MiniWindow()
{
}

void MiniWindow::Update()
{
	GetJoypadXInputState(DX_INPUT_PAD1, &inputCommand);
	GetJoypadAnalogInputRight(&Mx, &My, DX_INPUT_PAD1);
	
	int DZ = 200; // デッドゾーン
	stickUp = My < -DZ;
	stickDown = My > DZ;

	nowCUp = inputCommand.Buttons[XINPUT_BUTTON_DPAD_UP];
	nowCDown = inputCommand.Buttons[XINPUT_BUTTON_DPAD_DOWN];

	upCHit = nowCUp && !commandUpPressed;
	downCHit = nowCDown && !commandDownPressed;

	if (firstOptionFrame) {
		upCHit = false;
		downCHit = false;
		firstOptionFrame = false;
	}

	if (commandwindowOpen) { 
		if ((CheckHitKey(KEY_INPUT_UP) == 1 || stickUp || upCHit) && !windowUpKeyInput && !listStat) { // 上キー → 上スクロール
			scrollOffset += 100;
			windowUpKeyInput = true;
			listLast = false;
		}
		if (CheckHitKey(KEY_INPUT_UP) == 0) { windowUpKeyInput = false; }
	
		if ((CheckHitKey(KEY_INPUT_DOWN) == 1 || stickDown || downCHit) && !windowDownKeyInput && !listLast) { // 下キー → 下スクロール
			scrollOffset -= 100;
			windowDownKeyInput = true;
			listStat = false;
		}
		if (CheckHitKey(KEY_INPUT_DOWN) == 0) { windowDownKeyInput = false; }

		commandUpPressed = nowCUp;
		commandDownPressed = nowCDown;
	}

	//ImGui::Begin("Mini");
	//ImGui::Checkbox("stickUp", &stickUp);
	//ImGui::Checkbox("stickDown", &stickDown);
	//ImGui::InputInt("My", &My);
	//ImGui::End();

	// === スクロール範囲の制限 ===
	//int minOffset = -(commandCount * itemHeight - windowHeight); // 一番下まで
	//if (scrollOffset < minOffset) scrollOffset = minOffset;
	//if (scrollOffset > 0) scrollOffset = 0; // 一番上まで
}

void MiniWindow::Draw()
{
	if (padwindowOpen) {
		int rx = 400, ry = 370, rw = 500, rh = 300;
		DrawBox(rx, ry, rx + rw, ry + rh, GetColor(50, 50, 50), TRUE); // 背景（濃い灰色）
		DrawBox(rx, ry, rx + rw, ry + rh, GetColor(255, 255, 255), FALSE); // 枠線（白）
	}

	if (menuwindowOpen) {
		int mx = 0, my = 0.5, mw = 1280, mh = 720;
		DrawBox(mx, my, mx + mw, my + mh, GetColor(50, 50, 50), TRUE); // 背景（濃い灰色）
		DrawBox(mx, my, mx + mw, my + mh, GetColor(255, 255, 255), FALSE); // 枠線（白）
		DrawGraph(mx, my, optionBack, TRUE);
		// DrawRotaGraph(arrowX, arrowY, 1.0f, 0, optionArrow, TRUE);

		int boxHeight = 120;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 96);
		DrawBox(300, boxY - boxHeight / 2, 1000, boxY + boxHeight / 2, GetColor(0, 80, 255), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		
		DrawExtendString(10, 10, 2, 2, "TRAINING OPTION", GetColor(255, 255, 255));
		DrawExtendString(500, 150, 5, 5, "コマンド", GetColor(255, 255, 255));
		DrawExtendString(500, 350, 5, 5, "セレクト", GetColor(255, 255, 255));
		DrawExtendString(500, 550, 5, 5, "タイトル", GetColor(255, 255, 255));
	}

	if (resultwindowOpen) {
		int rx = 400, ry = 370, rw = 500, rh = 300;
		DrawBox(rx, ry, rx + rw, ry + rh, GetColor(192, 192, 192), TRUE); // 背景（濃い灰色）
		DrawBox(rx, ry, rx + rw, ry + rh, GetColor(255, 255, 255), FALSE); // 枠線（白）

		int boxHeight = 70;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 126);
		DrawBox(420, boxY - boxHeight / 2, 880, boxY + boxHeight / 2, GetColor(r, g, b), TRUE);
		// DrawBox(420, 380, 880, 450, GetColor(0, 80, 255), TRUE); // 再戦
		// DrawBox(420, 480, 880, 550, GetColor(0, 80, 255), TRUE); // セレクト
		// DrawBox(420, 580, 880, 650, GetColor(0, 80, 255), TRUE); // タイトル
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		// DrawBox(420, 380, 880, 450, GetColor(255, 255, 255), FALSE);

		DrawExtendString(590, 390, 3, 3, "再戦", GetColor(255, 255, 255));
		DrawExtendString(550, 490, 3, 3, "セレクト", GetColor(255, 255, 255));
		DrawExtendString(550, 590, 3, 3, "タイトル", GetColor(255, 255, 255));
	}

	if (commandwindowOpen) { CommandWindow();}
}

void MiniWindow::CommandWindow()
{
	int x = 50, y = 50, w = 1200, h = 650;
	const int baseY = 100; // 最初の行のY
	const int StatIndex = 0; // 最初の項目のインデックス
	const int LastIndex = 12; // 最後の項目のインデックス
	int rightY = baseY + StatIndex * itemHeight + scrollOffset;
	int guardY = baseY + LastIndex * itemHeight + scrollOffset;
		
	DrawBox(x, y, x + w, y + h, GetColor(50, 50, 50), TRUE); // 背景（濃い灰色）
	DrawBox(x, y, x + w, y + h, GetColor(255, 255, 255), FALSE); // 枠線（白）

	// ---- ここからクリッピング開始 ----
	SetDrawArea(x, y, x + w, y + h);

	for (int i = 0; i < commandCount; i++) {
		int drawX = 200;
		int drawY = 100 + i * 100 + scrollOffset; // 基準Y + スクロール
		
		if (i == 9) { drawX = 70; }

		DrawExtendString(drawX, drawY, 2, 2, commandList[i], GetColor(255, 255, 255));
	}

	for (int k = 0; k < KeyCount; k++) {
		int keY = 100 + k * 100 + scrollOffset;
		DrawExtendString(600, keY, 2, 2, KeyList[k], GetColor(255, 255, 255));
	}

	for (int p = 0; p < PadCount; p++) {
		int paY = 100 + p * 100 + scrollOffset;
		DrawExtendString(900, paY, 2, 2, PadList[p], GetColor(255, 255, 255));
	}

	if (rightY >= y && rightY <= y + h) { listStat = true; }
	if (guardY >= y && guardY <= y + h) { listLast = true; }

	// ---- クリッピング解除（画面全体に戻す）----
	SetDrawArea(0, 0, 1920, 1080);
}

void MiniWindow::SetLayout(const std::vector<int>& _postionY)
{
	Yposition = _postionY;

	// === 初期位置を最初のメニューに設定 ===
	if (!Yposition.empty()) {
		boxY = Yposition[0];
	}
}

void MiniWindow::MoveBox(int _box)
{
	// === 配列が空なら動かせない ===
	if (Yposition.empty()) return;

	// === コマンド表示中なら動かさない！ ===
	if (commandwindowOpen) return; 

	// const int step = 200;
	// const int MenuFont[3] = { 200,400,600 };

	// === 現在の index を求める ===
	int index = 0;
	for (int i = 0; i < (int)Yposition.size(); i++) {
		if (boxY == Yposition[i]) {
			index = i;
			break;
		}
	}

	// === dir に応じてインデックスを移動 ===
	index += _box;

	// === 範囲を循環させる ===
	if (index < 0) index = (int)Yposition.size() - 1;
	if (index >= (int)Yposition.size()) index = 0;

	// === 新しい位置に設定 ===
	boxY = Yposition[index];
}

int MiniWindow::GetMenuOption() const
{
	for (int i = 0; i < (int)Yposition.size(); i++)
	{
		if (boxY == Yposition[i])  // 完全一致の場合
			return i;
	}

	return -1; // 見つからなかった場合
}

void MiniWindow::DrawResultBox(int _rnumbers)
{
	switch (_rnumbers) {
	case 1:
		r = 190; g = 0; b = 63; // 赤系
		break;
	case 2:
		r = 0; g = 100; b = 255; // 青系
		break;
	case 3:
		r = 0; g = 255; b = 0;    // 緑
		break;
	}
}
