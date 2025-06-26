#include "PlayScene.h"
#include <DxLib.h>
#include "Player.h"
#include "Stage.h"
#include "Field.h"
#include "../ImGui/imgui.h"

int n = 0;
static float posx = 000.0f;
static float posy = 200.0f;
static float posz = -400.0f;

PlayScene::PlayScene()
{
	// SetCameraPositionAndTarget_UpVecY(VGet(posx, posy, -400.0f), VGet(posx, posy, 0));

	p1 = new Player(true);
	p2 = new Player(false);
	new Stage();
	new Field();

	p1->SetOpponent(p2);
	p2->SetOpponent(p1);
	p1->InitHitSpheres();
	p2->InitHitSpheres();
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	UpdateCamera();
	// VECTOR playerPos = p1->GetTransform().position;
	// SetCameraPositionAndTarget_UpVecY(VGet(playerPos.x, posy, -400.0f), VGet(playerPos.x, posy, 0));

	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
}

void PlayScene::Draw()
{
	// SetBackgroundColor(0, 0, 255); //※背景の色変更に使用
	
	DrawLine3D(VGet(0, 0, 0), VGet(1000, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(VGet(0, 0, 0), VGet(0, 1000, 0), GetColor(0, 255, 0));
	DrawLine3D(VGet(0, 0, 0), VGet(0, 0, 1000), GetColor(0, 0, 255));

	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
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

	ImGui::Begin("Camera");
	ImGui::InputFloat("zoomZ", &zoomZ);
	ImGui::End();

	SetCameraPositionAndTarget_UpVecY(VGet(posx, posy, zoomZ), VGet(posx, posy, 0));
}
