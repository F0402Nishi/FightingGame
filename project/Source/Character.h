#pragma once
#include "Object3D.h"
#include "Animator.h"
#include "Field.h"
#include <string>
#include <vector>
#include <iostream>

class UI2D;

enum class AttackType {
	Punch,
	Kick
};

class Character : public Object3D
{
public:
	Character();
	~Character();
	void Always();
	void SetHitSpheres();
	void SetOpponent(Character* other);
	void SetWinner(bool val, bool ali);
	
	virtual void Update() override = 0;
	virtual void Draw() override;
	virtual bool GetisPlayey() const = 0;
	virtual bool GetisCpu() const = 0;

	std::vector<SphereCollder> hitSpheres;
	std::string GetHit() { return hitPart; }
	int GetHp() const { return Hp; }
	int GetMaxHp() const { return MaxHp; }
	bool GetWinner() const { return isWinner; }
	// bool GetisPunching() const { return isPunching; }
	// bool GetisPlayer() const { return isPlayer; }

protected:
	Character* opponent;
	Animator* anim;
	Field* field;

	std::string hitPart;

	AttackType attacktype;

	VECTOR inputDir; // 初期位置
	// VECTOR velocity;
	VECTOR basePos; // 現在位置
	VECTOR worldCenter;  // Colliderの現在位置
	VECTOR attackPos; // 攻撃の時に当たったColliderの位置

	int Hp; // 現在のHP
	int MaxHp; // 最大値のHP
	int damage; // 攻撃のダメージ
	int colIndex; // 攻撃時に判定に使うCollider
	// int DrawValue;
	// int TargetValue;
	int speed; // 移動の速さ

	// float time = 0.0f;
	float velocityY = 0.0f; // ジャンプ時のY座標
	float attackRadius; // 攻撃時に当たったColliderの半径
	float frame; // 今のフレーム取得
	float total; // 総フレーム数
	float ratio; // 0～1 の割合
	float deltaTime; // 前フレームから現在フレームまでの経過時間
	float idleTimer;

	bool isJumping; //ジャンプ中かの判定
	bool canReduceHp; // trueのときだけHPを減らせる
	// bool isPlayer;
	bool isMoveing; // アニメーションが再生中かの判定
	bool isPunching; // パンチ中かの判定
	bool canCancel; // 攻撃キャンセル可能かの判定中
	bool isGuarding; // ガード中かの判定
	bool isAltIdle;
	bool isHitPlaying;
	bool isAlive; // HP > 0 の時だけ true
	bool isWinner;

	// Playerの骨制御
	int headBone;
	int bodyBone;
	int left_UpperArmBone;
	int left_LowerArmBone;
	int left_HandBone;
	int right_UpperArmBone;
	int right_LowerArmBone;
	int right_HandBone;
	int left_UpperLegBone;
	int left_LowerLegBone;
	int left_FootBone;
	int right_UpperLegBone;
	int right_LowerLegBone;
	int right_FootBone;
	int hips_Bone;
	VECTOR heardWorldPos;
	VECTOR bodyWorldPos;
	VECTOR left_UpperArmWorldPos;
	VECTOR left_LowerArmWorldPos;
	VECTOR left_HandWorldPos;
	VECTOR right_UpperArmWorldPos;
	VECTOR right_LowerArmWorldPos;
	VECTOR right_HandWorldPos;
	VECTOR left_UpperLegWorldPos;
	VECTOR left_LowerLegWorldPos;
	VECTOR left_FootWorldPos;
	VECTOR right_UpperLegWorldPos;
	VECTOR right_LowerLegWorldPos;
	VECTOR right_FootWorldPos;
	VECTOR hips_WorldPos;
	VECTOR hipsNow;

	enum State {
		S_STOP,
		S_PUNCH1,
		S_PUNCH2,
		S_PUNCH3,
		S_KICK1,
		S_KICK2,
		S_KICK3,
		S_PROTECT,
		S_JUMP
	};

	State state;
	void UpdateStop(float deltaTime);
	void UpdatePunch1();
	void UpdatePunch2();
	void UpdatePunch3();
	void UpdateKick1();
	void UpdateKick2();
	void UpdateKick3();
	void UpdateProtect();
	void PlayAttack(const std::string& animFile, bool loop);
	void InReturn();
	void UpdateJump();
	void UpdateDamage(int dmg, AttackType type);
	void CollisionDetection();
	void ResolvePlayerCollision();
	void BoneCollision();
};
