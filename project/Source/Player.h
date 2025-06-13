#pragma once
#include "Object3D.h"
#include "Animator.h"
#include "Field.h"

class Player : public Object3D
{
public:
	Player(bool _isPlayer);
	~Player();
	void Update() override;
	void Draw() override;
	void SetOpponent(Player* other);

private:
	Animator* anim;
	Field* field;
	bool isJumping = false;
	VECTOR velocity;
	float velocityY = 0.0f;
	float time = 0.0f;
	bool isPlayer;
	int damage;
	int Hp = 1000;

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
	void UpdateDamage(int dmg);

	Player* opponent;
};
