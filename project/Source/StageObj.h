#pragma once
#include "Object3D.h"

class StageObj : public Object3D
{
public:
	StageObj(int kind, VECTOR pos, VECTOR rot, VECTOR sca);
	~StageObj();
	bool SearchObject(VECTOR pos1, VECTOR pos2, VECTOR* hit = nullptr);
	VECTOR CollderObject(VECTOR pos, SphereCollder col);

private:
	int hitModel;
};
