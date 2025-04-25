#include "Player.h"
#include <assert.h>
#include "../ImGui/imgui.h"
#include "Stage.h"

#define PLAYER_SPEED 1.0f;

Player::Player()
{
	hModel = MV1LoadModel("data/Character/Armature/Armature.mv1");
	assert(hModel >= 0);

	transform.position = VGet(-5.0f, 100.0f, 0);
	transform.rotation = VGet(0, DegToRad(-90.0f), 0);
	transform.scale = VGet(2, 2, 2);

	anim = new Animator(hModel);

	state = S_STOP;
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

	// ’n–Ê‚É—§‚½‚¹‚é
	Stage* stage = FindGameObject<Stage>();
	VECTOR push;
	if (stage->SearchObject(transform.position + VGet(0, 1000, 0), transform.position + VGet(0, -10, 0), &push)) {
		transform.position += push;
	}
	else {
		velocityY = 0.0f;
		state = S_JUMP;
	}

#if false
	VECTOR hitPos; // “–‚½‚Á‚½‚çêŠ‚ð•Ô‚µ‚Ä‚à‚ç‚¤
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
	ImGui::Text("push.x: %.2f", push.x);
	ImGui::Text("push.y: %.2f", push.y);
	ImGui::End();
}

void Player::Draw()
{
	Object3D::Draw();
}

void Player::UpdateStop()
{
	VECTOR inputDir = VGet(0, 0, 0);

	anim->Play("data/Character/Player/Fight_Idle.mv1", true);

	if (CheckHitKey(KEY_INPUT_A)) {
		inputDir.x = -1.0f;
		anim->Play("data/Character/Player/Walk_F.mv1", false);
	}
	if (CheckHitKey(KEY_INPUT_D)) {
		inputDir.x = 1.0f;
		anim->Play("data/Character/Player/Walk_B.mv1", false);
	}
	if (CheckHitKey(KEY_INPUT_SPACE)) {
		velocityY = 10.0f;
		transform.position.y = velocityY;
		state = S_JUMP;
	}

	// ¶‰EˆÚ“®
	if (VSize(inputDir) > 0) {
		if (VSize(inputDir) >= 1.0f) {
			inputDir = VNorm(inputDir);
		}
		velocity = inputDir * PLAYER_SPEED;
		transform.position += velocity;
	}
}

void Player::UpdateAttack1()
{
}

void Player::UpdateAttack2()
{
}

void Player::UpdateAttack3()
{
}

void Player::UpdateJump()
{
	transform.position += velocity;
	transform.position.y = velocityY;

	static const float Gravity = 5.0f;
	velocityY -= Gravity;
}
