#pragma once
#include "Object3D.h"
#include "Animator.h"
#include "Field.h"
#include <string>
#include <vector>

enum PlayerType {
	PLAYER1,
	PLAYER2
};

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
	void InitHitSpheres(PlayerType type);
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