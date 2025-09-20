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

MiniWindow::MiniWindow(SelectScene* scene)
{
	selectScene = scene;

	scrollOffset = 0;
	itemHeight = 100;
	windowHeight = 650 - 100; // 文字開始 y = 100 なので調整

	commandCount = sizeof(commandList) / sizeof(commandList[0]);
	KeyCount = sizeof(KeyList) / sizeof(KeyList[0]);

	windowOpen = false;
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
	windowOpen = selectScene->GetMiniWindow();

	if (!windowOpen) { return; }

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

	// === スクロール範囲の制限 ===
	//int minOffset = -(commandCount * itemHeight - windowHeight); // 一番下まで
	//if (scrollOffset < minOffset) scrollOffset = minOffset;
	//if (scrollOffset > 0) scrollOffset = 0; // 一番上まで
}

void MiniWindow::Draw()
{
	if (windowOpen) {
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
}