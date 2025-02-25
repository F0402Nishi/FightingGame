#pragma once
#include "Object3D.h"
#include "Animator.h"

class Player : public Object3D
{
public:
	Player();
	~Player();
	void Update() override;
	void Draw() override;

private:
	Animator* anim;
};
