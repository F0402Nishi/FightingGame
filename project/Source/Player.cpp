#include "Player.h"
#include <assert.h>
#include "../ImGui/imgui.h"
#include "Stage.h"
#include "HitCheck.h"
#include "HPber.h"

// #define PLAYER_SPEED 2.0f
// #define PLAYER_JUMP 25.0f
// #define PLAYER_HP 1000

Player::Player(bool _isPlayer)
{

	isPlayer = _isPlayer;
	isJumping = false;
	isPunching = false;
	// state = S_STOP;
	// colIndex = 0;
	// isMoveing = false;
	// isGuarding = false;
	// canReduceHp = false;
	// Hp = PLAYER_HP;
	// MaxHp = PLAYER_HP;

	if (isPlayer) {
		transform.position = VGet(-200.0f, 14.0f, 150.0f);
		transform.rotation = VGet(0, DegToRad(-90.0f), 0);
	}

#if 0 
	transform.scale = VGet(2, 2, 2);
	anim = new Animator(hModel);
	E_collder = new EllipseCollider(VGet(0, 150, 0), VGet(0, 150, 0), 200);


	// Playerの骨を取得して、番号をつけてる
	headBone = MV1SearchFrame(hModel, "Head");
	bodyBone = MV1SearchFrame(hModel, "UpperChest");
	left_UpperArmBone = MV1SearchFrame(hModel, "Left_UpperArm");
	left_LowerArmBone = MV1SearchFrame(hModel, "Left_LowerArm");
	left_HandBone = MV1SearchFrame(hModel, "Left_Hand");
	right_UpperArmBone = MV1SearchFrame(hModel, "Right_UpperArm");
	right_LowerArmBone = MV1SearchFrame(hModel, "Right_LowerArm");
	right_HandBone = MV1SearchFrame(hModel, "Right_Hand");
	left_UpperLegBone = MV1SearchFrame(hModel, "Left_UpperLeg");
	left_LowerLegBone = MV1SearchFrame(hModel, "Left_LowerLeg");
	left_FootBone = MV1SearchFrame(hModel, "Left_Foot");
	right_UpperLegBone = MV1SearchFrame(hModel, "Right_UpperLeg");
	right_LowerLegBone = MV1SearchFrame(hModel, "Right_LowerLeg");
	right_FootBone = MV1SearchFrame(hModel, "Right_Foot");


	// アニメーションの制御実験
	int mB = MV1SearchFrame(hModel, "UpperChest");
	int mc = MV1SearchFrame(hModel, "Neck");
	int md = MV1SearchFrame(hModel, "Head");
	MV1SetFrameUserLocalMatrix(hModel, mB, MGetIdent());
	MV1SetFrameUserLocalMatrix(hModel, mc, MGetIdent());
	MV1SetFrameUserLocalMatrix(hModel, md, MGetIdent());
	hips_Bone = MV1SearchFrame(hModel, "Hips");
	hips_WorldPos = MV1GetFramePosition(hModel, hips_Bone);
#endif
}

Player::~Player()
{
	// delete anim;
}

void Player::Update()
{
	Character::Always();

	// !isPlayerの攻撃判定用Colliderの位置を調整
	if (!isPlayer) {
		hitSpheres[4].localOffset = left_HandWorldPos - basePos + VGet(-8.0f, 3.5f, -10.0f);
		hitSpheres[7].localOffset = right_HandWorldPos - basePos + VGet(-5.0f, 7.0f, 0.0f);
	}

	if (!isPlayer) anim->Play("data/Character/Player/Fight_Idle.mv1", true, false);
	if (!isPlayer) return;

	if (state == S_STOP) {
		if (CheckHitKey(KEY_INPUT_A)) {
			inputDir.x = -10.0f;
			// anim->Play("data/Character/Player/Walk_B.mv1", true); // 後ろ歩き
		}
		else if (CheckHitKey(KEY_INPUT_D)) {
			inputDir.x = 10.0f;
			// anim->Play("data/Character/Player/Walk_F.mv1", true); // 前歩き
		}
	}

	// 左右移動
	if (VSize(inputDir) > 0) {
		if (VSize(inputDir) >= 1.0f) {
			inputDir = VNorm(inputDir);
		}
		velocity = inputDir * speed;
		transform.position += velocity;
	}

	// KEYが押しっぱなしの時に、canReduceHpがtrueとfalseを繰り返している
	// そのため、ダメージが重複している。かつ、アニメーションは１回しか再生されておらず分かりにくい
	// KEY_INPUTの際に、bool型の変数を追加して条件を「KEY_INPUT＆false」の時に変更
	// アニメーションが終わったときに、trueにする。

	// キャンセル用の関数または変数を定義
	// 例.Uのアニメーション中にIを押されたら、、、

	if (CheckHitKey(KEY_INPUT_U) && !isMoveing) { state = S_PUNCH1; canReduceHp = true; isMoveing = true; } // パンチ1
	if (CheckHitKey(KEY_INPUT_I) && !isMoveing) { state = S_PUNCH2; canReduceHp = true; isMoveing = true; } // パンチ2
	if (CheckHitKey(KEY_INPUT_O) && !isMoveing) { state = S_PUNCH3; canReduceHp = true; isMoveing = true; } // パンチ3
	if (CheckHitKey(KEY_INPUT_J) && !isMoveing) { state = S_KICK1; canReduceHp = true; isMoveing = true; } // キック1
	if (CheckHitKey(KEY_INPUT_K) && !isMoveing) { state = S_KICK2; canReduceHp = true; isMoveing = true;} // キック2
	if (CheckHitKey(KEY_INPUT_L) && !isMoveing) { state = S_KICK3; canReduceHp = true; isMoveing = true; } // キック3
	if (CheckHitKey(KEY_INPUT_H)) { state = S_PROTECT; isGuarding = true; } // ガード

	if (state == S_PUNCH1 && canCancel) {
		if (CheckHitKey(KEY_INPUT_I)) { state = S_PUNCH2; canReduceHp = true; isMoveing = true; }
		if (CheckHitKey(KEY_INPUT_O)) { state = S_PUNCH3; canReduceHp = true; isMoveing = true; }
	}

	ImGui::Begin("PLAYER");
	ImGui::InputFloat("position.x", &transform.position.x);
	ImGui::InputFloat("position.y", &transform.position.y);
	// ImGui::Text("push.x: %.2f", hit.x);
	// ImGui::Text("push.y: %.2f", hit.y);
	ImGui::Text("state: %d", (int)state);
	ImGui::InputFloat("frame: ", &frame);
	ImGui::InputFloat("totalframe: ", &total);
	ImGui::InputFloat("ratioframe: ", &ratio);
	// ImGui::Text("HP: %d", (int)Hp);
	ImGui::End();

#if false 
	
	anim->Update();
	
	BoneCollision();
	ResolvePlayerCollision();

	switch (state) {
	case S_STOP:
		UpdateStop();
		break;
	case S_PUNCH1:
		UpdatePunch1();
		break;
	case S_PUNCH2:
		UpdatePunch2();
		break;
	case S_PUNCH3:
		UpdatePunch3();
		break;
	case S_KICK1:
		UpdateKick1();
		break;
	case S_KICK2:
		UpdateKick2();
		break;
	case S_KICK3:
		UpdateKick3();
		break;
	case S_PROTECT:
		UpdateProtect();
		break;
	case S_JUMP:
		UpdateJump();
		break;
	}

	// 地面に立たせる
	Stage* stage = FindGameObject<Stage>();
	VECTOR hit;
	VECTOR hit1;
	if (stage->SearchObject(transform.position + VGet(0, 1000, 0), transform.position + VGet(0, -10, 0), &hit)) {
		transform.position = hit + VGet(0, 15.0f, 0);
		if (state == S_JUMP) { state = S_STOP; }
	}
	else { static const float Gravity = 1.0f; velocityY -= Gravity; }

	// 壁との当たり判定
	if (stage->SearchObject(transform.position + VGet(100, 0, 0), transform.position + VGet(-100, 0, 0), &hit1)) {
		if (transform.position.x >= hit1.x) { // 左の壁に当たった時...
			transform.position = hit1 + VGet(100, 0, 0); // hit1の位置に+ x.100を加える
		}
		else // 右の壁に当たった時...
		{
			transform.position = hit1 + VGet(-100, 0, 0); // hit1の位置に- x.100を加える
		}
	}

	// 地面に立たせる(初期構想)
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
}

void Player::Draw()
{
	Character::Draw();

#if 0
	basePos = transform.position;
	for (const SphereCollder& col : hitSpheres) {
		worldCenter = col.GetWorldCenter(basePos);
		//DrawSphere3D(VAdd(worldCenter, VGet(0, 0, 0)), col.radius, 20, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
	}

	DrawFormatString(50, 50, GetColor(255, 255, 0), "ＨＰ", Hp);
	DrawFillBox(200, 50, Hp * DRAW_SIZE, 66, GetColor(255, 255, 0));
	DrawLineBox(200, 50, Hp * DRAW_SIZE, 66, GetColor(0, 0, 0));

	DrawFormatString(50, 100, GetColor(0, 255, 255), "描画の値", DrawValue);

	// バーの色変化
	int color = GetColor(0, 255, 0);  // 通常：緑

	if (DrawValue <= PLAYER_HP * DRAW_SIZE / 2) {
		color = GetColor(255, 255, 0);  // 半分以下：黄色
	}

	if (DrawValue <= DRAW_SIZE) {
		color = GetColor(255, 0, 0);    // 残りHP1以下：赤
	}

	// アニメーションバーの描画
	DrawFillBox(200, 100, 200 + DrawValue, 116, color);
	DrawLineBox(200, 100, 200 + DrawValue, 116, GetColor(0, 0, 0));
#endif // 0
}

#if false "参考のために保留"
void Player::SetHitSpheres()
{
	hitSpheres.clear();

	// Colliderのサイズを骨ごとに調整
	hitSpheres.emplace_back(VGet(0, 0, 0), 32, "Head");
	hitSpheres.emplace_back(VGet(0, 0, 0), 60, "Body");
	hitSpheres.emplace_back(VGet(0, 0, 0), 30, "Left_UpperArm");
	hitSpheres.emplace_back(VGet(0, 0, 0), 30, "Left_LowerArm");
	hitSpheres.emplace_back(VGet(0, 0, 0), 30, "Left_Hand");
	hitSpheres.emplace_back(VGet(0, 0, 0), 30, "Right_UpperArm");
	hitSpheres.emplace_back(VGet(0, 0, 0), 30, "Right_LowerArm");
	hitSpheres.emplace_back(VGet(0, 0, 0), 30, "Right_Hand");
	hitSpheres.emplace_back(VGet(0, 0, 0), 45, "Left_UpperLeg");
	hitSpheres.emplace_back(VGet(0, 0, 0), 45, "Left_LowerLeg");
	hitSpheres.emplace_back(VGet(0, 0, 0), 42, "Left_Foot");
	hitSpheres.emplace_back(VGet(0, 0, 0), 45, "Right_UpperLeg");
	hitSpheres.emplace_back(VGet(0, 0, 0), 45, "Right_LowerLeg");
	hitSpheres.emplace_back(VGet(0, 0, 0), 42, "Right_Foot");
}

void Player::SetOpponent(Player* other)
{

	opponent = other;

	if (isPlayer) {
		transform.position = VGet(-200.0f, 14.0f, 150.0f);
		transform.rotation = VGet(0, DegToRad(-90.0f), 0);
	}
	else
	{
		transform.position = VGet(200.0f, 14.0f, 150.0f);
		transform.rotation = VGet(0, DegToRad(90.0f), 0);
	}
	// hpber->SetMaxHp(opponent->GetMaxHp());
}

void Player::UpdateStop()
{
	VECTOR inputDir = VGet(0, 0, 0);
	isMoveing = false;

	if (!isPlayer) anim->Play("data/Character/Player/Fight_Idle.mv1", true);
	if (!isPlayer) return;

	if (CheckHitKey(KEY_INPUT_A)) {
		inputDir.x = -10.0f;
		// anim->Play("data/Character/Player/Walk_B.mv1", true); //アニメーションが先行してしまうので、保留
	}
	if (CheckHitKey(KEY_INPUT_D)) {
		inputDir.x = 10.0f;
		// anim->Play("data/Character/Player/Walk_F.mv1", true); //アニメーションが先行してしまうので、保留
	}

//のちに戻す
#if false 
	if (CheckHitKey(KEY_INPUT_SPACE)) {
		velocityY = PLAYER_JUMP;
		transform.position.y += velocityY;
		state = S_JUMP;
	}
#endif

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


	if (CheckHitKey(KEY_INPUT_U)) { // パンチ1
		anim->Play("data/Character/Player/Atk_P_1.mv1", false);
		state = S_PUNCH1;
		isPunching = true;
		if (opponent != nullptr) {
			canReduceHp = true;
			colIndex = 4;
			damage = 10;

			// 相手がガード中かどうか判定
			if (opponent->isGuarding) { damage = 0; } // ガード中はダメージを0に
		}
	}
	if (CheckHitKey(KEY_INPUT_I)) { // パンチ2
		anim->Play("data/Character/Player/Atk_P_2.mv1", false);
		state = S_PUNCH2;
		if (opponent != nullptr) {
			canReduceHp = true;
			isPunching = true;
			colIndex = 7;
			damage = 50;

			// 相手がガード中かどうか判定
			if (opponent->isGuarding) { damage = static_cast<int>(damage * 0.2f); } // ガード中はダメージが2割に
		}
	}
	if (CheckHitKey(KEY_INPUT_O)) { // パンチ3
		anim->Play("data/Character/Player/Atk_P_3.mv1", false);
		state = S_PUNCH3;
		if (opponent != nullptr) {
			canReduceHp = true;
			isPunching = true;
			colIndex = 4;
			damage = 70;

			// 相手がガード中かどうか判定
			if (opponent->isGuarding) { damage = static_cast<int>(damage * 0.5f); } // ガード中はダメージが5割に
		}
	}

	if (CheckHitKey(KEY_INPUT_J)) { // キック1
		anim->Play("data/Character/Player/Atk_K_1.mv1", false);
		state = S_KICK1;
		if (opponent != nullptr) {
			canReduceHp = true;
			colIndex = 13;
			damage = 10;

			// 相手がガード中かどうか判定
			if (opponent->isGuarding) { damage = 0; } // ガード中はダメージを0に
		}
	}
	if (CheckHitKey(KEY_INPUT_K)) { // キック2
		anim->Play("data/Character/Player/Atk_K_2.mv1", false);
		state = S_KICK2;
		if (opponent != nullptr) {
			canReduceHp = true;
			colIndex = 10;
			damage = 50;

			// 相手がガード中かどうか判定
			if (opponent->isGuarding) { damage = static_cast<int>(damage * 0.2f); } // ガード中はダメージが2割に
		}
	}
	if (CheckHitKey(KEY_INPUT_L)) { // キック3
		anim->Play("data/Character/Player/Atk_K_3.mv1", false);
		state = S_KICK3;
		if (opponent != nullptr) {
			canReduceHp = true;
			colIndex = 13;
			damage = 70;

			// 相手がガード中かどうか判定
			if (opponent->isGuarding) { damage = static_cast<int>(damage * 0.5f); } // ガード中はダメージが5割に
		}
	}

	CollisionDetection();

	if (CheckHitKey(KEY_INPUT_H)) { // ガード
		anim->Play("data/Character/Player/Guard_Idle.mv1", false);
		isGuarding = true;
		state = S_PROTECT;
	}
}

void Player::UpdatePunch1()
{
	if (anim->IsFinish()) {
		isPunching = false;
		canReduceHp = false;
		state = S_STOP;
		return;
	}

	CollisionDetection();
	// ダメージの高い攻撃を実装予定
}

void Player::UpdatePunch2()
{
	if (anim->IsFinish()) {
		isPunching = false;
		canReduceHp = false;
		state = S_STOP;
		return;
	}

	CollisionDetection();
	// ダメージの高い攻撃を実装予定
}

void Player::UpdatePunch3()
{
	if (anim->IsFinish()) {
		isPunching = false;
		canReduceHp = false;
		state = S_STOP;
		return;
	}
	
	CollisionDetection();
}

void Player::UpdateKick1()
{
	if (anim->IsFinish()) {
		canReduceHp = false;
		state = S_STOP;
		return;
	}

	CollisionDetection();
}

void Player::UpdateKick2()
{
	if (anim->IsFinish()) {
		canReduceHp = false;
		state = S_STOP;
		return;
	}

	CollisionDetection();
}

void Player::UpdateKick3()
{
	if (anim->IsFinish()) {
		canReduceHp = false;
		state = S_STOP;
		return;
	}

	CollisionDetection();
}

void Player::UpdateProtect()
{
	if (anim->IsFinish()) {
		isGuarding = false;
		canReduceHp = false;
		state = S_STOP;
		return;
	}

	CollisionDetection();
}

void Player::UpdateJump()
{
//	transform.position += velocity;
	transform.position.y += velocityY;
}

void Player::UpdateDamage(int dmg)
{
	Hp -= dmg;
	// hpber->SetHp(opponent->GetHp());
#if 0
	TargetValue = Hp * DRAW_SIZE;
	if (Hp > PLAYER_HP) Hp = PLAYER_HP;
	if (Hp < 0) Hp = 0;
	if (DrawValue > TargetValue) { DrawValue--; }
#endif // 0
}

void Player::CollisionDetection()
{
	if (opponent != nullptr) {
		attackPos = hitSpheres[colIndex].GetWorldCenter(transform.position);
		attackRadius = hitSpheres[colIndex].radius;
		hitPart = HitCheck::CheckHitToPart(*opponent, attackPos, attackRadius);
		if (!hitPart.empty() && canReduceHp) {
			opponent->UpdateDamage(damage);
			canReduceHp = false;
		}
	}
}

void Player::ResolvePlayerCollision()
{
	Player* p = FindGameObject<Player>();
	if (p == nullptr || p == this) { return; }

	// カプセルの中心線（今回は left〜right の midpoint）
	VECTOR center = (E_collder->left + E_collder->right) * 0.5f + transform.position;
	VECTOR center2 = (p->E_collder->left + p->E_collder->right) * 0.5f + p->GetTransform().position;

	// 距離ベクトルと長さ
	VECTOR diff = center2 - center;
	float dist = VSize(diff);
	float minDist = E_collder->radius;

	// 重なっている（当たり判定）
	if (dist < minDist && dist > 0.0001f)
	{
		float overlap = minDist - dist;
		VECTOR dir = VNorm(diff); // 押し返し方向（単位ベクトル）

		// 双方を均等に押し返す
		transform.position -= dir * (overlap * 0.5f);
		p->transform.position += dir * (overlap * 0.5f);
	}
}

void Player::BoneCollision()
{
	// Playerの攻撃判定用Colliderの位置を調整
	heardWorldPos = MV1GetFramePosition(hModel, headBone);
	bodyWorldPos = MV1GetFramePosition(hModel, bodyBone);
	left_UpperArmWorldPos = MV1GetFramePosition(hModel, left_UpperArmBone);
	left_LowerArmWorldPos = MV1GetFramePosition(hModel, left_LowerArmBone);
	left_HandWorldPos = MV1GetFramePosition(hModel, left_HandBone);
	right_UpperArmWorldPos = MV1GetFramePosition(hModel, right_UpperArmBone);
	right_LowerArmWorldPos = MV1GetFramePosition(hModel, right_LowerArmBone);
	right_HandWorldPos = MV1GetFramePosition(hModel, right_HandBone);
	left_UpperLegWorldPos = MV1GetFramePosition(hModel, left_UpperLegBone);
	left_LowerLegWorldPos = MV1GetFramePosition(hModel, left_LowerLegBone);
	left_FootWorldPos = MV1GetFramePosition(hModel, left_FootBone);
	right_UpperLegWorldPos = MV1GetFramePosition(hModel, right_UpperLegBone);
	right_LowerLegWorldPos = MV1GetFramePosition(hModel, right_LowerLegBone);
	right_FootWorldPos = MV1GetFramePosition(hModel, right_FootBone);
	hitSpheres[0].localOffset = (heardWorldPos - basePos) + VGet(0, 10.0f, 0);
	hitSpheres[1].localOffset = bodyWorldPos - basePos;
	hitSpheres[2].localOffset = left_UpperArmWorldPos - basePos;
	hitSpheres[3].localOffset = left_LowerArmWorldPos - basePos;
	hitSpheres[4].localOffset = left_HandWorldPos - basePos + VGet(8.0f, 3.5f, -10.0f);
	hitSpheres[5].localOffset = right_UpperArmWorldPos - basePos;
	hitSpheres[6].localOffset = right_LowerArmWorldPos - basePos;
	hitSpheres[7].localOffset = right_HandWorldPos - basePos + VGet(5.0f, 7.0f, 0.0f);
	hitSpheres[8].localOffset = left_UpperLegWorldPos - basePos;
	hitSpheres[9].localOffset = left_LowerLegWorldPos - basePos;
	hitSpheres[10].localOffset = left_FootWorldPos - basePos;
	hitSpheres[11].localOffset = right_UpperLegWorldPos - basePos;
	hitSpheres[12].localOffset = right_LowerLegWorldPos - basePos;
	hitSpheres[13].localOffset = right_FootWorldPos - basePos;

	if (!isPlayer) {
		hitSpheres[4].localOffset = left_HandWorldPos - basePos + VGet(-8.0f, 3.5f, -10.0f);
		hitSpheres[7].localOffset = right_HandWorldPos - basePos + VGet(-5.0f, 7.0f, 0.0f);
	}
}
#endif
