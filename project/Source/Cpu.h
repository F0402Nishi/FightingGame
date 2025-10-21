#pragma once
#include "Character.h"
#include <string>
#include <vector>
// #include "Animator.h"
// #include "Field.h"
// #include "Player.h"
// #include <iostream>  // ← これが必須

class Player;
class Animator;

class CPU : public Character
{
public:
	CPU(bool _iscpu);
	~CPU();
	void Update() override;
	void Draw() override ;
	void EffectiveRange();
	
	
	virtual bool GetisCpu() const override { return isCpu; }
	virtual bool GetisPlayey() const override { return false; }

private:
	Player* player;
	Animator* anim;

	VECTOR mypos; // 自分の位置
	VECTOR playerpos; // Playerの位置
	VECTOR previousStop; // 前回の停止位置
	VECTOR CPUvelocity;

	//  === 乱数 ===
	int r;
	int m;
	int a;
	int sum;
	int attackCount;

	float targetDistance; // 理想の間合い
	float expandThreshold; // ここまで広がったら詰め直す
	float dx; // Playerの位置からCPUの位置を引いた位置(中心値)
	float dist; // プレイヤーと CPU の水平距離
	float playerMoveDir; // 現在のPlayerの座標と前の座標の差分
	float followThreshold; // CPUが離れてから追う動作に切り替えるための基準値
	float time;
	float CPUpos;
	float moved;

	bool opponent;
	bool punch;
	bool isCpu;
	bool reachedTarget;
	bool isFollowing;
	bool NowAnim;
	bool NowDice;
	bool NowMovement;
	bool NowAttack;
	bool Nowpos;
	bool actionFinished;
	bool GuardNow;

	State attackStates[7] = {
		S_PUNCH1, // 弱P
		S_PUNCH2, // 中P
		S_PUNCH3, // 強P
		S_KICK1,  // 弱K
		S_KICK2,  // 中K
		S_KICK3,  // 強K
		S_PROTECT // ガード
	};

	int Close_attack[6] = { 15, 25, 15, 10, 20, 15 };
	int Mid_attack[6] = { 11, 28, 17, 11, 22, 11 };
	int Long_attack[6] = { 5, 30, 30, 5, 20, 10 };

	enum Brain {
		CLOSE_COMBAT, // 近距離
		MID_COMBAT, // 中距離
		LONG_COMBAT // 長距離
	};

	Brain brain;
	void UpdateCloseCombat();
	void UpdateMidCombat();
	void UpdateLongCombat();
	void UpdateDice();
};
