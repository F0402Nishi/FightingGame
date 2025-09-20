#pragma once
#include <DxLib.h>
#include "Object3D.h"

class SelectScene;

class MiniWindow : public Object3D
{
public:
	MiniWindow(SelectScene* scene);
	~MiniWindow();
	void Update() override;
	void Draw() override;

private:
	SelectScene* selectScene;

	int scrollOffset;
	int commandCount; // コマンドリストの項目数
	int itemHeight; // 1項目の高さ
	int windowHeight; // 枠の高さから余白を引く
	int KeyCount; // コマンドキーリストの項目数

	bool windowOpen;
	bool windowUpKeyInput;
	bool windowDownKeyInput;
	bool listStat;
	bool listLast;
};
