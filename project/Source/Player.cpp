#include "Player.h"
#include <assert.h>

Player::Player()
{
	hModel = MV1LoadModel("data/Character/Player/untitled.mv1");
	assert(hModel >= 0);
}

Player::~Player()
{
}

void Player::Update()
{
}

void Player::Draw()
{
	Object3D::Draw();
}
