#include "PlayScene.h"
#include <DxLib.h>
#include "Player.h"
#include "Stage.h"
#include "Field.h"

int n = 0;
float posx = 000.0f;
float posy = 200.0f;

PlayScene::PlayScene()
{
	SetCameraPositionAndTarget_UpVecY(VGet(posx, posy, -800.0f), VGet(posx, posy, 0));

	new Player(true);
	new Player(false);
	new Stage();
	new Field();
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
}

void PlayScene::Draw()
{
	SetBackgroundColor(0, 0, 255); //※背景の色変更に使用
	DrawLine3D(VGet(0, 0, 0), VGet(1000, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(VGet(0, 0, 0), VGet(0, 1000, 0), GetColor(0, 255, 0));
	DrawLine3D(VGet(0, 0, 0), VGet(0, 0, 1000), GetColor(0, 0, 255));

	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
