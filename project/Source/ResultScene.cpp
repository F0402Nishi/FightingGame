#include "ResultScene.h"
#include <DxLib.h>
#include <fstream>
#include <assert.h>

ResultScene::ResultScene()
{
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 0.0f, -1000.0f), VGet(0.0f, 0.0f, 0));

	resultBackImage = LoadGraph("data/2D/Result_Back00.png");
	assert(resultBackImage >= 0);

	memset(keyCounter, 0, sizeof(keyCounter));
	UpdateKey();
}

ResultScene::~ResultScene()
{
}

void ResultScene::Update()
{
	UpdateKey();

	if (keyCounter[KEY_INPUT_TAB] == 1) {
		SceneManager::ChangeScene("TITLE");
	}
}

void ResultScene::Draw()
{
	DrawString(10, 10, "Result SCENE", GetColor(255, 255, 255));  //※Sceneの確認に使用
	DrawRotaGraph3D(0, 0, 0, 1.08f, 0, resultBackImage, TRUE);
}
