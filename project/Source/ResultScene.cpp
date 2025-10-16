#include "ResultScene.h"
#include <DxLib.h>
#include <fstream>
#include <assert.h>
#include "../ImGui/imgui.h"

ResultScene::ResultScene()
{
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 0.0f, -1000.0f), VGet(0.0f, 0.0f, 0));

	resultBackImage = LoadGraph("data/2D/Result_Back00.png");
	assert(resultBackImage >= 0);
	winImage = LoadGraph("data/2D/WIN.png");
	assert(winImage >= 0);
	loseImage = LoadGraph("data/2D/LOSE.png");
	assert(loseImage >= 0);

	resultNumber = PlayScene::lastResult;

	miniwindow = new MiniWindow();

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

	ImGui::Begin("Result");
	ImGui::Text("resultNumber = %d", (int)resultNumber);
	ImGui::End();
}

void ResultScene::Draw()
{
	DrawRotaGraph3D(0, 0, 0, 1.08f, 0, resultBackImage, TRUE);
	DrawString(10, 10, "Result SCENE", GetColor(255, 255, 255));  //※Sceneの確認に使用

	UpdateResultFont();
}

void ResultScene::UpdateResultFont()
{
	const char* resultFontTextP = "";
	const char* resultFontTextC = "";
	float resultFontSize = 3.0f;
	int resultFontColor = GetColor(255, 255, 255);
	int winx = 0;
	int losex = 0;

	switch (resultNumber) {
	case Result::Win:
		resultFontTextP = "PLAYER";
		resultFontTextC = "CPU";
		winx = -600;
		losex = 600;
		break;
	case Result::Lose:
		resultFontTextC = "CPU";
		resultFontTextP = "PLAYER";
		winx = 600;
		losex = -600;
		break;
	case Result::Draw:
		resultFontTextP = "PLAYER";
		resultFontTextC = "CPU";
		resultFontColor = GetColor(0, 255, 0);
		break;
	case Result::P1Win:
		resultFontTextP = "PLAYER1";
		resultFontTextC = "PLAYER2";
		winx = -600;
		losex = 600;
		break;
	case Result::P2Win:
		resultFontTextP = "PLAYER2";
		resultFontTextC = "PLAYER1";
		winx = 600;
		losex = -600;
		break;
	}

	DrawExtendString(150, 100, resultFontSize, resultFontSize, resultFontTextP, resultFontColor);
	DrawExtendString(950, 100, resultFontSize, resultFontSize, resultFontTextC, resultFontColor);

	DrawRotaGraph3D(winx, 160, 0, 3.0f, 0, winImage, TRUE);
	DrawRotaGraph3D(losex, 160, 0, 3.0f, 0, loseImage, TRUE);

	miniwindow->ToggleReslut(true);
}
