#pragma once
#include "Character.h"
#include <string>
#include <vector>
// #include "Animator.h"
// #include "Field.h"
// #include <iostream>  // © ‚±‚ê‚ª•K{
// #include "Player.h"

class Player;

class CPU : public Character
{
public:
	CPU(bool _iscpu);
	~CPU();
	void Update() override;
	void Draw() override ;
	void EffectiveRange();
	
	
	virtual bool GetisPlayey() const override { return false; }
	virtual bool GetisCpu() const override { return true; }

private:
	Player* player;
	Animator* anim;

	VECTOR mypos; // ©•ª‚ÌˆÊ’u
	VECTOR playerpos; // Player‚ÌˆÊ’u

	float dx;

	bool opponent;
	bool punch;
	bool isCpu;
};
