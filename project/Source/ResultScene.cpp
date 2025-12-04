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
	miniwindow->SetLayout({ 415, 515, 615 });

	fade = new Fade();

	resultnumbers = 0;
	sceneNumber = 0;
	changeScene = false;
	windowClose = false;

	firstFrame = true;
	wasBPressed = false;
	wasUpPressed = false;
	wasDownPressed = false;

	memset(keyCounter, 0, sizeof(keyCounter));
	UpdateKey();

	SoundManager::Load("SelectSe", "data/sound/SE/SNES-Fighting06/SNES-Fighting06-13(Select).mp3");
	SoundManager::Load("DecideSe", "data/sound/SE/SNES-Fighting06/SNES-Fighting06-14(Select).mp3");
}

ResultScene::~ResultScene()
{
	SoundManager::DeleteAll();
}

void ResultScene::Update()
{
	UpdateKey();
	GetJoypadXInputState(DX_INPUT_PAD1, &inputScene);
	GetJoypadAnalogInput(&Gx, &Gy, DX_INPUT_PAD1);
	fade->Update();
	miniwindow->DrawResultBox(resultnumbers);

	bPressed = inputScene.Buttons[XINPUT_BUTTON_B];
	nowUp = keyCounter[KEY_INPUT_UP] == 1 || Gy < -200 || inputScene.Buttons[XINPUT_BUTTON_DPAD_UP]; // 上キー or スティック上
	nowDown = keyCounter[KEY_INPUT_DOWN] == 1 || Gy > 200 || inputScene.Buttons[XINPUT_BUTTON_DPAD_DOWN];

	bHit = bPressed && !wasBPressed;
	upHit = nowUp && !wasUpPressed;
	downHit = nowDown && !wasDownPressed;

	// シーン開始直後は押した瞬間判定を無効化
	if (firstFrame) {
		bHit = false;
		upHit = false;
		downHit = false;
		firstFrame = false;
	}

	if (miniwindow->IsResultNow()) {
		if (downHit) {
			SoundManager::Play("SelectSe", DX_PLAYTYPE_BACK);
			miniwindow->MoveBox(1); // 下移動
		}
		if (upHit) {
			SoundManager::Play("SelectSe", DX_PLAYTYPE_BACK);
			miniwindow->MoveBox(-1); // 上移動
		}
		if (keyCounter[KEY_INPUT_RETURN] == 1 || bHit) {
			int option = miniwindow->GetMenuOption();
			SoundManager::Play("DecideSe", DX_PLAYTYPE_BACK);

			switch (option) {
			case 0:
				sceneNumber = 1;
				windowClose = true;
				fade->FadeOut();
				changeScene = true;
				break;
			case 1:
				sceneNumber = 2;
				windowClose = true;
				fade->FadeOut();
				changeScene = true;
				break;
			case 2:
				sceneNumber = 3;
				windowClose = true;
				fade->FadeOut();
				changeScene = true;
				break;
			}
		}

		wasBPressed = bPressed;
		wasUpPressed = nowUp;
		wasDownPressed = nowDown;
	}

	if (windowClose) { miniwindow->ToggleReslut(false); }

	if (sceneNumber == 1 && changeScene && fade->IsFadeOutEnd()) { SceneManager::ChangeScene("PLAY"); }
	if (sceneNumber == 2 && changeScene && fade->IsFadeOutEnd()) { SceneManager::ChangeScene("SELECT"); }
	if (sceneNumber == 3 && changeScene && fade->IsFadeOutEnd()) { SceneManager::ChangeScene("TITLE"); }

	ImGui::Begin("Result");
	ImGui::InputFloat("winScale", &winScale);
	ImGui::InputFloat("loseScale", &loseScale);
	// ImGui::Checkbox("window", &windowClose);
	// ImGui::Checkbox("resultwindowOpen", &miniwindow->resultwindowOpen);
	// ImGui::Text("boxY = %d", miniwindow->boxY);
	// ImGui::Text("resultNumber = %d", (int)resultNumber);
	// ImGui::Text("resultnumbers = %d", (int)resultnumbers);
	ImGui::End();
}

void ResultScene::Draw()
{
	SetBackgroundColor(0, 0, 0); //※背景の色変更に使用

	DrawRotaGraph3D(0, 0, 0, 1.08f, 0, resultBackImage, TRUE);
	//DrawString(10, 10, "Result SCENE", GetColor(255, 255, 255));  //※Sceneの確認に使用

	UpdateResultFont();
	fade->Draw();
	miniwindow->Draw();
}

/// <summary>
/// 勝敗結果に応じてリザルト画面の文字と画像を描画する。
/// </summary>
void ResultScene::UpdateResultFont()
{
	const char* resultFontTextP = "";
	const char* resultFontTextC = "";
	float resultFontSize = 3.0f;
	int resultFontColor = GetColor(255, 255, 255);
	int winx = 0;
	int losex = 0;

	// --- スケール（拡大率） ---
	static bool winFinished = false;

	switch (resultNumber) {
	case Result::Win:
		resultFontTextP = "PLAYER";
		resultFontTextC = "CPU";
		winx = -600;
		losex = 600;
		resultnumbers = 1;
		break;
	case Result::Lose:
		resultFontTextC = "CPU";
		resultFontTextP = "PLAYER";
		winx = 600;
		losex = -600;
		resultnumbers = 2;
		break;
	case Result::Draw:
		resultFontTextP = "PLAYER";
		resultFontTextC = "CPU";
		resultFontColor = GetColor(0, 255, 0);
		resultnumbers = 3;
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

	// --- 拡大アニメーション ---
	if (winScale < 3.0f) {
		winScale += (3.0f - winScale) * 0.1f;
		if (winScale >= 3.0f) {
			winScale = 3.0f;
			winFinished = true;
		}
	}
	else if (winFinished && loseScale < 3.0f) {
		loseScale += 0.05f;
		if (loseScale > 3.0f) loseScale = 3.0f;
	}

	DrawExtendString(150, 100, resultFontSize, resultFontSize, resultFontTextP, resultFontColor);
	DrawExtendString(950, 100, resultFontSize, resultFontSize, resultFontTextC, resultFontColor);

	// --- 勝者から出てくる ---
	DrawRotaGraph3D(winx, 160, 0, winScale, 0, winImage, TRUE);

	// --- 負けた方は勝者が出てきてから出現 ---
	if (winFinished)
	{
		DrawRotaGraph3D(losex, 160, 0, loseScale, 0, loseImage, TRUE);
	}

	if (!windowClose && winScale >= 3.0f && loseScale >= 3.0f) { miniwindow->ToggleReslut(true); }
}
