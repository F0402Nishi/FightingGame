#pragma once
#include "Object3D.h"
#include "Animator.h"
#include "Field.h"
#include <string>
#include <vector>
#include "HitCheck.h"

class Player : public Object3D
{
public:
	Player(bool _isPlayer);
	~Player();
	void Update() override;
	void Draw() override;
	void SetOpponent(Player* other);
	void SetDamage(int dmg);
	void ResolvePlayerCollision();
	void InitHitSpheres();
	std::vector<SphereCollder> hitSpheres;

private:
	Animator* anim;
	// Field* field;
	bool isJumping = false;
	VECTOR velocity;
	float velocityY = 0.0f;
	int Hp = 1000;
	int damage;
	// float time = 0.0f;
	bool isPlayer;
	Player* opponent;
	VECTOR basePos;
	VECTOR worldCenter;

	// PlayerÇÃçúêßå‰
	int headBone;
	int bodyBone;
	int left_UpperArmBone;
	int left_LowerArmBone;
	int left_HandBone;
	int right_UpperArmBone;
	int right_LowerArmBone;
	int right_HandBone;
	int left_UpperLegBone;
	int left_LowerLegBone;
	int left_FootBone;
	int right_UpperLegBone;
	int right_LowerLegBone;
	int right_FootBone;
	VECTOR heardWorldPos;
	VECTOR bodyWorldPos;
	VECTOR left_UpperArmWorldPos;
	VECTOR left_LowerArmWorldPos;
	VECTOR left_HandWorldPos;
	VECTOR right_UpperArmWorldPos;
	VECTOR right_LowerArmWorldPos;
	VECTOR right_HandWorldPos;
	VECTOR left_UpperLegWorldPos;
	VECTOR left_LowerLegWorldPos;
	VECTOR left_FootWorldPos;
	VECTOR right_UpperLegWorldPos;
	VECTOR right_LowerLegWorldPos;
	VECTOR right_FootWorldPos;

	enum State {
		S_STOP,
		S_ATTACK1,
		S_ATTACK2,
		S_ATTACK3,
		S_JUMP
	};

	State state;
	void UpdateStop();
	void UpdateAttack1();
	void UpdateAttack2();
	void UpdateAttack3();
	void UpdateJump();
};