#pragma once
#include "Object3D.h"

class Field : public Object3D
{
public:
	Field();
	~Field();
	void Update() override;
	bool SearchGround(VECTOR pos1, VECTOR pos2, VECTOR* hit = nullptr);
};
