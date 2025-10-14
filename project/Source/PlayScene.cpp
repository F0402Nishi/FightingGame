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

PlayScene::PlayScene()
{
	// SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 0.0f, -1000.0f), VGet(0.0f, 0.0f, 0));
	opponentType = SelectScene::gameType;

	p1 = new Player(true);

	if (opponentType == 1) { // トレーニングモード
		p2 = new CPU(false);
	}
	else if (opponentType == 2) { // CPU戦モード
		p2 = new CPU(true);
	}
	else if (opponentType == 3) { // Player戦モード
		p2 = new Player(false);
	}

	new Stage();
	new Field();
	h1 = new UI2D();
	h2 = new UI2D();
	miniwindow = new MiniWindow();
	fade = new Fade();

	h1->Init(p1, opponentType);
	h2->Init(p2, opponentType);
	p1->SetOpponent(p2);
	p2->SetOpponent(p1);
	p1->SetHitSpheres();
	p2->SetHitSpheres();
	p1->SetAlive(false);
	p2->SetAlive(false);


	PlayNow = false;
	isMenu = false;
	isWind = false;
	openwind = false;
	changeScene = false;

	memset(keyCounter, 0, sizeof(keyCounter));
	UpdateKey();

	// === スタート時 READY 表示 ===
	if (opponentType == 1) { 
		h1->SetMessage(Battle::Training, 120);
		battlePhase = 2;
	}
	else {
		h1->SetMessage(Battle::Ready, 120); // 2秒
		h2->SetMessage(Battle::Ready, 120);
		battlePhase = 1;
	}
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	UpdateKey();
	UpdateCamera();
	UpdateBattleFont();
	MenuKey();
	fade->Update();

	// === デバッグ用：強制遷移 ===
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
	if (CheckHitKey(KEY_INPUT_R)) {
		SceneManager::ChangeScene("SELECT");
	}

	//ImGui::Begin("Menu");
	//ImGui::Checkbox("changeScene", &changeScene);
	//ImGui::Checkbox("isWind", &isWind);
	//ImGui::Text("alpha = %d", fade->alpha);
	//ImGui::InputInt("Type", &opponentType);
	//ImGui::End();
#if false
	if (CheckHitKey(KEY_INPUT_TAB) && PlayerKeyInput == false) {
		PlayerKeyInput = true;
	}
	if (!CheckHitKey(KEY_INPUT_TAB)) {
		PlayerKeyInput = false;
	}
#endif
}

void PlayScene::Draw()
{
	SetBackgroundColor(199, 199, 199); //※背景の色変更に使用
	
	miniwindow->Draw();
	fade->Draw();

#if false
	int screenW, screenH;
	GetDrawScreenSize(&screenW, &screenH); // 現在の画面の幅と高さを取得

	//DrawString(0, 0, "左上", GetColor(255, 255, 255));
	//DrawString(screenW - 100, 0, "右上", GetColor(255, 255, 255));
	//DrawString(0, screenH - 20, "左下", GetColor(255, 255, 255));
	//DrawString(screenW - 100, screenH - 20, "右下", GetColor(255, 255, 255));

	DrawRotaGraph3D(200.0f, 430.0f, 0, 0.38f, 0, HPImage, TRUE);
	
	DrawLine3D(VGet(0, 0, 0), VGet(1000, 0, 0), GetColor(255, 0, 0)); //※ステージのx座標を確認に使用
	DrawLine3D(VGet(0, 0, 0), VGet(0, 1000, 0), GetColor(0, 255, 0)); //※ステージのy座標を確認に使用
	DrawLine3D(VGet(0, 0, 0), VGet(0, 0, 1000), GetColor(0, 0, 255)); //※ステージのy座標を確認に使用

	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));  //※Sceneの確認に使用
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
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

#if 0
	ImGui::Begin("Camera");
	ImGui::InputFloat("zoomZ", &zoomZ);
	//ImGui::InputInt("Type", &opponentType);
	ImGui::End();
#endif // 0
}

void PlayScene::UpdateBattleFont()
{
	// === READY が終わったら FIGHT ===
	if (battlePhase == 1 && h1->IsBattleFinish()) {
		h1->SetMessage(Battle::Fight, 60); // 1秒
		h2->SetMessage(Battle::Fight, 60);
		PlayNow = true;
		battlePhase = 2;
	}

	// === FIGHT が終わったらバトル開始 ===
	if (battlePhase == 2 && h1->IsBattleFinish()) {
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

			if (opponentType == 2) { h1->SetMessage(Battle::Lose, 180); }
			else if (opponentType == 3) { h2->SetMessage(Battle::P2Win, 180); }
			battlePhase = 4;
		}
		else if (p2->GetHp() <= 0 && p1->GetHp() > 0) {
			// CPU死亡、プレイヤー生存
			p1->SetAlive(false);
			p2->SetAlive(false);

			if (opponentType == 2) { h1->SetMessage(Battle::Win, 180); }
			else if (opponentType == 3) { h1->SetMessage(Battle::P1Win, 180); }
			battlePhase = 4;
		}
		else if (p1->GetHp() <= 0 && p2->GetHp() <= 0) {
			// 両方 HP0 → 引き分け
			p1->SetAlive(false);
			p2->SetAlive(false);

			h1->SetMessage(Battle::Draw, 180);
			battlePhase = 4;
		}
	}

	// === 勝敗メッセージ終了後 ===
	if (battlePhase == 4 && h1->IsBattleFinish()) {
		// シーン遷移する or 次ラウンドへ
		SceneManager::ChangeScene("RESULT");
	}
}

void PlayScene::MenuKey()
{
	if (isWind && !miniwindow->IsCommandNow()) {
		if (keyCounter[KEY_INPUT_TAB] == 1 && !isMenu) {
			isMenu = !isMenu;
			openwind = !openwind;
			miniwindow->ToggleMenu();
		}
		if (keyCounter[KEY_INPUT_TAB] == 0 && isMenu) {
			isMenu = false;
		}
	}

	// === 矢印移動(上下) ===
	if (openwind) {
		if (!miniwindow->IsCommandNow()) {
			if (keyCounter[KEY_INPUT_DOWN] == 1) {
				miniwindow->MoveArrow(1); // 下移動
			}
			if (keyCounter[KEY_INPUT_UP] == 1) {
				miniwindow->MoveArrow(-1); // 上移動
			}
		}

		if (keyCounter[KEY_INPUT_RETURN] == 1) {
			int option = miniwindow->GetMenuOption();

			switch (option) {
			case 0: // コマンド表示
				miniwindow->ToggleCommand();
				break;
			case 1:
				fade->FadeOut();
				changeScene = true;
				SceneManager::ChangeScene("SELECT");
				break;
			case 2:
				fade->FadeOut();
				changeScene = true;
				SceneManager::ChangeScene("TITLE");
				break;
			}
		}
	}
}