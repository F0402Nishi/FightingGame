#include "MiniWindow.h"
#include <assert.h>
#include "SelectScene.h"

MiniWindow::MiniWindow(SelectScene* scene)
{
	selectScene = scene;

	scrollOffset = 0;

	windowOpen = false;
}

MiniWindow::~MiniWindow()
{
}

void MiniWindow::Update()
{
	windowOpen = selectScene->GetMiniWindow();

	if (windowOpen) {
		if (CheckHitKey(KEY_INPUT_DOWN) == 1) {
			scrollOffset -= 30;
		}
		if (CheckHitKey(KEY_INPUT_UP) == 1) {
			scrollOffset += 30;
		}

		if (scrollOffset > 0) scrollOffset = 0;
		if (scrollOffset < -(900 - 600)) scrollOffset = -(900 - 600);
	}
}

void MiniWindow::Draw()
{
	if (windowOpen) {
		int x = 50, y = 50, w = 1200, h = 650;
		
		DrawBox(x, y, x + w, y + h, GetColor(50, 50, 50), TRUE); // 背景（濃い灰色）
		DrawBox(x, y, x + w, y + h, GetColor(255, 255, 255), FALSE); // 枠線（白）

		DrawExtendString(200, 100, 2, 2, "移動：右", GetColor(255, 255, 255));
		DrawExtendString(200, 200, 2, 2, "移動：左", GetColor(255, 255, 255));
		DrawExtendString(200, 300, 2, 2, "弱パンチ", GetColor(255, 255, 255));
		DrawExtendString(200, 400, 2, 2, "中パンチ", GetColor(255, 255, 255));
		DrawExtendString(200, 500, 2, 2, "強パンチ", GetColor(255, 255, 255));
		DrawExtendString(200, 600, 2, 2, "弱キック", GetColor(255, 255, 255));
		DrawExtendString(200, 700, 2, 2, "中キック", GetColor(255, 255, 255));
		DrawExtendString(200, 800, 2, 2, "強キック", GetColor(255, 255, 255));
		DrawExtendString(200, 900, 2, 2, "ガード", GetColor(255, 255, 255));
	}
}