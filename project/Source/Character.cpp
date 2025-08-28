#include "Character.h"
#include <assert.h>
#include "../ImGui/imgui.h"
#include "Stage.h"
#include "HitCheck.h"
#include "HPber.h"

#define PLAYER_SPEED 2.0f
#define PLAYER_JUMP 25.0f
#define PLAYER_HP 1000

Character::Character()
{
	hModel = MV1LoadModel("data/Character/Armature/Armature.mv1");
	assert(hModel >= 0);

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

	if (isPlayer) {
		transform.position = VGet(-200.0f, 14.0f, 150.0f);
		transform.rotation = VGet(0, DegToRad(-90.0f), 0);
	}
	else
	{
		transform.position = VGet(200.0f, 14.0f, 150.0f);
		transform.rotation = VGet(0, DegToRad(90.0f), 0);
	}
}

void Character::UpdateStop()
{
}

void Character::UpdatePunch1()
{
}

void Character::UpdatePunch2()
{
}

void Character::UpdatePunch3()
{
}

void Character::UpdateKick1()
{
}

void Character::UpdateKick2()
{
}

void Character::UpdateKick3()
{
}

void Character::UpdateProtect()
{
}

void Character::UpdateJump()
{
}

void Character::UpdateDamage(int dmg)
{
}

void Character::CollisionDetection()
{
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
	float minDist = E_collder->radius;

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

	if (!isPlayer) {
		hitSpheres[4].localOffset = left_HandWorldPos - basePos + VGet(-8.0f, 3.5f, -10.0f);
		hitSpheres[7].localOffset = right_HandWorldPos - basePos + VGet(-5.0f, 7.0f, 0.0f);
	}
}
