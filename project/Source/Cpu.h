#pragma once
#include "Character.h"
#include <string>
#include <vector>
// #include "Animator.h"
// #include "Field.h"
// #include <iostream>  // ← これが必須
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

	VECTOR mypos; // 自分の位置
	VECTOR playerpos; // Playerの位置

	float targetDistance; // 理想の間合い
	float expandThreshold; // ここまで広がったら詰め直す
	float dx; // Playerの位置からCPUの位置を引いた位置(中心値)
	float playerMoveDir; // 現在のPlayerの座標と前の座標の差分

	bool opponent;
	bool punch;
	bool isCpu;
	bool reachedTarget;
};
