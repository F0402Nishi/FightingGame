#include "Player.h"
#include <assert.h>
#include "../ImGui/imgui.h"
#include "Stage.h"
#include "HitCheck.h"
#include "Cpu.h"
#include "2DUI.h"

// 入力履歴用
std::vector<int> inputHistory;
const int maxHistory = 10;

Player::Player(bool _isPlayer)
{
	isPlayer = _isPlayer;
	isJumping = false;

	lastKey = NEUTRAL;
	currentKey = NEUTRAL;

	lx = 0;
	ly = 0;

	ui2d = new UI2D();

	if (!isPlayer) {
		transform.position = VGet(200.0f, 14.0f, 150.0f);
		transform.rotation = VGet(0, DegToRad(90.0f), 0);
	}

	// 古い入力を削除
	while (inputHistory.size() > maxHistory) {
		inputHistory.erase(inputHistory.begin());
	}
}

Player::~Player()
{
}

void Player::Update()
{
	Character::Always();
	// PlayerAttack();
	
	// === 入力を取得 ===
	if (isPlayer) { // PLAYER1 = ゲームパッド
	}

	if (!isPlayer) { // !isPlayerの攻撃判定用Colliderの位置を調整
		hitSpheres[4].localOffset = left_HandWorldPos - basePos + VGet(-8.0f, 3.5f, -10.0f);
		hitSpheres[7].localOffset = right_HandWorldPos - basePos + VGet(-5.0f, 7.0f, 0.0f);
	}

	if (!isAlive) return;

	// 攻撃入力
	if (isPlayer) { PlayerAttack(DX_INPUT_PAD1, true, 1); }
	else { PlayerAttack(DX_INPUT_PAD2, true, 2); }

	// 左右移動
	if (VSize(inputDir) > 0) {
		if (VSize(inputDir) >= 1.0f) {
			inputDir = VNorm(inputDir);
		}
		velocity = inputDir * speed;
		transform.position += velocity;
	}

	// === 履歴に追加するのは押した瞬間だけ ===
	if (inputHistory.empty() || currentKey != lastKey) {
		inputHistory.push_back(currentKey);
	}

	// === 次フレーム用に保存 ===
	lastKey = currentKey; 

	// 古い入力を削除
	while (inputHistory.size() > maxHistory) {
		inputHistory.erase(inputHistory.begin());
	}

	if (InputTypeP) {
		ui2d->DrawInputHistory(inputHistory);
	}

#if false "のちに戻す" 


	//ImGui::Begin("PLAYER");
	//ImGui::Checkbox("InputTypeP", &InputTypeP);
	//ImGui::Checkbox("canReduceHp", &canReduceHp);
	//ImGui::Checkbox("isMoving", &isMoveing);
	//ImGui::Checkbox("animRetun", &animRetun);
	//ImGui::InputInt("debugCollisionCount", &debugCollisionCount);
	//ImGui::InputInt("debugframe", &debugframe);
	//ImGui::InputFloat("position.x", &transform.position.x);
	//ImGui::InputFloat("position.y", &transform.position.y);
	//ImGui::InputFloat("position.z", &transform.position.z);
	//ImGui::InputFloat("velocity.x", &velocity.x);
	//ImGui::InputFloat("inputDir.x", &inputDir.x);
	//ImGui::InputFloat("IdleTimer", &idleTimer);
	//ImGui::InputFloat("dist", &dist);
	//ImGui::InputFloat("frame", &frame);
	//ImGui::InputFloat("totalframe", &total);
	//ImGui::InputFloat("ratioframe", &ratio);
	//ImGui::Text("state: %d", (int)state);
	//ImGui::Text("push.x: %.2f", hit.x);
	//ImGui::Text("push.y: %.2f", hit.y);
	//ImGui::Text("Position: z=%.2f", startPos.z);
	//ImGui::Text("HP: %d", (int)Hp);
	//ImGui::End();

	//ImGui::Begin("PAD DEBUG");
	//ImGui::Text("joyInput: %d", joyInput);
	//ImGui::Text("lx: %d, ly: %d", lx, ly);
	//ImGui::End();

	if (CheckHitKey(KEY_INPUT_SPACE)) {
		velocityY = PLAYER_JUMP;
		transform.position.y += velocityY;
		state = S_JUMP;
	}

	// 途中キャンセルを試してみた

	if (state == S_PUNCH1 && canCancel) {
		if (CheckHitKey(KEY_INPUT_I)) { state = S_PUNCH2; canReduceHp = true; isMoveing = true; }
		if (CheckHitKey(KEY_INPUT_O)) { state = S_PUNCH3; canReduceHp = true; isMoveing = true; }
	}
	if (state == S_PUNCH2 && canCancel) {
		if (CheckHitKey(KEY_INPUT_O)) { state = S_PUNCH3; canReduceHp = true; isMoveing = true; }
		if (CheckHitKey(KEY_INPUT_U)) { state = S_PUNCH1; canReduceHp = true; isMoveing = true; }
	}
	if (state == S_PUNCH3 && canCancel) {
		if (CheckHitKey(KEY_INPUT_U)) { state = S_PUNCH1; canReduceHp = true; isMoveing = true; }
		if (CheckHitKey(KEY_INPUT_I)) { state = S_PUNCH2; canReduceHp = true; isMoveing = true; }
	}
#endif
}

void Player::Draw()
{
	Character::Draw();

}

void Player::PlayerAttack(int padIndex, bool useKeyboardFallback, int playerNum)
{
	// KEYが押しっぱなしの時に、canReduceHpがtrueとfalseを繰り返している
	// そのため、ダメージが重複している。かつ、アニメーションは１回しか再生されておらず分かりにくい
	// KEY_INPUTの際に、bool型の変数を追加して条件を「KEY_INPUT＆false」の時に変更
	// アニメーションが終わったときに、trueにする。

	// キャンセル用の関数または変数を定義
	// 例.Uのアニメーション中にIを押されたら、、、

	if (GetJoypadNum() >= padIndex) {
		GetJoypadXInputState(padIndex, &input);
		GetJoypadAnalogInput(&lx, &ly, padIndex);

		if (state == S_STOP && !isHitPlaying) {
			if (lx < -200 || input.Buttons[XINPUT_BUTTON_DPAD_LEFT]) {
				inputDir.x = -10.0f;
				currentKey = KEY_INPUT_A;
				// anim->Play("data/Character/Player/Walk_B.mv1", true);
			}
			else if (lx > 200 || input.Buttons[XINPUT_BUTTON_DPAD_RIGHT]) {
				inputDir.x = 10.0f;
				currentKey = KEY_INPUT_D;
				// anim->Play("data/Character/Player/Walk_F.mv1", true);
			}
			else {
				anim->Play("data/Character/Player/Fight_Idle.mv1", true);
				currentKey = NEUTRAL;
			}
		}

		if (input.Buttons[XINPUT_BUTTON_B] && !isMoveing) {
			state = S_PUNCH1; canReduceHp = true; isMoveing = true; //animRetun = true;
			currentKey = KEY_INPUT_U;
		}
		if (input.Buttons[XINPUT_BUTTON_RIGHT_SHOULDER] && !isMoveing) {
			state = S_PUNCH2; canReduceHp = true; isMoveing = true; //animRetun = true;
			currentKey = KEY_INPUT_I;
		}
		if (input.RightTrigger > 50 && !isMoveing) {
			state = S_PUNCH3; canReduceHp = true; isMoveing = true; //animRetun = true;
			currentKey = KEY_INPUT_O;
		}
		if (input.Buttons[XINPUT_BUTTON_A] && !isMoveing) {
			state = S_KICK1; canReduceHp = true; isMoveing = true; //animRetun = true;
			currentKey = KEY_INPUT_J;
		}
		if (input.Buttons[XINPUT_BUTTON_LEFT_SHOULDER] && !isMoveing) {
			state = S_KICK2; canReduceHp = true; isMoveing = true; //animRetun = true;
			currentKey = KEY_INPUT_K;
		}
		if (input.LeftTrigger > 50 && !isMoveing) {
			state = S_KICK3; canReduceHp = true; isMoveing = true; //animRetun = true;
			currentKey = KEY_INPUT_L;
		}
		if (input.Buttons[XINPUT_BUTTON_X]) {
			state = S_PROTECT; isGuarding = true;
			currentKey = KEY_INPUT_H;
		}
		else if (!input.Buttons[XINPUT_BUTTON_X]) {
			InReturn();
		}

		return;
	}

	if (useKeyboardFallback) {
		if (state == S_STOP && !isHitPlaying) {
			if (CheckHitKey(KEY_INPUT_A)) { // 後ろ歩き
				inputDir.x = -10.0f;
				currentKey = KEY_INPUT_A;
				// anim->Play("data/Character/Player/Walk_B.mv1", true);
			}
			else if (CheckHitKey(KEY_INPUT_D)) { // 前歩き
				inputDir.x = 10.0f;
				currentKey = KEY_INPUT_D;
				// anim->Play("data/Character/Player/Walk_F.mv1", true);
			}
			else {
				anim->Play("data/Character/Player/Fight_Idle.mv1", true);
				currentKey = NEUTRAL;
			}
		}

		if (CheckHitKey(KEY_INPUT_U) && !isMoveing) { // パンチ1
			state = S_PUNCH1; canReduceHp = true; isMoveing = true; //animRetun = true;
			currentKey = KEY_INPUT_U;
		}
		if (CheckHitKey(KEY_INPUT_I) && !isMoveing) { // パンチ2
			state = S_PUNCH2; canReduceHp = true; isMoveing = true; //animRetun = true;
			currentKey = KEY_INPUT_I;
		}
		if (CheckHitKey(KEY_INPUT_O) && !isMoveing) { // パンチ3
			state = S_PUNCH3; canReduceHp = true; isMoveing = true; //animRetun = true;
			currentKey = KEY_INPUT_O;
		}
		if (CheckHitKey(KEY_INPUT_J) && !isMoveing) { // キック1
			state = S_KICK1; canReduceHp = true; isMoveing = true; //animRetun = true;
			currentKey = KEY_INPUT_J;
		}
		if (CheckHitKey(KEY_INPUT_K) && !isMoveing) {  // キック2
			state = S_KICK2; canReduceHp = true; isMoveing = true; //animRetun = true;
			currentKey = KEY_INPUT_K;
		}
		if (CheckHitKey(KEY_INPUT_L) && !isMoveing) { // キック3
			state = S_KICK3; canReduceHp = true; isMoveing = true; //animRetun = true;
			currentKey = KEY_INPUT_L;
		}

		if (CheckHitKey(KEY_INPUT_H)) {  // ガード
			state = S_PROTECT; isGuarding = true;
			currentKey = KEY_INPUT_H;
		}
		else if (!CheckHitKey(KEY_INPUT_H)) {
			InReturn();
		}
	}

}
