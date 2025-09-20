#include "Cpu.h"
#include "Player.h"
#include "../ImGui/imgui.h"

// === CPUの動作 ===
// 1. Playerとの距離によるパターン (Brain)
// 500.0f以上：移動 90% (前進：90%、後進：10%)
//			　 攻撃 10% (弱P：%、中P：%、強P：%、弱K：%、中K：%、強K：%、ガード：%)
// 500.0f未満210.0以上：移動 50% (前進：50%、後進：50%)
//						攻撃 50% (弱P：%、中P：%、強P：%、弱K：%、中K：%、強K：%、ガード：%)
// 210.0以内：移動 20% (前進：20%、後進：80%)
//			　攻撃 80% (弱P：%、中P：%、強P：%、弱K：%、中K：%、強K：%、ガード：%)

// === 次のCPUの行動をいつ始めるか ===
// 前提条件.攻撃中は強制的に他の行動に切り替えない（キャンセル可能技なら別処理）
// 1.行動ごとに 持続フレーム数 を持たせる
// 2.行動再判定の 間隔をランダム化 する

CPU::CPU(bool _iscpu)
{
	player = FindGameObject<Player>();
	
	transform.position = VGet(200.0f, 14.0f, 150.0f);
	transform.rotation = VGet(0, DegToRad(90.0f), 0);

	previousStop = transform.position;

	r = 0;

	dx = 0.0f;
	dist = 0.0f;
	targetDistance = 10.0f;
	expandThreshold = 200.0f;
	followThreshold = 500.0f;

	isCpu = _iscpu;
	reachedTarget = false;
	isFollowing = true;

	brain = MID_COMBAT;
	UpdateDice();
}

CPU::~CPU()
{
}

void CPU::Update()
{	
	Character::Always();

	hitSpheres[4].localOffset = left_HandWorldPos - basePos + VGet(-8.0f, 3.5f, -10.0f);
	hitSpheres[7].localOffset = right_HandWorldPos - basePos + VGet(-5.0f, 7.0f, 0.0f);

	if (!isCpu) anim->Play("data/Character/Player/Fight_Idle.mv1", true);
	if (!isCpu) return;

	// === 座標取得 ===
	mypos = transform.position;
	playerpos = player->GetTransform().position;

	// === 距離によるCPUの思考パターンの決定 ===
	float ds = fabs(playerpos.x - mypos.x);
	if (ds < 210.0f) {
		brain = CLOSE_COMBAT;
	}
	else if (ds >= 210.0f && 500.0f >= ds)
	{
		brain = MID_COMBAT;
	}
	else if (ds > 500.0f) {
		brain = LONG_COMBAT;
	}

	// === CPUの思考パターンによる行動 ===
	switch (brain) {
	case CLOSE_COMBAT:
		UpdateCloseCombat();
		break;
	case MID_COMBAT:
		UpdateMidCombat();
		break;
	case LONG_COMBAT:
		UpdateLongCombat();
		break;
	}

	// EffectiveRange();

	ImGui::Begin("CPU");
	ImGui::InputInt("r", &r);
	ImGui::InputFloat("position.x", &transform.position.x);
	ImGui::InputFloat("position.y", &transform.position.y);
	ImGui::InputFloat("dx", &dx);
	ImGui::InputFloat("dist", &dist);
	ImGui::InputFloat("playerMoveDir", &playerMoveDir);
	// ImGui::Text("MyPosition：%d", (int) & mypos);
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

	// === 優先度 ===
	// 1. Playerの追尾 (右にいったら、離れる。左にいったら、追う)
	// 2. PlayerとCPUの距離で追尾の反転 (CPUが静止した位置から500.0fになったら、離れてたのを追うに変更)
	// 3. PlayerとCPUの距離で静止 (200.0fになったら、移動をやめて止まる)

	// === 最優先の処理 ===
	// 3 → 2 → 1

	dx = playerpos.x - mypos.x;
	dist = fabsf(dx);

	// === CPUの挙動 ===
	if (!reachedTarget) {
		if (dist < targetDistance + expandThreshold) { // 3
			previousStop = mypos;
			reachedTarget = true;
			isFollowing = false;
			return;
		}
	}

	// === Player が移動したときの追従 ===	
	if (!isFollowing) {
		mypos.x += speed; 
		
		if (fabs(mypos.x - previousStop.x) > followThreshold) {
			isFollowing = true;
			reachedTarget = false;
		}
	}
	else { mypos.x -= speed;} // 2
	
	transform.position = mypos;

#if 0
	// === Player の移動方向を判定 ===
	// static float prevPlayerX = playerpos.x; // 前のフレームの Player の X 座標
	// playerMoveDir = playerpos.x - prevPlayerX; // >0:右に動いた, <0:左に動いた
	// prevPlayerX = playerpos.x;

	if (playerMoveDir > 0) {
		// Player が右に移動 → CPU も右に下がる
		mypos.x += speed + 1.0f;
	}

	if (playerMoveDir < 0) {
		// Player が左に移動 → CPU も左に詰める
		mypos.x -= speed + 1.0f;
	}

	if (!reachedTarget)
	{
		// Player が移動したときの追従
		if (playerMoveDir > 0) { //  && dx < -targetDistance
			// Player が右に移動 → CPU も右に下がる
			mypos.x += speed + 1.0f;

		}
		
		if (playerMoveDir < 0) {
			mypos.x -= speed + 1.0f;
		}


		if (dist > followThreshold) {
			reachedTarget = true;
			isFollowing = true;
		}
		if (isFollowing) {
		}


		if (dx == expandThreshold) {
			reachedTarget = true;
		}
	}
	
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

	// 追尾開始判定
	if (!isFollowing && dist > followThreshold)
	{
		// isFollowing = true; // Player が遠すぎる → 追尾開始
		// Player に近づく（targetDistance + expandThreshold まで）
		if (dist > targetDistance + expandThreshold)
		{
			mypos.x += -speed;
		}
		else
		{
			isFollowing = false; // 目標距離まで詰めたら追尾終了
		}
		if (fabsf(playerpos.x - prevPlayerX) > expandThreshold)
		{
			mypos.x -= speed + 1.0f;
		}
	}
#endif // 0
}

void CPU::UpdateCloseCombat()
{
}

void CPU::UpdateMidCombat()
{
}

void CPU::UpdateLongCombat()
{
}

void CPU::UpdateDice()
{
	r = rand() % 100; // 0～99 の乱数を作る
}
