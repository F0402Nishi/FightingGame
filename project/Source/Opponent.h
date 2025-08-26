#pragma once
#include "Object3D.h"
#include "Animator.h"
#include "Field.h"
#include "Player.h"
#include <string>
#include <vector>

class Opponent : public Object3D
{
public:
	Opponent();
	~Opponent();
	void Update() override;
	void Draw() override;

private:
	Player* player;
	Animator* anim;
	
	bool opponent;
	bool punch;
};
