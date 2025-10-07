#include "TitleScene.h"
#include <DxLib.h>
#include <fstream>
#include <assert.h>

TitleScene::TitleScene()
{
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 0.0f, -1000.0f), VGet(0.0f, 0.0f, 0));

	MechImage = LoadGraph("data/2D/mech.png");
	assert(MechImage >= 0);
	BrawlersImage = LoadGraph("data/2D/brawlers.png");
	assert(BrawlersImage >= 0);
	PoseLeftImage = LoadGraph("data/2D/fightingPoseLeft.png");
	assert(PoseLeftImage >= 0);
	PoseRightImage = LoadGraph("data/2D/fightingPoseRight.png");
	assert(PoseRightImage >= 0);
	TitleBackImage = LoadGraph("data/2D/Title_Back.png");
	assert(TitleBackImage >= 0);


#if false "ステージ試作"
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
	if (CheckHitKey(KEY_INPUT_RETURN)) {
		SceneManager::ChangeScene("SELECT");
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}


void TitleScene::Draw()
{
	// SetBackgroundColor(199, 199, 199); //※背景の色変更に使用

	// DrawRotaGraph3D(0, 0, 0, 1.1f, 0, TitleBackImage, TRUE);
	DrawRotaGraph3D(0, 0, 0, 3.2f, 0, TitleBackImage, TRUE);
	DrawRotaGraph3D(220, 130, 0, 3, 0, MechImage, TRUE);
	DrawRotaGraph3D(0, -100, 0, 3, 0, BrawlersImage, TRUE);
	DrawRotaGraph3D(-600, 90, 0, 0.7f, 0, PoseLeftImage, TRUE);
	DrawRotaGraph3D(-390, 90, 0, 0.7f, 0, PoseRightImage, TRUE);
	
	DrawExtendString(450, 550, 2, 2, "Push [Enter]Key To Play", GetColor(255, 255, 255));
	// DrawString(520, 600, "Push [Tab]Key To Setting", GetColor(64, 64, 64));

	// DrawString(0, 0, "TITLE SCENE", GetColor(64, 64, 64)); //※Sceneの確認に使用
	// DrawFormatString(100, 100, GetColor(255, 255, 255), "%d", readVal);
}
