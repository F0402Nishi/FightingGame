#include "PlayerObj.h"
#include "Player.h"

PlayerObj::PlayerObj()
{
	E_collder = nullptr;
}

PlayerObj::~PlayerObj()
{
}

void PlayerObj::Update()
{
}

void PlayerObj::Draw()
{
	if (E_collder != nullptr) {
		// EllipseCollider(transform.position, transform.position, TRUE);
		// DrawCapsule3D(E_collder->left + transform.position, E_collder->right + transform.position, E_collder->radius, 20, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
	}
}

void PlayerObj::ResolvePlayerCollision(Player* self, Player* opponent)
{
	if (!self || !opponent) return;

	// VECTOR selfPos = self->GetPosition();
}
