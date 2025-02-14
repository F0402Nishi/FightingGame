#include "Goblin.h"
#include "Field.h"
#include <assert.h>
#include "../ImGui/imgui.h"
#include "Player.h"

Goblin::Goblin() : Goblin(VGet(0, 0, 0), 0)
{
}

Goblin::Goblin(VECTOR pos, float rotY)
{
	hModel = MV1LoadModel("data/Character/Goblin/Goblin.mv1");
	assert(hModel >= 0);

	anim = new Animator(hModel);
	anim->Play("data/Character/Goblin/Anim_Neutral.mv1", true);

	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));

	hWeapon = MV1LoadModel("data/Character/Weapon/Axe/Axe.mv1");
	assert(hWeapon >= 0);

	state = S_IDLE;

	transform.position = pos;
	transform.rotation.y = rotY;
	basePosition = pos;
}

Goblin::~Goblin()
{
	delete anim;
}

void Goblin::Update()
{
	ImGui::Begin("GOBLIN");
	ImGui::InputInt("State", (int*)&state);
	ImGui::End();

	anim->Update();

	switch (state) {
	case S_IDLE:
		UpdateIdle();
		break;
	case S_RUN:
		UpdateRun();
		break;
	case S_ATTACK:
		UpdateAttack();
		break;
	case S_DAMAGE:
		UpdateDamage();
		break;
	}

	// 地面に立たせる
	Field* field = FindGameObject<Field>();
	VECTOR hitPos; // 当たったら場所を返してもらう
	if (field->SearchGround(transform.position + VGet(0, 1000, 0), transform.position + VGet(0, -1000, 0), &hitPos)) {
		transform.position = hitPos;
	}
}

void Goblin::Draw()
{
	Object3D::Draw();

	int wp = MV1SearchFrame(hModel, "hansocketR");
	MATRIX m = MV1GetFrameLocalWorldMatrix(hModel, wp);
	MV1SetMatrix(hWeapon, m);
	MV1DrawModel(hWeapon);
}

bool Goblin::CheckLine(VECTOR p1, VECTOR p2)
{
	bool ret = false;
	auto hit = MV1CollCheck_Triangle(hModel, -1, lastP1, lastP2, p1);
	if (hit.HitNum > 0) {
		ret = true;
		// return true;
	}

	hit = MV1CollCheck_Triangle(hModel, -1, p1, lastP2, p2);
	if (hit.HitNum > 0) {
		ret = true;
		// return true;
	}

	if (ret) {
		anim->Play("data/Character/Player/Anim_Damage.mv1", false);
		state = S_DAMAGE;

	}

	lastP1 = p1;
	lastP2 = p2;
	return ret;
}

void Goblin::UpdateIdle()
{
	Player* p = FindGameObject<Player>();
	if (p == nullptr) {
		return;
	}

	// Goblinの視野にPlayerが、入ったら…
	VECTOR forPlayer = p->GetTransform().position - transform.position;
	if (VSize(forPlayer) > 1000) /* 距離を確認 */ {
		return;
	}
	VECTOR forward = VGet(0, 0, 1) * MGetRotY(transform.rotation.y);
	VECTOR a = VNorm(forPlayer);
	float dot = VDot(forward, a); // cosθを求める
	if (dot >= cos(DegToRad(30.0f))) {
		anim->Play("data/Character/Goblin/Anim_Walk.mv1", true);
		state = S_RUN;
	}
}

void Goblin::UpdateRun()
{
	Player* p = FindGameObject<Player>();
	VECTOR forPlayer = p->GetTransform().position - transform.position;
	if (VSize(forPlayer) <= 150.0f) {
		anim->Play("data/Character/Goblin/Anim_Attack1.mv1", false);
		state = S_ATTACK;
	}

#if false // 角度バージョン
	// Playerへのベクトル
	float ang = atan2(forPlayer.x, forPlayer.z);
	{
		ImGui::Begin("Angle");
		ImGui::InputFloat("ang", & ang);
		ImGui::InputFloat("rotation", &transform.rotation.y);
		ImGui::End();
	}

	float diff = ang - transform.rotation.y; // 差を求める
	// diffを -P1～P1の間に収める

	while (diff > DX_PI)
	{
		diff -= DX_PI * 2.0f; //度でいうと、360を引いたのと同じ
	}
	while (diff < -DX_PI)
	{
		diff += DX_PI * 2.0f; //度でいうと、360を足したのと同じ
	}
	if (diff > 0) /*自分の向きが小さいので*/ {
		transform.rotation.y += DegToRad(5.0f); //5度増やす
	}
	if (diff < 0) /*自分の向きが大きいので*/ {
		transform.rotation.y -= DegToRad(5.0f); //5度減らす
	}

#else // 内積バージョン
	VECTOR right = VGet(1, 0, 0) * MGetRotY(transform.rotation.y);
	float dot = VDot(right, forPlayer);
	if (dot > 0) {
		transform.rotation.y += DegToRad(5.0f);
	}
	if (dot < 0) {
		transform.rotation.y -= DegToRad(5.0f);
	}
#endif

	VECTOR forward = VGet(0, 0, 1) * MGetRotY(transform.rotation.y);
	transform.position += forward * 100.0f * Time::DeltaTime();
	if (VSize(basePosition - transform.position) > 1000) {
		state = S_BACK;
	}
}

void Goblin::UpdateAttack()
{
	if (anim->IsFinish()) {
		anim->Play("data/Character/Goblin/Anim_Neutral.mv1", true);
		state = S_IDLE;
	}
}

void Goblin::UpdateBack()
{
	VECTOR right = VGet(1, 0, 0) * MGetRotY(transform.rotation.y);
	VECTOR forBase = basePosition - transform.position;
	float dot = VDot(right, forBase);
	if (dot > 0) {
		transform.rotation.y += DegToRad(5.0f);
	}
	if (dot < 0) {
		transform.rotation.y -= DegToRad(5.0f);
	}

	VECTOR forward = VGet(0, 0, 1) * MGetRotY(transform.rotation.y);
	transform.position += forward * 100.0f * Time::DeltaTime();

	if (VSize(forBase) < 100) {
		anim->Play("data/Character/Goblin/Anim_Neutral.mv1", true);
		state = S_IDLE;
	}
}

void Goblin::UpdateDamage()
{
	if (anim->IsFinish()) {
		anim->Play("data/Character/Player/Anim_Neutral.mv1", true);
		state = S_IDLE;
	}
}
