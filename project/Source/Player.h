#pragma once
#include "Object3D.h"
#include "Animator.h"
#include "Field.h"
#include <string>
#include <vector>

class HPber;

class Player : public Object3D
{
public:
	Player(bool _isPlayer);
	~Player();
	void Update() override;
	void Draw() override;
	void SetHitSpheres();
	void SetOpponent(Player* other);

	std::vector<SphereCollder> hitSpheres;
	std::string GetHit() { return hitPart; }
	int GetHp() const { return Hp; }
	int GetMaxHp() const { return MaxHp; }
	bool GetisPlayer() const { return isPlayer; }

private:
	Player* opponent;
	Animator* anim;
	Field* field;

	std::string hitPart;

	VECTOR velocity;
	VECTOR basePos;
	VECTOR worldCenter;
	VECTOR attackPos;

	int Hp;
	int MaxHp;
	int damage;
	int colIndex;
	int DrawValue;
	int TargetValue;

	// float time = 0.0f;
	float velocityY = 0.0f;
	float attackRadius;

	bool isJumping;
	bool canReduceHp; // trueÇÃÇ∆Ç´ÇæÇØHPÇå∏ÇÁÇπÇÈ
	bool isPlayer;
	bool isGuarding;
	bool isMoveing;
	bool isPunching;

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
	int hips_Bone;
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
	VECTOR hips_WorldPos;
	VECTOR hipsNow;

	enum State {
		S_STOP,
		S_PUNCH1,
		S_PUNCH2,
		S_PUNCH3,
		S_KICK1,
		S_KICK2,
		S_KICK3,
		S_PROTECT,
		S_JUMP
	};

	State state;
	void UpdateStop();
	void UpdatePunch1();
	void UpdatePunch2();
	void UpdatePunch3();
	void UpdateKick1();
	void UpdateKick2();
	void UpdateKick3();
	void UpdateProtect();
	void UpdateJump();
	void UpdateDamage(int dmg);
	void CollisionDetection();
	void ResolvePlayerCollision();
	void BoneCollision();
};