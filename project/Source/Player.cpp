#include "Player.h"
#include <assert.h>
#include "Field.h"
#include "Goblin.h"
#include "Camera.h"
#include "stage.h"
#include "../ImGui/imgui.h"

Player::Player()
{
	hModel = MV1LoadModel("data/Character/BaseDisplay.mv1");
	assert(hModel >= 0);
	hWeapon = MV1LoadModel("data/Character/Weapon/Sabel/Sabel.mv1");
	assert(hWeapon >= 0);

	anim = new Animator(hModel);

	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));
	// armRot = 0.0f;

	state = S_STOP;

	collder = new SphereCollder(VGet(0, 50, 0), 50);
}
 
Player::~Player()
{
	delete anim;
}

void Player::Update()
{
	anim->Update();

#if 0
	if (CheckHitKey(KEY_INPUT_R)) {
		anim->Play("data/Character/Player/Anim_Run.mv1");
	}

	if (CheckHitKey(KEY_INPUT_Y)) {
		anim->Play("data/Character/Player/Anim_Neutral.mv1");
	}
#endif

	switch (state) {
	case S_STOP:
		UpdateStop();
		break;
	case S_ATTACK1:
		UpdateAttack1();
		break;
	case S_ATTACK2:
		UpdateAttack2();
		break;
	case S_ATTACK3:
		UpdateAttack3();
		break;
	}


	// 地面に立たせる
	Stage* stage = FindGameObject<Stage>();
	VECTOR hitPos; // 当たったら場所を返してもらう
	if (stage->SearchObject(transform.position + VGet(0, 1000, 0), transform.position + VGet(0, -1000, 0), &hitPos)) {
		transform.position = hitPos;
	}

	//敵との当たり判定をする
	auto goblins = FindGameObjects<Goblin>();
	for (Goblin* goblin : goblins) {
		VECTOR gPos = goblin -> GetTransform().position;
		VECTOR vec = gPos - transform.position;
		vec.y = 0;
		if (VSize(vec) < 100.0f) {
			float pushVal = 100.0f - VSize(vec);
			transform.position -= VNorm(vec) * pushVal;
		}
	}

	//状態遷移図

#if 0
	/** 骨をいじる実験 */
	armRot += DegToRad(3.0f);
	int armID = MV1SearchFrame(hModel, "armL");
	MATRIX mrotA = MGetRotY(armRot);
	MATRIX mtransA = MGetTranslate(VGet(13.05, 0, 0));
	MV1SetFrameUserLocalMatrix(hModel, armID, mrotA * mtransA);
	int elbowID = MV1SearchFrame(hModel, "elbowL");
	MATRIX mrot = MGetRotY(-DX_PI_F / 2);
	MATRIX mtrans = MGetTranslate(VGet(28.5 * 2, 0, 0));
	MV1SetFrameUserLocalMatrix(hModel, elbowID, mrot * mtrans);


	/** アニメーションの実験 **/
	int max = MV1GetAnimTotalTime(hAnimRun, 0);
	animTime += 0.5f;
	if (animTime >= max) {
		animTime -= max;
	}
	MV1SetAttachAnimTime(hModel, attachID, animTime);
#endif
}

void Player::Draw()
{
	Object3D::Draw();

	int wp = MV1SearchFrame(hModel, "wp");
	MATRIX m = MV1GetFrameLocalWorldMatrix(hModel, wp);
	MV1SetMatrix(hWeapon, m);
	MV1DrawModel(hWeapon);

	DrawLine3D(VGet(0, 0, 0) * m, VGet(0, -100, 0) * m, GetColor(255, 0, 0));
}

void Player::UpdateStop()
{
	Camera* c = FindGameObject<Camera>();
	VECTOR inputDir = VGet(0, 0, 0);

	if (CheckHitKey(KEY_INPUT_W)) {
		// transform.position.z += 2.0f * cosf (transfrom.rotation.y);
		inputDir.z = 1.0f;
	}

	if (CheckHitKey(KEY_INPUT_D)) {
		inputDir.x = 1.0f;
	}

	if (CheckHitKey(KEY_INPUT_A)) {
		inputDir.x = -1.0f;
	}

	if (CheckHitKey(KEY_INPUT_S)) {
		inputDir.z = -1.0f;
	}

	int inputX, inputY;
	GetJoypadAnalogInput(&inputX, &inputY, DX_INPUT_KEY_PAD1);

	inputDir += VGet(inputX / 1000.0f, 0, -inputY / 1000.0f); //inputX, inputYを、inputDirに反映する

	if (VSize(inputDir) > 0) {
		if (VSize(inputDir) >= 1.0f) {
			inputDir = VNorm(inputDir); // 長さを1にする
		}
		VECTOR velocity = inputDir * MGetRotY(c->GetTransform().rotation.y) * 10.0f; // 移動ベクトル = 入力ベクトル×カメラの回転×速度
		transform.position += velocity; // 座標 += 移動ベクトル
		transform.rotation.y = atan2f(velocity.x, velocity.z); // キャラの向きは、移動ベクトルから求める
		anim->Play("data/Character/Player/Anim_Run.mv1", true);
	}
	else {
		anim->Play("data/Character/Player/Anim_Neutral.mv1", true);
	}

	int keys = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	ImGui::Begin("PAD");
	ImGui::InputInt("KEY", &keys);
	ImGui::End();

	if (CheckHitKey(KEY_INPUT_M) || (keys & PAD_INPUT_3) !=0) {
		anim->Play("data/Character/Player/Anim_Attack1.mv1", false);
		state = S_ATTACK1;
	}
}

void Player::UpdateAttack1()
{
	if (anim -> IsFinish()) {
		state = S_STOP;
	}

	if (anim->CurrentAnimTime() > 8.5f) {
		if (CheckHitKey(KEY_INPUT_M)) {
			anim->Play("data/Character/Player/Anim_Attack2.mv1", false);
			state = S_ATTACK2;
		}
	}

	AttackEnemy();
}

void Player::UpdateAttack2()
{
	if (anim->CurrentAnimTime() > 8.5f) {
		if (CheckHitKey(KEY_INPUT_M)) {
			anim->Play("data/Character/Player/Anim_Attack3.mv1", false);
			state = S_ATTACK3;
		}
	}

	AttackEnemy();
}

void Player::UpdateAttack3()
{
	if (anim->IsFinish()) {
		state = S_STOP;
	}

	AttackEnemy();
}

void Player::AttackEnemy()
{
	int wp = MV1SearchFrame(hModel, "wp");
	MATRIX m = MV1GetFrameLocalWorldMatrix(hModel, wp);
	MV1SetMatrix(hWeapon, m);
	MV1DrawModel(hWeapon);

	auto goblins = FindGameObjects<Goblin>();
	for (Goblin* goblin : goblins) {
		goblin -> CheckLine(VGet(0, 0, 0) * m, VGet(0, -100, 0) * m);
	}
}
