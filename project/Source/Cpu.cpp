#include "Cpu.h"
#include "Player.h"
#include "Character.h"
#include "../ImGui/imgui.h"

// === CPUの動作 ===
// 1. Playerとの距離によるパターン (Brain)
// 500.0f以上：移動 75% (前進：90%、後進：10%)
//			　 攻撃 15% (弱P：5%、中P：30%、強P：30%、弱K：5%、中K：20%、強K：10%)
//			　 ガード 10%
// 500.0f未満210.0以上：移動 45% (前進：50%、後進：50%)
//						攻撃 45% (弱P：11%、中P：28%、強P：17%、弱K：11%、中K：22%、強K：11%)
//						ガード 10%
// 210.0以内：移動 20% (前進：20%、後進：80%)
//			　攻撃 70% (弱P：15%、中P：25%、強P：15%、弱K：10%、中K：20%、強K：15%)
//			　ガード 10%

// === 次のCPUの行動をいつ始めるか ===
// 前提条件.攻撃中は強制的に他の行動に切り替えない（キャンセル可能技なら別処理）
// 1.行動ごとに 持続フレーム数 を持たせる
// 2.行動再判定の 間隔をランダム化 する

// === メモ ===
// 攻撃は、種類が多くて打つのが大変なので、配列で処理する

CPU::CPU(bool _iscpu)
{
	player = FindGameObject<Player>();
	
	transform.position = VGet(200.0f, 14.0f, 183.0f);
	transform.rotation = VGet(0, DegToRad(90.0f), 0);

	previousStop = transform.position;

	r = 0;
	m = 0;
	a = 0;
	sum = 0;

	dx = 0.0f;
	dist = 0.0f;
	targetDistance = 10.0f;
	expandThreshold = 200.0f;
	followThreshold = 500.0f;
	time = 0;
	CPUpos = 0;

	isCpu = _iscpu;
	reachedTarget = false;
	isFollowing = true;
	NowAnim = false;
	NowDice = false;
	NowMovement = false;
	NowAttack = false;
	Nowpos = false;
	actionFinished = false;
	GuardNow = false;

	brain = MID_COMBAT;

	// === 確率（合計100）===
	attackCount = sizeof(Mid_attack) / sizeof(int);
}

CPU::~CPU()
{
}

void CPU::Update()
{	
	Character::Always();

	hitSpheres[4].localOffset = left_HandWorldPos - basePos + VGet(-8.0f, 3.5f, -10.0f);
	hitSpheres[7].localOffset = right_HandWorldPos - basePos + VGet(-5.0f, 7.0f, 0.0f);

	if (!isCpu && CheckHitKey(KEY_INPUT_Q) && !GuardNow) { GuardOn = !GuardOn; GuardNow = true; }
	else if (!CheckHitKey(KEY_INPUT_Q) && GuardNow) { GuardNow = false; }
	if (GuardOn) {
		state = S_PROTECT; isGuarding = true;
	}
	else
	{
		state = S_STOP;
	}
	
	ImGui::Begin("CPU");
	ImGui::Checkbox("GuardOn", &GuardOn);
	ImGui::Checkbox("GuardNow", &GuardNow);
	ImGui::End();
	
	if (!isCpu) return;

	// === HPが0以下なら行動を止める ===
	if (!isAlive) {
		time = 0;
		r = 0;
		m = 0;
		a = 0;
		return;
	}

	// === 座標取得 ===
	mypos = transform.position;
	playerpos = player->GetTransform().position;

	// === 距離によるCPUの思考パターンの決定 ===
	if (!isMoveing) {
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
	}


	// === CPUの思考パターン別による関数に移動 ===
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

	// 左右移動
	if (VSize(inputDir) > 0) {
		if (VSize(inputDir) >= 1.0f) {
			inputDir = VNorm(inputDir);
		}
		CPUvelocity = inputDir * speed;
		transform.position += CPUvelocity;
	}

	// EffectiveRange();
	time += 1.0f;

#if 0
	ImGui::Checkbox("isMoving", &isMoveing);
	ImGui::Checkbox("isAlive", &isAlive);
	ImGui::Text("brain: %d", (int)brain);
	ImGui::Text("state: %d", (int)state);
	ImGui::InputInt("damage", &damage);
	ImGui::InputInt("dice", &r);
	ImGui::InputInt("movement", &m);
	ImGui::InputInt("attack", &a);
	ImGui::InputFloat("time", &time);
	ImGui::InputInt("speed", &speed);
	ImGui::InputFloat("position.x", &transform.position.x);
	ImGui::InputFloat("position.y", &transform.position.y);
	ImGui::InputFloat("dx", &dx);
	ImGui::InputFloat("dist", &dist);
	ImGui::InputFloat("playerMoveDir", &playerMoveDir);
	ImGui::InputFloat("float moved", &moved);
	ImGui::InputFloat("float CPUpos", &CPUpos);
	ImGui::Text("MyPosition：%d", (int) & mypos);
#endif // 0
}

void CPU::Draw()
{
	std::cout << "CPU Draw" << std::endl;

	Character::Draw();
}

void CPU::UpdateCloseCombat()
{
	UpdateDice();

	if (r < 20) { // 0～19 → 移動 20%
		if (!Nowpos) { CPUpos = mypos.x; }
		// NowMovement = true; // m もリセット
		isMoveing = true;
		NowAttack = true;

		if (m < 20) {
			inputDir.x = -1.0f;
			Nowpos = true;
		}
		else {
			inputDir.x = 10.0f;
			NowMovement = false;
			Nowpos = true;
		}

		moved = fabs(mypos.x - CPUpos);
		if (moved >= 10.0f) {
			isMoveing = false;
			Nowpos = false;
		}

	}
	else if (r < 90) { // 20～89 → 攻撃 70%
		// NowAttack = true; // a もリセット
		NowMovement = true;

		// === 攻撃系 ===
		for (int i = 0; i < attackCount; i++) {
			sum += Close_attack[i];
			if (a < sum) {
				if (state != attackStates[i]) {
					state = attackStates[i];
					canReduceHp = true;
				}
				isMoveing = true;
				sum = 0;
				break;
			}
		}
	}
	else { // 90〜99 → ガード（10%）
		state = S_PROTECT;
		isGuarding = true;
		isMoveing = true;
	}
}

void CPU::UpdateMidCombat()
{
	UpdateDice();
	
	if (r < 45) { // 0〜44 → 移動 45%
		if (!Nowpos) { CPUpos = mypos.x; }
		// NowMovement = true; // m もリセット
		isMoveing = true;
		NowAttack = true;

		if (m < 50) {
			inputDir.x = -5.0f;
			Nowpos = true;
			moved = fabs(mypos.x - CPUpos);
			if (moved >= 5.0f) {
				isMoveing = false;
				Nowpos = false;
			}
		}
		else {
			inputDir.x = 10.0f;
			NowMovement = false;
			Nowpos = true;
			moved = fabs(mypos.x - CPUpos);
			if (moved >= 10.0f) {
				isMoveing = false;
				Nowpos = false;
			}
		}

	}
	else if (r < 90){ // 45〜89 → 攻撃 45%
		// NowAttack = true; // a もリセット
		NowMovement = true;

		// === 攻撃系 ===
		for (int i = 0; i < attackCount; i++) {
			sum += Mid_attack[i];
			if (a < sum) {
				state = attackStates[i];
				canReduceHp = true;
				isMoveing = true;
				sum = 0;
				break;
			}
		}
	}
	else { // 90〜99 → ガード（10%）
		state = S_PROTECT;
		isGuarding = true;
		isMoveing = true;
	}
}

void CPU::UpdateLongCombat()
{
	UpdateDice();

	if (r < 75) { // 0～74 → 移動 75%
		if (!Nowpos) { CPUpos = mypos.x; }
		isMoveing = true;
		NowAttack = true;

		if (m < 10) {
			inputDir.x = 10.0f;
			Nowpos = true;
		}
		else {
			inputDir.x = -10.0f;
			Nowpos = true;
		}

		moved = fabs(mypos.x - CPUpos);
		if (moved >= 10.0f) {
			isMoveing = false;
			Nowpos = false;
		}
	}
	else if (r < 85) { // 75～84 → 攻撃 10%
		NowMovement = true;

		// === 攻撃系 ===
		for (int i = 0; i < attackCount; i++) {
			sum += Long_attack[i];
			if (a < sum) {
				state = attackStates[i];
				canReduceHp = true;
				isMoveing = true;
				sum = 0;
				break;
			}
		}
	}
	else { // 85～94 → ガード 10%
		state = S_PROTECT;
		isGuarding = true;
		isMoveing = true;
	}
}

void CPU::UpdateDice()
{
	// ===== 新しい行動を決める =====
	if (!isMoveing && time > 100.0f) { 
		NowDice = false;
		NowMovement = false;
		NowAttack = false; 
		time = 0;
	}
	
	// if (time == 30) { } // 0～99 の乱数を作る
	if (!NowDice) { r = rand() % 100; NowDice = true; }
	if (!NowMovement) { m = rand() % 100; NowMovement = true; }
	if (!NowAttack) { a = rand() % 100; NowAttack = true; }
		

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
