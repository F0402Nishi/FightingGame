#include "Player.h"
#include <assert.h>
#include "../ImGui/imgui.h"
#include "Stage.h"

#define PLAYER_SPEED 10.0f;

Player::Player()
{
	hModel = MV1LoadModel("data/Character/Armature/Armature.mv1");
	assert(hModel >= 0);

	transform.position = VGet(-500.0f, 100.0f, 0);
	transform.rotation = VGet(0, DegToRad(-90.0f), 0);
	transform.scale = VGet(2, 2, 2);

	anim = new Animator(hModel);
}

Player::~Player()
{
	delete anim;
}

void Player::Update()
{
	VECTOR inputDir = VGet(0, 0, 0);
	anim->Update();
	anim->Play("data/Character/Player/Fight_Idle.mv1", true);

	if (!isJumping && CheckHitKey(KEY_INPUT_SPACE)) {
		inputDir.y = 2.0f;
		velocityY = jumpPower;
		isJumping = true;
	}
	if (CheckHitKey(KEY_INPUT_A)) {
		inputDir.x = -1.0f;
		anim->Play("data/Character/Player/Walk_F.mv1", false);
	}
	if (CheckHitKey(KEY_INPUT_D)) {
		inputDir.x = 1.0f;
		anim->Play("data/Character/Player/Walk_B.mv1", false);
	}

	// ç∂âEà⁄ìÆ
	if (VSize(inputDir) > 0) {
		if (VSize(inputDir) >= 1.0f) {
			inputDir = VNorm(inputDir);
		}
		VECTOR velocity = inputDir * PLAYER_SPEED;
		transform.position += velocity;
	}

	// èdóÕêßå‰
	velocityY += gravity;
	transform.position.y = velocityY;

	// ínñ Ç…óßÇΩÇπÇÈ
	Stage* stage = FindGameObject<Stage>();
	VECTOR hitPos; // ìñÇΩÇ¡ÇΩÇÁèÍèäÇï‘ÇµÇƒÇ‡ÇÁÇ§
	if (stage->SearchObject(transform.position + VGet(0, 1000, 0), transform.position + VGet(0, -1000, 0), &hitPos)) {
		transform.position = hitPos;
		if (transform.position.y <= hitPos.y) {
			transform.position.y = hitPos.y;
			isJumping = false;
			velocityY = 0;
		}
	}

	ImGui::Begin("PLAYER");
	ImGui::InputFloat("position.x", &transform.position.x);
	ImGui::InputFloat("position.y", &transform.position.y);
	ImGui::End();
}

void Player::Draw()
{
	Object3D::Draw();
}
