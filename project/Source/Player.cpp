#include "Player.h"
#include <assert.h>
#include "../ImGui/imgui.h"
#include "Stage.h"

#define PLAYER_SPEED 1.0f;
#define PLAYER_JUMP 25.0f;

Player::Player()
{
	hModel = MV1LoadModel("data/Character/Armature/Armature.mv1");
	assert(hModel >= 0);

	transform.position = VGet(-5.0f, 100.0f, 0);
	transform.rotation = VGet(0, DegToRad(-90.0f), 0);
	transform.scale = VGet(2, 2, 2);

	anim = new Animator(hModel);

	state = S_STOP;

	S_collder = new SphereCollder(VGet(10, 300, 0), 30);
	E_collder = new EllipseCollder(VGet(-10, 250, 0), VGet(-10, 150, 0), 30);

#if false	
	// アニメーションの制御実験
	int mA = MV1SearchFrame(hModel, "Hips");
	int mB = MV1SearchFrame(hModel, "UpperChest");
	int mc = MV1SearchFrame(hModel, "Neck");
	int md = MV1SearchFrame(hModel, "Head");
	MV1SetFrameUserLocalMatrix(hModel, mA, MGetIdent());
	MV1SetFrameUserLocalMatrix(hModel, mB, MGetIdent());
	MV1SetFrameUserLocalMatrix(hModel, mc, MGetIdent());
	MV1SetFrameUserLocalMatrix(hModel, md, MGetIdent());
#endif

}

Player::~Player()
{
	delete anim;
}

void Player::Update()
{
	anim->Update();

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
	case S_JUMP:
		UpdateJump();
		break;
	}

	// 地面に立たせる
	Stage* stage = FindGameObject<Stage>();
	VECTOR hit;
	if (stage->SearchObject(transform.position + VGet(0, 1000, 0), transform.position + VGet(0, -10, 0), &hit)) {
		transform.position = hit;
		if (state == S_JUMP) {
			state = S_STOP;
		}
	}
	else {
		//velocityY = 0.0f;
		static const float Gravity = 1.0f;
		velocityY -= Gravity;
		// transform.position.y += velocityY;
	}

#if false
	VECTOR hitPos; // 当たったら場所を返してもらう
	if (stage->SearchObject(transform.position + VGet(0, 1000, 0), transform.position + VGet(0, -1000, 0), &hitPos)) {
		transform.position = hitPos;
		if (transform.position.y <= hitPos.y) {
			transform.position.y = hitPos.y;
			isJumping = false;
			velocityY = 0;
		}
	}
#endif

	ImGui::Begin("PLAYER");
	ImGui::InputFloat("position.x", &transform.position.x);
	ImGui::InputFloat("position.y", &transform.position.y);
	ImGui::Text("push.x: %.2f", hit.x);
	ImGui::Text("push.y: %.2f", hit.y);
	ImGui::Text("state: %d", (int)state);
	ImGui::End();
}

void Player::Draw()
{
	Object3D::Draw();
}

void Player::UpdateStop()
{
	VECTOR inputDir = VGet(0, 0, 0);

	if (CheckHitKey(KEY_INPUT_A)) {
		inputDir.x = -1.0f;
		anim->Play("data/Character/Player/Walk_B.mv1", true);
	}
	if (CheckHitKey(KEY_INPUT_D)) {
		inputDir.x = 1.0f;
		anim->Play("data/Character/Player/Walk_F.mv1", true);
	}
	if (CheckHitKey(KEY_INPUT_SPACE)) {
		velocityY = PLAYER_JUMP;
		transform.position.y += velocityY;
		state = S_JUMP;
	}

	if (VSize(inputDir) == 0) {
		anim->Play("data/Character/Player/Fight_Idle.mv1", true);
	}

	// 左右移動
	if (VSize(inputDir) > 0) {
		if (VSize(inputDir) >= 1.0f) {
			inputDir = VNorm(inputDir);
		}
		velocity = inputDir * PLAYER_SPEED;
		transform.position += velocity;
	}

	if (CheckHitKey(KEY_INPUT_I)) {
		anim->Play("data/Character/Player/Atk_P_1.mv1", false);
		state = S_ATTACK1;
	}
	if (CheckHitKey(KEY_INPUT_S) && CheckHitKey(KEY_INPUT_I)) {
		anim->Play("data/Character/Player/Atk_K_1.mv1", false);
		state = S_ATTACK1;
	}
	if (CheckHitKey(KEY_INPUT_H)) {
		anim->Play("data/Character/Player/Guard_Idle.mv1", false);
		state = S_ATTACK1;
	}
}

void Player::UpdateAttack1()
{
	if (anim->IsFinish()) {
		state = S_STOP;
	}

	if (anim->CurrentAnimTime() > 6.0f) {
		if (CheckHitKey(KEY_INPUT_U)) {
			anim->Play("data/Character/Player/Atk_P_2.mv1", false);
			state = S_ATTACK2;
		}
		if (CheckHitKey(KEY_INPUT_S) && CheckHitKey(KEY_INPUT_U)) {
			anim->Play("data/Character/Player/Atk_K_2.mv1", false);
			state = S_ATTACK2;
		}
	}
}

void Player::UpdateAttack2()
{
	if (anim->IsFinish()) {
		state = S_STOP;
	}

	if (anim->CurrentAnimTime() > 6.0f) {
		if (CheckHitKey(KEY_INPUT_P)) {
			anim->Play("data/Character/Player/Atk_P_3.mv1", false);
			state = S_ATTACK3;
		}
		if (CheckHitKey(KEY_INPUT_S) && CheckHitKey(KEY_INPUT_P)) {
			anim->Play("data/Character/Player/Atk_K_3.mv1", false);
			state = S_ATTACK3;
		}
	}
}

void Player::UpdateAttack3()
{
	if (anim->IsFinish()) {
		state = S_STOP;
	}
}

void Player::UpdateJump()
{
//	transform.position += velocity;
	transform.position.y += velocityY;
}
