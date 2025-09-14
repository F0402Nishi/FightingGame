#include "Cpu.h"
#include "Player.h"
#include "../ImGui/imgui.h"

CPU::CPU(bool _iscpu)
{
	isCpu = _iscpu;
	dx = 0.0f;

	transform.position = VGet(200.0f, 14.0f, 150.0f);
	transform.rotation = VGet(0, DegToRad(90.0f), 0);

	player = FindGameObject<Player>();
}

CPU::~CPU()
{
}

void CPU::Update()
{	
	std::cout << "CPU::Update called" << std::endl;

	Character::Always();

	hitSpheres[4].localOffset = left_HandWorldPos - basePos + VGet(-8.0f, 3.5f, -10.0f);
	hitSpheres[7].localOffset = right_HandWorldPos - basePos + VGet(-5.0f, 7.0f, 0.0f);

	// if (!opponent) return;
	if (!isCpu) anim->Play("data/Character/Player/Fight_Idle.mv1", true);
	if (!isCpu) return;

	EffectiveRange();
}

void CPU::Draw()
{
	std::cout << "CPU Draw" << std::endl;

	Character::Draw();
}

void CPU::EffectiveRange()
{
	mypos = transform.position;
	playerpos = player->GetTransform().position;

	dx = playerpos.x - mypos.x;
	float dist = fabsf(dx);

	const float minDistance = 1.0f; // ‚±‚êˆÈã‹ß‚Ã‚©‚È‚¢
	const float maxDistance = 10.0f; // ‚±‚Ì‹——£‚æ‚è—£‚ê‚Ä‚½‚ç‹ß‚Ã‚­

	if (dist > maxDistance) {
		// ‘ŠŽè‚Ì•û‚ÖˆÚ“®
		mypos.x += (dx > 0) ? speed : -speed;
	}
	else if (dist < minDistance) {
		// ‘ŠŽè‚©‚ç—£‚ê‚é
		mypos.x += (dx > 0) ? -speed : speed;
	}
	
	transform.position = mypos;

	ImGui::Begin("CPU-pos");
	ImGui::InputFloat("dx", &dx);
	//ImGui::InputInt("Type", &opponentType);
	ImGui::End();
}
