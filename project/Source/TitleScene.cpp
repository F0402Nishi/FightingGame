#include "TitleScene.h"
#include <DxLib.h>
#include <fstream>
#include <assert.h>


TitleScene::TitleScene()
{
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 0.0f, -1100.0f), VGet(0.0f, 0.0f, 0));

	MechImage = LoadGraph("data/2D/mech.png");
	assert(MechImage >= 0);
	BrawlersImage = LoadGraph("data/2D/brawlers.png");
	assert(BrawlersImage >= 0);
	TitleBackImage = LoadGraph("data/2D/Title_Back00.png");
	assert(TitleBackImage >= 0);

	SoundManager::Load("MechSe", "data/sound/SE/Title/MECH.mp3");
	SoundManager::Load("BrawlersSe", "data/sound/SE/Title/BRAWLERS.mp3");
	SoundManager::Load("DecideSe", "data/sound/SE/SNES-Fighting06/SNES-Fighting06-14(Select).mp3");
	
	SoundManager::Load("TitleBgm", "data/sound/BGM/BGM_Title.mp3");
	SoundManager::ChangeVolume("TitleBgm", 80);
	SoundManager::Play("TitleBgm", DX_PLAYTYPE_BACK);

	memset(keyCounter, 0, sizeof(keyCounter));
	UpdateKey();

	Mechscale = 0;
	Brawlersscale = 0;
	scaleSpeed = 0.15f;
	statFontScale = 0;

	ScaleCount = false;
	playSe = false;
	playedSecond = false;
	scaleOk = false;
	decideSoundPlayed = false;
	
	padNow = false;

	// === 前フレームの状態を保持 ===
	wasBPressed = false;
	wasBackPressed = false;
}

TitleScene::~TitleScene()
{
	SoundManager::DeleteAll();
}

void TitleScene::Update()
{
	UpdateKey();
	GetJoypadXInputState(DX_INPUT_PAD1, &inputScene);
	padNow = (GetJoypadXInputState(DX_INPUT_PAD1, &inputScene) == 0);

	// --- 拡大率を更新 ---
	if (Mechscale < 3.0f) {
		Mechscale += scaleSpeed;
		if (Mechscale > 3.0f) { Mechscale = 3.0f; }
	}
	else {
		Brawlersscale += scaleSpeed;
		if (Brawlersscale > 3.0f) { Brawlersscale = 3.0f; scaleOk = true; }
	}

	if (scaleOk) {
		if (statFontScale < 2.0f) {
			statFontScale += 0.02f;
			if (statFontScale > 2.0f) { statFontScale = 2.0f; ScaleCount = true; }
		}
	}

	// === SEの確認用 ===
	static int mechTimer = 0;

	if (Mechscale < 1.0f && !playSe) {
		SoundManager::Play("MechSe", DX_PLAYTYPE_BACK);
		playSe = true;
		mechTimer = 0;
	}

	if (playSe && !playedSecond) {
		mechTimer++;
		if (mechTimer >= 24) {
			SoundManager::Play("BrawlersSe", DX_PLAYTYPE_BACK);
			playedSecond = true;
		}
	}

	if (SoundManager::IsPlaying("BrawlersSe") == 0 && playSe && playedSecond) {
		playSe = false;
		playedSecond = false;
	}

	// === 今フレームの状態 ===
	bPressed = inputScene.Buttons[XINPUT_BUTTON_B];
	backPressed = inputScene.Buttons[XINPUT_BUTTON_BACK];

	// === 押した瞬間だけ ===
	bHit = bPressed && !wasBPressed;
	backHit = backPressed && !wasBackPressed;

	// === シーン開始直後は押した瞬間判定を無効化 ===
	if (firstFrame) {
		bHit = false;
		backHit = false;
		firstFrame = false;
	}

	if (!decideSoundPlayed && ScaleCount && (keyCounter[KEY_INPUT_RETURN] == 1 || bHit)) {
		SoundManager::Play("DecideSe", DX_PLAYTYPE_BACK);
		decideSoundPlayed = true;
		decideTimer = 0;
	}

	if (decideSoundPlayed) {
		decideTimer++;
		if (decideTimer > 42) { SceneManager::ChangeScene("SELECT"); }
	}

	if (keyCounter[KEY_INPUT_ESCAPE] == 1 || backHit) {
		SceneManager::Exit();
	}

	// === 前フレーム状態を更新 ===
	wasBPressed = bPressed;
	wasBackPressed = backPressed;
}

void TitleScene::Draw()
{
	SetBackgroundColor(0, 0, 0); //※背景の色変更に使用

	DrawRotaGraph3D(0, 0, 0, 1.2f, 0, TitleBackImage, TRUE);
	DrawRotaGraph3D(220, 130, 0, Mechscale, 0, MechImage, TRUE);
	DrawRotaGraph3D(0, -100, 0, Brawlersscale, 0, BrawlersImage, TRUE);

	if (scaleOk) {
		if (padNow) {
			DrawExtendString(450, 550, 2, statFontScale, "Push [B] To Play", GetColor(255, 255, 255));
			DrawExtendString(30, 680, 1.2f, 1.2f, "[Back] 終了", GetColor(255, 255, 255));
		}
		else {
			DrawExtendString(450, 550, 2, statFontScale, "Push [Enter] Key To Play", GetColor(255, 255, 255));
			DrawExtendString(30, 680, 1.2f, 1.2f, "[Esc] 終了", GetColor(255, 255, 255));
		}
	}

	// DrawExtendString(Screen::ToScreenX(450), Screen::ToScreenY(550), 2, 2, "Push [Enter] Key To Play", GetColor(255, 255, 255));
	
	// DrawString(0, 0, "TITLE SCENE", GetColor(64, 64, 64)); //※Sceneの確認に使用
}
