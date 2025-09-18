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
	VECTOR previousStop; // 前回の停止位置

	float targetDistance; // 理想の間合い
	float expandThreshold; // ここまで広がったら詰め直す
	float dx; // Playerの位置からCPUの位置を引いた位置(中心値)
	float dist; // プレイヤーと CPU の水平距離
	float playerMoveDir; // 現在のPlayerの座標と前の座標の差分
	float followThreshold; // CPUが離れてから追う動作に切り替えるための基準値

	bool opponent;
	bool punch;
	bool isCpu;
	bool reachedTarget;
	bool isFollowing;

	enum Brain {
		CLOSE_COMBAT, // 近距離
		MID_COMBAT, // 中距離
		LONG_COMBAT // 長距離
	};

	Brain brain;
	void UpdateCloseCombat();
	void UpdateMidCombat();
	void UpdateLongCombat();
};
