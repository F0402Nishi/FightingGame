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
	transform.position = VGet(-200.0f, 14.0f, 150.0f);
	transform.rotation = VGet(0, DegToRad(-90.0f), 0);
}

Character::~Character()
{
}

void Character::Draw()
{
}

void Character::SetHitSpheres()
{
}

void Character::SetOpponent(Character* other)
{
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
}

void Character::BoneCollision()
{
}
