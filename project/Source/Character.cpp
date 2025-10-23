#include "Character.h"
#include <assert.h>
#include "../ImGui/imgui.h"
#include "Stage.h"
#include "HitCheck.h"
#include "2DUI.h"

#define PLAYER_SPEED 2.0f
#define PLAYER_JUMP 25.0f
#define PLAYER_HP 1000

struct AttackData {
	int hitStartFrame; // 当たり判定が「出る」最初のフレーム
	int hitEndFrame; // 当たり判定が「消える」最後のフレーム
	int cancelStartFrame; // 別の技へ“キャンセルしてよい”最初のフレーム
	int cancelEndFrame; // キャンセル“してよい”最後のフレーム
};

static const AttackData Punch1Data = { 7, 10, 0, 6 }; // 15
static const AttackData Punch2Data = { 7, 11, 0, 6 }; // 19
static const AttackData Punch3Data = { 12, 17, 0, 11 }; // 22
static const AttackData Kick1Data = { 15, 20, 0, 14 }; // 27
static const AttackData Kick2Data = { 15, 20, 0, 14 }; // 27
static const AttackData Kick3Data = { 18, 23, 0, 17 }; // 44

Character::Character()
{
	hModel = MV1LoadModel("data/Character/Armature/Armature.mv1");
	assert(hModel >= 0);

	transform.position = VGet(-200.0f, 14.0f, 150.0f);
	transform.rotation = VGet(0, DegToRad(-90.0f), 0);
	transform.scale = VGet(2, 2, 2);
	anim = new Animator(hModel);
	// anim->Play("data/Character/Player/Fight_Idle.mv1", true, true);
	E_collder = new EllipseCollider(VGet(0, 150, 0), VGet(0, 150, 0), 200);

	state = S_STOP;
	colIndex = 0;
	deltaTime = 1.0f / 60.0f;
	idleTimer = 0.0f;
	framespeed = 0.2f;

	canReduceHp = false;
	canCancel = false;
	isAltIdle = false;
	isMoveing = false;
	isGuarding = false;
	isHitPlaying = false;
	isAlive = true;
	GuardOn = false;
	startPosSaved = false;

	Hp = PLAYER_HP;
	MaxHp = PLAYER_HP;
	speed = PLAYER_SPEED;

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
}

Character::~Character()
{
	delete anim;
}

void Character::Always()
{
	anim->Update();

	BoneCollision();
	ResolvePlayerCollision();

	myPos = transform.position;
	opPos = opponent->GetTransform().position;
	direction = VNorm(opPos - myPos);

	if (isHitPlaying) {
		if (anim->IsFinish()) { isHitPlaying = false; }
		else { return; }
	}

	switch (state) {
	case S_STOP:
		UpdateStop(deltaTime);
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
}

void Character::Draw()
{
	Object3D::Draw();

	basePos = transform.position;
	for (const SphereCollder& col : hitSpheres) {
		worldCenter = col.GetWorldCenter(basePos);
		//DrawSphere3D(VAdd(worldCenter, VGet(0, 0, 0)), col.radius, 20, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
	}

	if (!anim) return;
	const int h = anim->GetModelHandle();
	if (h >= 0) { MV1DrawModel(h); }
}

void Character::SetHitSpheres()
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

void Character::SetOpponent(Character* other)
{
	opponent = other;
}

void Character::UpdateStop(float deltaTime)
{
	inputDir = VGet(0, 0, 0);
	idleTimer += deltaTime;

	if (!isAltIdle && !isHitPlaying) {
		anim->Play("data/Character/Player/Fight_Idle.mv1", true); // , false のちに追加
		// PlayAttack("data/Character/Player/Fight_Idle.mv1", true);

		if (!isAlive) return;

		if (idleTimer > 10.0f) {
			anim->Play("data/Character/Player/Taunt.mv1", false);
			isAltIdle = true;
			idleTimer = 0.0f;

		}
	}

	if (anim->IsFinish() && isAltIdle) {
		isAltIdle = false;
		idleTimer = 0.0f;
		return;
	}
}

void Character::UpdatePunch1()
{
 	PlayAttack("data/Character/Player/Atk_P_1.mv1", false);
	
	frame = anim->CurrentAnimTime();
	total = anim->TotalTime();
	ratio = anim->NormalizedTime();
	
	if (opponent != nullptr) {
		if (frame >= Punch1Data.hitStartFrame && frame <= Punch1Data.hitEndFrame) {
			colIndex = 4;
			damage = 20;
			attacktype = AttackType::Punch;

			// 相手がガード中かどうか判定
			if (opponent->isGuarding) { damage = 0; } // ガード中はダメージを0に
			
			CollisionDetection();
		}

		// if (frame >= Punch1Data.cancelStartFrame && frame <= Punch1Data.cancelEndFrame) { canCancel = true; }
		// else { canCancel = false; }
	}


	InReturn();
}

void Character::UpdatePunch2()
{
	PlayAttack("data/Character/Player/Atk_P_2.mv1", false);
	
	frame = anim->CurrentAnimTime();
	total = anim->TotalTime();
	
	if (opponent != nullptr) {
		if (frame >= Punch2Data.hitStartFrame && frame <= Punch2Data.hitEndFrame) {
			colIndex = 7;
			damage = 50;
			attacktype = AttackType::Punch;

			if (opponent->isGuarding) { damage = static_cast<int>(damage * 0.2f); } // ガード中はダメージが2割に
			
			CollisionDetection();
		}

		// if (frame >= Punch2Data.cancelStartFrame && frame <= Punch2Data.cancelEndFrame) { canCancel = true; }
		// else { canCancel = false; }
	}

	InReturn();
}

void Character::UpdatePunch3()
{
	PlayAttack("data/Character/Player/Atk_P_3.mv1", false);
	
	frame = anim->CurrentAnimTime();
	total = anim->TotalTime();

	if (opponent != nullptr) {
		if (frame >= Punch3Data.hitStartFrame && frame <= Punch3Data.hitEndFrame) {
			colIndex = 4;
			damage = 70;
			attacktype = AttackType::Punch;

			if (opponent->isGuarding) { damage = static_cast<int>(damage * 0.5f); } // ガード中はダメージが5割に
			
			CollisionDetection();
		}

		// if (frame >= Punch3Data.cancelStartFrame && frame <= Punch3Data.cancelEndFrame) { canCancel = true; }
		// else { canCancel = false; }
	}

	InReturn();
}

void Character::UpdateKick1()
{
	frame = anim->CurrentAnimTime();
	total = anim->TotalTime();

	if (!startPosSaved) {
		startPos = transform.position;
		startPosSaved = true;
	}

	// === 攻撃モーション補正（踏み込み・振りかぶり）===
	direction.z = 0; // Zは後で別に補正するのでここでは無視
	VECTOR offset;

	if (frame < Kick1Data.hitStartFrame) {
		offset = VScale(direction, -0.5f); // 振りかぶり
	}
	else if (frame >= Kick1Data.hitStartFrame && frame <= Kick1Data.hitEndFrame) {
		offset = VScale(direction, 1.0f);  // 踏み込み
	}
	else {
		offset = VScale(direction, 0.2f);  // 振り抜き
	}

	// === Z軸だけ別にずらす ===
	if (frame <= 16) {
		// 例えば、自分が左なら -1.5、右なら +1.5
		zOffset = (myPos.x < opPos.x) ? -1.5f : 1.5f;
	}
	else {
		// 16フレームを超えたらゆっくり戻る
		float diffZ = startPos.z - transform.position.z;
		zOffset = diffZ * framespeed; // 徐々にstartPosへ近づく
	}

	// Z方向の最終補正を offset に反映
	offset.z = zOffset;

	// 位置更新
	transform.position = VAdd(myPos, offset);

	PlayAttack("data/Character/Player/Atk_K_1.mv1", false);
	
	if (opponent != nullptr) {
		// === ヒット判定 ===
		if (frame >= Kick1Data.hitStartFrame && frame <= Kick1Data.hitEndFrame) {
			colIndex = 13;
			damage = 20;
			attacktype = AttackType::Kick;

			if (opponent->isGuarding) { damage = 0; } // ガード中はダメージを0に
			
			CollisionDetection();
		}

		// if (frame >= Kick1Data.cancelStartFrame && frame <= Kick1Data.cancelEndFrame) { canCancel = true; }
		// else { canCancel = false; }
	}

	InReturn();
}

void Character::UpdateKick2()
{
	PlayAttack("data/Character/Player/Atk_K_2.mv1", false);
	
	frame = anim->CurrentAnimTime();
	total = anim->TotalTime();

	if (opponent != nullptr) {
		if (frame >= Kick2Data.hitStartFrame && frame <= Kick2Data.hitEndFrame) {
			colIndex = 10;
			damage = 50;
			attacktype = AttackType::Kick;

			if (opponent->isGuarding) { damage = static_cast<int>(damage * 0.2f); } // ガード中はダメージが2割に
			
			CollisionDetection();
		}

		// if (frame >= Kick2Data.cancelStartFrame && frame <= Kick2Data.cancelEndFrame) { canCancel = true; }
		// else { canCancel = false; }

	}

	InReturn();
}

void Character::UpdateKick3()
{
	PlayAttack("data/Character/Player/Atk_K_3.mv1", false);
	
	frame = anim->CurrentAnimTime();
	total = anim->TotalTime();

	if (opponent != nullptr) {
		if (frame >= Kick3Data.hitStartFrame && frame <= Kick3Data.hitEndFrame) {
			colIndex = 13;
			damage = 70;
			attacktype = AttackType::Kick;

			if (opponent->isGuarding) { damage = static_cast<int>(damage * 0.5f); } // ガード中はダメージが5割に
			
			CollisionDetection();
		}

		// if (frame >= Kick3Data.cancelStartFrame && frame <= Kick3Data.cancelEndFrame) { canCancel = true; }
		// else { canCancel = false; }

	}

	InReturn();
}

void Character::UpdateProtect()
{
	PlayAttack("data/Character/Player/Guard_Idle.mv1", false);

	if (anim->IsFinish()) {
		isMoveing = false;
		isGuarding = false;
		canReduceHp = false;
		state = S_STOP;
		return;
	}

	CollisionDetection();
}

/// <summary>
/// 攻撃キャンセル処理の試作
/// 時間が足りないため、一時保留
/// </summary>
/// <param name="animFile"></param>
/// <param name="loop"></param>
void Character::PlayAttack(const std::string& animFile, bool loop)
{
	if (!anim) { return; } // 安全対策

	const std::string cur = anim->GetCurrentFile(); // 現在再生中のアニメ名を1回だけ取得

	// Idle からの移行なら force = false、攻撃中など Idle 以外からの移行なら force = true
	// const bool isFromIdle = (cur == "data/Character/Player/Fight_Idle.mv1"); 
	// const bool force = !isFromIdle;

	anim->Play(animFile, loop); // , true のちに追加

	//ImGui::Begin("PlayAttack");
	//ImGui::Text("Current Animation: %s", cur.c_str());
	//ImGui::Text("isFromIdle: %s", isFromIdle ? "true" : "false");
	//ImGui::Text("force: %s", force ? "true" : "false");
	//ImGui::End();
}

void Character::InReturn()
{
	if (anim->IsFinish()) {
		startPosSaved = false; // 次回攻撃用にフラグリセット
		isMoveing = false; // 攻撃終了
		canReduceHp = false;
		state = S_STOP; // 状態を通常に戻す
		return;
	}
}

void Character::UpdateJump()
{
	transform.position.y += velocityY;
}

void Character::UpdateDamage(int dmg, AttackType type)
{
	Hp -= dmg;

	if (!isHitPlaying) { 
		switch (type) {
		case AttackType::Punch:
			if (!isGuarding) { anim->Play("data/Character/Player/Hit_A.mv1", false); }
			else { anim->Play("data/Character/Player/Hit_F.mv1", false); }
			break;
		case AttackType::Kick:
			anim->Play("data/Character/Player/Hit_B.mv1", false);
			break;
		}
		isHitPlaying = true;
	}
}

void Character::SetAlive(bool ali)
{
	isAlive = ali;

	if (!isAlive) {
		state = S_STOP;
		inputDir = VGet(0, 0, 0);
	}
}

void Character::ApplyAttackMotion()
{
}

void Character::CollisionDetection()
{
	if (opponent != nullptr) {
		attackPos = hitSpheres[colIndex].GetWorldCenter(transform.position);
		attackRadius = hitSpheres[colIndex].radius;
		hitPart = HitCheck::CheckHitToPart(*opponent, attackPos, attackRadius);
		if (!hitPart.empty() && canReduceHp) {
			opponent->UpdateDamage(damage, attacktype);
			canReduceHp = false;
		}
	}
}

void Character::ResolvePlayerCollision()
{
	Character* c = FindGameObject<Character>();
	if (c == nullptr || c == this) { return; }

	// カプセルの中心線（今回は left〜right の midpoint）
	VECTOR center = (E_collder->left + E_collder->right) * 0.5f + transform.position;
	VECTOR center2 = (c->E_collder->left + c->E_collder->right) * 0.5f + c->GetTransform().position;

	// 距離ベクトルと長さ
	VECTOR diff = center2 - center;
	float dist = VSize(diff);
	float minDist = E_collder->radius * 0.7f; // 半径に係数をかける(もっと近づける)

	// 重なっている（当たり判定）
	if (dist < minDist && dist > 0.0001f)
	{
		float overlap = minDist - dist;
		VECTOR dir = VNorm(diff); // 押し返し方向（単位ベクトル）

		// 双方を均等に押し返す
		transform.position -= dir * (overlap * 0.5f);
		c->transform.position += dir * (overlap * 0.5f);
	}
}

void Character::BoneCollision()
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
}
