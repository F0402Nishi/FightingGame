#pragma once
#include "Character.h"
#include "Animator.h"
#include "Field.h"
#include <string>
#include <vector>
#include <iostream>  // © ‚±‚ê‚ª•K{
#include "Player.h"

class Opponent : public Character
{
public:
	Opponent();
	~Opponent();
	bool OpponentInit(Player* target);
	void Update() override { std::cout << "Opponent Update" << std::endl; };
	void Draw() override { std::cout << "Character Draw" << std::endl; };

private:
	// Player* player;
	Animator* anim;
	
	bool opponent;
	bool punch;
};
