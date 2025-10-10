#include "MiniWindow.h"
#include <assert.h>
#include "SelectScene.h"

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
	"ガード"
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
	"H"
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
	arrowY = 200;

	commandCount = sizeof(commandList) / sizeof(commandList[0]);
	KeyCount = sizeof(KeyList) / sizeof(KeyList[0]);

	commandwindowOpen = false;
	menuwindowOpen = false;
	resultwindowOpen = false;
	windowUpKeyInput = false;
	windowDownKeyInput = false;
	listLast = false;
	listStat = false;
}

MiniWindow::~MiniWindow()
{
}

void MiniWindow::Update()
{
	// windowOpen = selectScene->GetMiniWindow();

	if (commandwindowOpen) { 
		if (CheckHitKey(KEY_INPUT_UP) == 1 && !windowUpKeyInput && !listStat) { // 上キー → 上スクロール
			scrollOffset += 100;
			windowUpKeyInput = true;
			listLast = false;
		}
		if (CheckHitKey(KEY_INPUT_UP) == 0) { windowUpKeyInput = false; }
	
		if (CheckHitKey(KEY_INPUT_DOWN) == 1 && !windowDownKeyInput && !listLast) { // 下キー → 下スクロール
			scrollOffset -= 100;
			windowDownKeyInput = true;
			listStat = false;
		}
		if (CheckHitKey(KEY_INPUT_DOWN) == 0) { windowDownKeyInput = false; }
	}

	// === スクロール範囲の制限 ===
	//int minOffset = -(commandCount * itemHeight - windowHeight); // 一番下まで
	//if (scrollOffset < minOffset) scrollOffset = minOffset;
	//if (scrollOffset > 0) scrollOffset = 0; // 一番上まで
}

void MiniWindow::Draw()
{
	if (commandwindowOpen) { CommandWindow();}

	if (menuwindowOpen) {
		int mx = 0, my = 0.5, mw = 1280, mh = 720;
		DrawBox(mx, my, mx + mw, my + mh, GetColor(50, 50, 50), TRUE); // 背景（濃い灰色）
		DrawBox(mx, my, mx + mw, my + mh, GetColor(255, 255, 255), FALSE); // 枠線（白）
		DrawGraph(mx, my, optionBack, TRUE);
		DrawRotaGraph(arrowX, arrowY, 1.0f, 0, optionArrow, TRUE);
		DrawExtendString(10, 10, 2, 2, "TRAINING OPTION", GetColor(255, 255, 255));
		DrawExtendString(500, 150, 5, 5, "コマンド", GetColor(255, 255, 255));
		DrawExtendString(500, 350, 5, 5, "セレクト", GetColor(255, 255, 255));
		DrawExtendString(500, 550, 5, 5, "タイトル", GetColor(255, 255, 255));
	}

	if (resultwindowOpen) {
		int rx = 500, ry = 200, rw = 300, rh = 300;
		DrawBox(rx, ry, rx + rw, ry + rh, GetColor(50, 50, 50), TRUE); // 背景（濃い灰色）
		DrawBox(rx, ry, rx + rw, ry + rh, GetColor(255, 255, 255), FALSE); // 枠線（白）
	}
}

void MiniWindow::CommandWindow()
{
	int x = 50, y = 50, w = 1200, h = 650;
	const int baseY = 100; // 最初の行のY
	const int StatIndex = 0;
	const int LastIndex = 9;
	int rightY = baseY + StatIndex * itemHeight + scrollOffset;
	int guardY = baseY + LastIndex * itemHeight + scrollOffset;
		
	DrawBox(x, y, x + w, y + h, GetColor(50, 50, 50), TRUE); // 背景（濃い灰色）
	DrawBox(x, y, x + w, y + h, GetColor(255, 255, 255), FALSE); // 枠線（白）

	// ---- ここからクリッピング開始 ----
	SetDrawArea(x, y, x + w, y + h);

	for (int i = 0; i < commandCount; i++) {
		int drawY = 100 + i * 100 + scrollOffset; // 基準Y + スクロール
		DrawExtendString(200, drawY, 2, 2, commandList[i], GetColor(255, 255, 255));
	}

	for (int k = 0; k < KeyCount; k++) {
		int keY = 100 + k * 100 + scrollOffset;
		DrawExtendString(800, keY, 2, 2, KeyList[k], GetColor(255, 255, 255));
	}

	if (rightY >= y && rightY <= y + h) { listStat = true; }
	if (guardY >= y && guardY <= y + h) { listLast = true; }

	// ---- クリッピング解除（画面全体に戻す）----
	SetDrawArea(0, 0, 1920, 1080);
}

void MiniWindow::MoveArrow(int _arrow)
{
	if (commandwindowOpen) return; // コマンド表示中なら動かさない！

	const int step = 200;
	const int MenuFont[3] = { 200,400,600 };

	// === 現在の index を求める ===
	int menufontIndex = 0;
	for (int i = 0; i < 3; i++) {
		if (arrowY == MenuFont[i]) {
			menufontIndex = i;
			break;
		}
	}

	// === dir に応じてインデックスを移動 ===
	menufontIndex += _arrow;

	// === 範囲を循環させる ===
	if (menufontIndex < 0) menufontIndex = 2;
	if (menufontIndex > 2) menufontIndex = 0;

	// === 新しい位置に設定 ===
	arrowY = MenuFont[menufontIndex];
}

int MiniWindow::GetMenuOption() const
{
	switch (arrowY) {
	case 200:
		return 0;
	case 400:
		return 1;
	case 600:
		return 2;
	}
}
