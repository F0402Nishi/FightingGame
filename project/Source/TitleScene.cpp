#include "TitleScene.h"
#include <DxLib.h>
#include <fstream>
#include <assert.h>

using namespace std;
float _posx;
float _posy;

TitleScene::TitleScene()
{
	SetCameraPositionAndTarget_UpVecY(VGet(_posx, _posy, -1000.0f), VGet(_posx, _posy, 0));

	TitleImage = LoadGraph("data/2D/TiteleLogo.png");
	assert(TitleImage >= 0);
	OperationImage = LoadGraph("data/2D/Ki-Bo-Do_4.png");
	assert(OperationImage >= 0);

	operation = false;
	TitleKeyInput = false;
	isSetting = false;

// ステージ試作
#if false
	ofstream f("data/test.txt"); //ファイルを開く
	// const char* str = "1Aa_";
	// f.write(str, 4);　//ファイルに書く
	// f.write(str, 4);

	const int val = 1234;
	f.write((char*)&val, 4);
	f.close(); //ファイルを閉じる

	ifstream f("data/test.txt");
	f.read((char*)&readVal, 4);
	f.close();


	ifstream f("data/stage00.csv");
	string str;
	getline(f, str);
	getline(f, str);
	f.close();
#endif
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	if (CheckHitKey(KEY_INPUT_RETURN) && !isSetting) {
		SceneManager::ChangeScene("PLAY");
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
	if (CheckHitKey(KEY_INPUT_TAB) && TitleKeyInput == false) {
		operation = !operation;
		isSetting = !isSetting;
		TitleKeyInput = true;
	}
	if (!CheckHitKey(KEY_INPUT_TAB)) {
		TitleKeyInput = false;
	}
}


void TitleScene::Draw()
{
	SetBackgroundColor(199, 199, 199); //※背景の色変更に使用

	DrawRotaGraph3D(0, 0, 0, 3, 0, TitleImage, TRUE);
	DrawString(520, 550, "Push [Enter]Key To Play", GetColor(64, 64, 64));
	DrawString(520, 600, "Push [Tab]Key To Setting", GetColor(64, 64, 64));

	if (operation == true) { DrawRotaGraph3D(0, 0, 0, 1.5f, 0, OperationImage, TRUE); isSetting = true; }

	// DrawString(0, 0, "TITLE SCENE", GetColor(64, 64, 64)); //※Sceneの確認に使用
	// DrawFormatString(100, 100, GetColor(255, 255, 255), "%d", readVal);
}
