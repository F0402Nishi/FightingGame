#pragma once
#include "Object3D.h"
#include "Animator.h"
#include "Field.h"

class Player : public Object3D
{
public:
	Player();
	~Player();
	void Update() override;
	void Draw() override;

private:
	Animator* anim;
	Field* field;
	bool isJumping = false;
	float velocityY = 0.0f;
	const float gravity = -0.4f;
	const float jumpPower = 8.0f;
};
