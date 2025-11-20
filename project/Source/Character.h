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

struct AttackData {
	int hitStartFrame; // 当たり判定が「出る」最初のフレーム
	int hitEndFrame; // 当たり判定が「消える」最後のフレーム
	int cancelStartFrame; // 別の技へ“キャンセルしてよい”最初のフレーム
	int cancelEndFrame; // キャンセル“してよい”最後のフレーム
};

static const AttackData Punch1Data = { 7, 10, 0, 6 }; // 15
static const AttackData Punch2Data = { 7, 11, 0, 6 }; // 19
static const AttackData Punch3Data = { 12, 17, 0, 11 }; // 22
static const AttackData Kick1Data = { 15, 20, 0, 14 }; // 27
static const AttackData Kick2Data = { 15, 20, 0, 14 }; // 27
static const AttackData Kick3Data = { 18, 23, 0, 17 }; // 44

class Character : public Object3D
{
public:
	Character();
	~Character();
	void Always();
	void SetHitSpheres();
	void SetOpponent(Character* other);
	void SetAlive(bool ali);
	void SetHp(int value) { Hp = value; }
	void SetInputDisplay(bool _display) { InputTypeP = _display; }

	
	virtual void Update() override = 0;
	virtual void Draw() override;
	virtual bool GetisPlayey() const = 0;
	virtual bool GetisCpu() const = 0;

	std::vector<SphereCollder> hitSpheres;
	std::string GetHit() { return hitPart; }

	VECTOR ApplyAttackMotion(const AttackData& data, float frame, char moveAxis);
	
	int GetHp() const { return Hp; }
	int GetMaxHp() const { return MaxHp; }
	
	bool IsGuardOn() const { return GuardOn; }
	// bool GetisPunching() const { return isPunching; }
	// bool GetisPlayer() const { return isPlayer; }

protected:
	Character* opponent;
	Animator* anim;
	Field* field;

	std::string hitPart;

	AttackType attacktype;

	VECTOR inputDir; // 初期位置
	VECTOR velocity;
	VECTOR basePos; // 現在位置
	VECTOR worldCenter;  // Colliderの現在位置
	VECTOR attackPos; // 攻撃の時に当たったColliderの位置
	VECTOR myPos;
	VECTOR opPos;
	VECTOR direction;
	VECTOR startPos;
	VECTOR baseOffset;

	int Hp; // 現在のHP
	int MaxHp; // 最大値のHP
	int damage; // 攻撃のダメージ
	int colIndex; // 攻撃時に判定に使うCollider
	// int DrawValue;
	// int TargetValue;
	int speed; // 移動の速さ
	int debugframe;

	// float time = 0.0f;
	float velocityY = 0.0f; // ジャンプ時のY座標
	float attackRadius; // 攻撃時に当たったColliderの半径
	float frame; // 今のフレーム取得
	float total; // 総フレーム数
	float ratio; // 0～1 の割合
	float deltaTime; // 前フレームから現在フレームまでの経過時間
	float idleTimer;
	float framespeed; // 戻る速さ（調整可能）
	float xOffset;
	float dist;
	float correctionRange;
	float waitTimer;
	float FrameCounter;

	bool isJumping; //ジャンプ中かの判定
	bool canReduceHp; // trueのときだけHPを減らせる
	// bool isPlayer;
	bool isMoveing; // アニメーションが再生中かの判定
	// bool isPunching; // パンチ中かの判定
	bool canCancel; // 攻撃キャンセル可能かの判定中
	bool isGuarding; // ガード中かの判定
	bool isAltIdle;
	bool isHitPlaying;
	bool isAlive; // HP > 0 の時だけ true
	bool InputTypeP;
	bool GuardOn;
	bool startPosSaved;
	bool isActing;
	bool animRetun;
	bool hasHit = false;
	bool waitForNextAction; // 攻撃直後の一時停止フラグ
	bool actionFinished;
	bool isAttacking;


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
