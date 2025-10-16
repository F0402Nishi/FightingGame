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

	resultNumber = PlayScene::lastResult;

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

	const char* resultFontText = "";
	float resultFontSize = 15.0f;
	int resultFontColor = GetColor(255, 255, 255);

	switch (resultNumber) {
	case Result::Win:
		resultFontText = "PLAY WIN";
		resultFontColor = GetColor(190, 0, 63);
		DrawExtendString(100, 200, resultFontSize, resultFontSize, resultFontText, resultFontColor);
		break;
	case Result::Lose:
		resultFontText = "CPU WIN";
		resultFontColor = GetColor(0, 0, 190);
		DrawExtendString(100, 200, resultFontSize, resultFontSize, resultFontText, resultFontColor);
		break;
	case Result::Draw:
		resultFontText = "DRAW";
		resultFontColor = GetColor(0, 255, 0);
		DrawExtendString(100, 200, resultFontSize, resultFontSize, resultFontText, resultFontColor);
		break;
	case Result::P1Win:
		resultFontText = "PLAY1 WIN";
		resultFontColor = GetColor(190, 0, 63);
		DrawExtendString(100, 200, resultFontSize, resultFontSize, resultFontText, resultFontColor);
		break;
	case Result::P2Win:
		resultFontText = "PLAY2 WIN";
		resultFontColor = GetColor(190, 0, 63);
		DrawExtendString(100, 200, resultFontSize, resultFontSize, resultFontText, resultFontColor);
		break;
	}
}
