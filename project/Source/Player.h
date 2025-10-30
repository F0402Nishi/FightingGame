#pragma once
#include "Character.h"
#include "Animator.h"
#include <string>
#include <vector>
// #include "Field.h"

class UI2D;
class CPU;

constexpr int NEUTRAL = -1; // 待機モーション

class Player : public Character
{
public:
	Player(bool _isPlayer);
	~Player();

	void Update() override;
	void Draw() override;
	virtual bool GetisPlayey() const override { return isPlayer; }
	virtual bool GetisCpu() const override { return false; }

private:
	CPU* cpu;
	UI2D* ui2d;

	VECTOR velocity;
	VECTOR stick; // 左スティックX軸取得用

	XINPUT_STATE input;

	int joyInput;
	int lastKey;
	int currentKey;
	int padNo; // 使用パッド番号
	int lx;
	int ly;
	
	bool isPlayer;
};