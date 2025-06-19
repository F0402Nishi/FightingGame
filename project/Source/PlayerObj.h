#pragma once
#include "../Library/GameObject.h"
#include "Object3D.h"
#include "Player.h"

class EllipseCollider
{
public:
	// VECTOR top;
	// VECTOR down;
	VECTOR left;
	VECTOR right;
	float radius;
	EllipseCollider(VECTOR l, VECTOR r, float ra) {
		// top = t;
		// down = d;
		left = l;
		right = r;
		radius = ra;
	}
};

class PlayerObj : public GameObject
{
public:
	PlayerObj();
	~PlayerObj();
	void Update() override;
	void Draw() override;
	void ResolvePlayerCollision(Player* self, Player* opponent);


protected:
	Player* player;
	EllipseCollider* E_collder;
};
