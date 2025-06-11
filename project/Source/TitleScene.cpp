#include "TitleScene.h"
#include <DxLib.h>
#include <fstream>
#include <assert.h>

using namespace std;

TitleScene::TitleScene()
{
	TitleImage = LoadGraph("data/2D/TiteleLogo.png");
	assert(TitleImage >= 0);

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
	if (CheckHitKey(KEY_INPUT_P)) {
		SceneManager::ChangeScene("PLAY");
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}


void TitleScene::Draw()
{
	DrawRotaGraph3D(0, 0, 0, 1, 0, TitleImage, TRUE);
	DrawString(0, 0, "TITLE SCENE", GetColor(255,255,255));
	DrawString(100, 400, "Push [P]Key To Play", GetColor(255, 255, 255));

	// DrawFormatString(100, 100, GetColor(255, 255, 255), "%d", readVal);
}
