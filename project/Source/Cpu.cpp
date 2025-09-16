#include "Cpu.h"
#include "Player.h"
#include "../ImGui/imgui.h"

CPU::CPU(bool _iscpu)
{
	player = FindGameObject<Player>();
	
	transform.position = VGet(200.0f, 14.0f, 150.0f);
	transform.rotation = VGet(0, DegToRad(90.0f), 0);

	isCpu = _iscpu;
	dx = 0.0f;
	targetDistance = 10.0f;
	expandThreshold = 200.0f;

	reachedTarget = false;
}

CPU::~CPU()
{
}

void CPU::Update()
{	
	std::cout << "CPU::Update called" << std::endl;

	Character::Always();

	hitSpheres[4].localOffset = left_HandWorldPos - basePos + VGet(-8.0f, 3.5f, -10.0f);
	hitSpheres[7].localOffset = right_HandWorldPos - basePos + VGet(-5.0f, 7.0f, 0.0f);

	// if (!opponent) return;
	if (!isCpu) anim->Play("data/Character/Player/Fight_Idle.mv1", true);
	if (!isCpu) return;

	EffectiveRange();

	ImGui::Begin("CPU-pos");
	ImGui::InputFloat("dx", &dx);
	ImGui::InputFloat("playerMoveDir", &playerMoveDir);
	ImGui::Text("MyPosition：%d", (int) & mypos);
	//ImGui::InputInt("Type", &opponentType);
	ImGui::End();
}

void CPU::Draw()
{
	std::cout << "CPU Draw" << std::endl;

	Character::Draw();
}

void CPU::EffectiveRange()
{
	// === 座標取得 ===
	mypos = transform.position;
	playerpos = player->GetTransform().position;

	dx = playerpos.x - mypos.x;
	float dist = fabsf(dx);

	// === Player の移動方向を判定 ===
	static float prevPlayerX = playerpos.x; // 前のフレームの Player の X 座標
	playerMoveDir = playerpos.x - prevPlayerX; // >0:右に動いた, <0:左に動いた
	prevPlayerX = playerpos.x;
	const float followThreshold = 400.0f; // この距離以上離れたら追従
	static bool isFollowing = false;

	// === CPUの挙動 ===
	if (!reachedTarget) {
		if (dist > targetDistance + expandThreshold) {
			// 相手の方へ移動
			mypos.x -= speed;
		}
		else if (dist < targetDistance) {
			// 相手から離れる
			mypos.x += speed;
		}
		else {
			// 距離が許容範囲内なら基本的に止まる
			// ただしプレイヤーが大きく動いたときのみ追従
			reachedTarget = true;
		}
	}
	else
	{
		// Player が移動したときの追従
		if (playerMoveDir > 0 && dx < -targetDistance) {
			// Player が右に移動 → CPU も右に下がる
			mypos.x += speed + 1.0f;
		}
		// 追尾開始判定
		if (!isFollowing && dist > followThreshold)
		{
			isFollowing = true; // Player が遠すぎる → 追尾開始
		}

		if (isFollowing)
		{
			// Player に近づく（targetDistance + expandThreshold まで）
			if (dist > targetDistance + expandThreshold)
			{
				mypos.x += (dx > 0) ? speed : -speed;
			}
			else
			{
				isFollowing = false; // 目標距離まで詰めたら追尾終了
			}
		}
	}

	transform.position = mypos;

#if 0
		if (fabsf(playerpos.x - prevPlayerX) > expandThreshold)
		{
			mypos.x -= speed + 1.0f;
		}
#endif // 0
}
