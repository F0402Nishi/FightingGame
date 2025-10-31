#include "PlayScene.h"
#include <DxLib.h>
#include "../ImGui/imgui.h"
#include "SelectScene.h"
#include "ResultScene.h"
#include "Character.h"
#include "Player.h"
#include "Cpu.h"
#include "Stage.h"
#include "Field.h"
#include "2DUI.h"

static float posx = 000.0f;
static float posy = 200.0f;
static float posz = -400.0f;

PlayScene* PlayScene::instance = nullptr;
Result PlayScene::lastResult = Result::None;

PlayScene::PlayScene()
{
	// SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 0.0f, -1000.0f), VGet(0.0f, 0.0f, 0));
	opponentType = SelectScene::gameType;
	instance = this;

	p1 = new Player(true);

	if (opponentType == 1) { // トレーニングモード
		p2 = new CPU(false);
		p1->SetInputDisplay(true);
	}
	else if (opponentType == 2) { // CPU戦モード
		p2 = new CPU(true);
		p1->SetInputDisplay(false);
	}
	else if (opponentType == 3) { // Player戦モード
		p2 = new Player(false);
		p1->SetInputDisplay(false);
	}


	new Stage();
	new Field();
	h1 = new UI2D();
	h2 = new UI2D();
	ui2d = new UI2D();
	miniwindow = new MiniWindow();
	fade = new Fade();

	h1->Init(p1, opponentType, UIType::HP);
	h2->Init(p2, opponentType, UIType::HP);
	ui2d->Init(nullptr, opponentType, UIType::B_Font);
	h1->AttackInformation();
	h2->AttackInformation();
	p1->SetOpponent(p2);
	p2->SetOpponent(p1);
	p1->SetHitSpheres();
	p2->SetHitSpheres();
	p1->SetAlive(false);
	p2->SetAlive(false);

	// === BOXの中央値 = (上端 + 下端) / 2 ===
	miniwindow->SetLayout({ 190, 390, 590 }); 

	resultType = Result::None;

	PlayNow = false;
	isMenu = false;
	isWind = false;
	openwind = false;
	changeScene = false;

	wasStartPressed = false;
	wasUpPressed = false;
	wasDownPressed = false;
	firstFrame = true;

	Gx = 0;
	Gy = 0;

	memset(keyCounter, 0, sizeof(keyCounter));
	UpdateKey();

	// === スタート時 READY 表示 ===
	if (opponentType == 1) { 
		ui2d->SetMessage(Battle::Training, 120);
		battlePhase = 2;
	}
	else {
		ui2d->SetMessage(Battle::Ready, 120); // 2秒
		// h2->SetMessage(Battle::Ready, 120);
		battlePhase = 1;
	}

}

PlayScene::~PlayScene()
{
	if (instance == this) instance = nullptr;
}

void PlayScene::Update()
{
	UpdateKey();
	UpdateCamera();
	UpdateBattleFont();
	MenuKey();
	GetJoypadXInputState(DX_INPUT_PAD1, &inputScene);
	GetJoypadAnalogInput(&Gx, &Gy, DX_INPUT_PAD1);

	startPressed = inputScene.Buttons[XINPUT_BUTTON_START];
	bPressed = inputScene.Buttons[XINPUT_BUTTON_B];
	nowUp = keyCounter[KEY_INPUT_UP] == 1 || Gy < -200 || inputScene.Buttons[XINPUT_BUTTON_DPAD_UP]; // 上キー or スティック上
	nowDown = keyCounter[KEY_INPUT_DOWN] == 1 || Gy > 200 || inputScene.Buttons[XINPUT_BUTTON_DPAD_DOWN];

	startHit = startPressed && !wasStartPressed;
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

	// === デバッグ用：強制遷移 ===
	//if (CheckHitKey(KEY_INPUT_T)) {
	//	SceneManager::ChangeScene("TITLE");
	//}
	//if (CheckHitKey(KEY_INPUT_R)) {
	//	SceneManager::ChangeScene("SELECT");
	//}
	
	//ImGui::Begin("Menu");
	//ImGui::Text("arrowY = %d", miniwindow->arrowY);
	//ImGui::InputInt("Type", &opponentType);
	//ImGui::Text("alpha = %d", fade->alpha);
	//ImGui::Text("IsBattleFinish = %d", h2->IsBattleFinish());
	//ImGui::Checkbox("changeScene", &changeScene);
	//ImGui::Checkbox("isWind", &isWind);
	//ImGui::Checkbox("stickUp", &miniwindow->stickUp);
	//ImGui::Checkbox("stickDown", &miniwindow->stickDown);
	//ImGui::End();
}

void PlayScene::Draw()
{
	SetBackgroundColor(199, 199, 199); //※背景の色変更に使用
	
	miniwindow->Draw();

#if false
	DrawLine3D(VGet(0, 0, 0), VGet(1000, 0, 0), GetColor(255, 0, 0)); //※ステージのx座標を確認に使用
	DrawLine3D(VGet(0, 0, 0), VGet(0, 1000, 0), GetColor(0, 255, 0)); //※ステージのy座標を確認に使用
	DrawLine3D(VGet(0, 0, 0), VGet(0, 0, 1000), GetColor(0, 0, 255)); //※ステージのy座標を確認に使用

	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));  //※Sceneの確認に使用
#endif // false
}

void PlayScene::UpdateCamera()
{
	float x1 = p1->GetTransform().position.x;
	float x2 = p2->GetTransform().position.x;

	float distX = fabsf(x1 - x2); // 距離(絶対値)

	// カメラの注視位置(真ん中)
	float targetX = (x1 + x2) * 0.5f;
	posx += (targetX - posx) * 0.1f;

	float zoomZ = posz - (distX - 200.0f) * 0.5f; // ※距離200を基準に奥に引く

	// ズーム範囲の制限（引きすぎ＆寄りすぎを防ぐ）
	if (zoomZ > -400.0f) zoomZ = -400.0f;     // 近づきすぎない
	if (zoomZ < -1200.0f) zoomZ = -1200.0f;   // 引きすぎない

	SetCameraPositionAndTarget_UpVecY(VGet(posx, posy, zoomZ), VGet(posx, posy, 0));

	//ImGui::Begin("Camera");
	//ImGui::InputFloat("zoomZ", &zoomZ);
	//ImGui::InputInt("Type", &opponentType);
	//ImGui::End();
}

void PlayScene::UpdateBattleFont()
{
	// === READY が終わったら FIGHT ===
	if (battlePhase == 1 && ui2d->IsBattleFinish()) {
		ui2d->SetMessage(Battle::Fight, 60); // 1秒
		// h2->SetMessage(Battle::Fight, 60);
		PlayNow = true;
		battlePhase = 2;
	}

	// === FIGHT が終わったらバトル開始 ===
	if (battlePhase == 2 && ui2d->IsBattleFinish()) {
		p1->SetAlive(true); // 入力解放
		p2->SetAlive(true);
		if (opponentType == 1) { isWind = true; }
		else { isWind = false; }
		if (PlayNow) { battlePhase = 3; }  // バトル中
	}

	// === 両方のHPを確認 ===
	if (battlePhase == 3) {
		if (p1->GetHp() <= 0 && p2->GetHp() > 0) {
			// プレイヤー死亡、CPU生存
			p1->SetAlive(false);
			p2->SetAlive(false);

			if (opponentType == 2) { resultType = Result::Lose; }
			else if (opponentType == 3) { resultType = Result::P2Win; }
			PlayScene::lastResult = resultType;
			ui2d->SetMessage(Battle::KO, 60);
			battlePhase = 4;
		}
		else if (p2->GetHp() <= 0 && p1->GetHp() > 0) {
			// CPU死亡、プレイヤー生存
			p1->SetAlive(false);
			p2->SetAlive(false);

			if (opponentType == 2) { resultType = Result::Win; }
			else if (opponentType == 3) { resultType = Result::P1Win; }
			PlayScene::lastResult = resultType;
			ui2d->SetMessage(Battle::KO, 60);
			battlePhase = 4;
		}
		else if (p1->GetHp() <= 0 && p2->GetHp() <= 0) {
			// 両方 HP0 → 引き分け
			p1->SetAlive(false);
			p2->SetAlive(false);

			resultType = Result::Draw;
			PlayScene::lastResult = resultType;
			ui2d->SetMessage(Battle::KO, 60);
			battlePhase = 4;
		}
	}

	// === 勝敗メッセージ終了後 ===
	if (battlePhase == 4 && ui2d->IsBattleFinish()) {
		// シーン遷移する or 次ラウンドへ
		SceneManager::ChangeScene("RESULT");
	}
}

void PlayScene::MenuKey()
{
	if (isWind && !miniwindow->IsCommandNow()) {
		if ((keyCounter[KEY_INPUT_TAB] == 1 || startHit) && !isMenu) {
			isMenu = !isMenu;
			openwind = !openwind;
			miniwindow->ToggleMenu();
		}
		if (keyCounter[KEY_INPUT_TAB] == 0 && isMenu) {
			isMenu = false;
		}

		wasStartPressed = startPressed;
	}

	// === 矢印移動(上下) ===
	if (openwind) {
		p1->SetInputDisplay(false);
		if (!miniwindow->IsCommandNow()) {
			if (downHit) {
				miniwindow->MoveBox(1); // 下移動
			}
			if (upHit) {
				miniwindow->MoveBox(-1); // 上移動
			}
		}

		if (keyCounter[KEY_INPUT_RETURN] == 1 || bHit) {
			int option = miniwindow->GetMenuOption();

			switch (option) {
			case 0: // コマンド表示
				miniwindow->ToggleCommand();
				break;
			case 1:
				fade->ExclusiveFadeOut();
				SceneManager::ChangeScene("SELECT");
				break;
			case 2:
				fade->ExclusiveFadeOut();
				SceneManager::ChangeScene("TITLE");
				break;
			}
		}

		wasBPressed = bPressed;
		wasUpPressed = nowUp;
		wasDownPressed = nowDown;
	}
	else if (!openwind && opponentType == 1){ 
		p1->SetInputDisplay(true);
	}
	else
	{
		p1->SetInputDisplay(false);
	}
}