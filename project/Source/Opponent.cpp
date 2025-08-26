#include "Opponent.h"
#include "Player.h"

Opponent::Opponent()
{
	opponent = player->GetisPlayer();
	punch = player->GetisPunching();
}

Opponent::~Opponent()
{
}

void Opponent::Update()
{
	// 待機アニメーションと移動アニメーションが読み込み順でバグるので、待機アニメーションを優先
	if (opponent) { anim->Play("data/Character/Player/Fight_Idle.mv1", true); }

	if (punch) { anim->Play("data/Character/Player/Hit_A.mv1", false); }
}

void Opponent::Draw()
{
}
