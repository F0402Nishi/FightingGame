#include "PlayScene.h"
#include <DxLib.h>
#include "Player.h"
#include "Stage.h"
#include "Field.h"
#include "HPber.h"
#include "../ImGui/imgui.h"

static int n = 0;
static float posx = 000.0f;
static float posy = 200.0f;
static float posz = -400.0f;

PlayScene::PlayScene()
{
	p1 = new Player(true);
	p2 = new Player(false);
	new Stage();
	new Field();
	new HPber();

	p1->SetOpponent(p2);
	p2->SetOpponent(p1);
	p1->SetHitSpheres();
	p2->SetHitSpheres();

	PlayerKeyInput = false;
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	UpdateCamera();

	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
	if (CheckHitKey(KEY_INPUT_TAB) && PlayerKeyInput == false) {
		PlayerKeyInput = true;
	}
	if (!CheckHitKey(KEY_INPUT_TAB)) {
		PlayerKeyInput = false;
	}
}

void PlayScene::Draw()
{
	SetBackgroundColor(199, 199, 199); //※背景の色変更に使用

	// DrawRotaGraph3D(200.0f, 430.0f, 0, 0.38f, 0, HPImage, TRUE);
	
	// DrawLine3D(VGet(0, 0, 0), VGet(1000, 0, 0), GetColor(255, 0, 0)); //※ステージのx座標を確認に使用
	// DrawLine3D(VGet(0, 0, 0), VGet(0, 1000, 0), GetColor(0, 255, 0)); //※ステージのy座標を確認に使用
	// DrawLine3D(VGet(0, 0, 0), VGet(0, 0, 1000), GetColor(0, 0, 255)); //※ステージのy座標を確認に使用

	// DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));  //※Sceneの確認に使用
	// DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
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
	ImGui::End();
#endif // 0
}
