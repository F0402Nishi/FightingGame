#pragma once
#include "Object3D.h"

class Stage : public Object3D
{
public:
	Stage();
	~Stage();
	void Draw() override;
	bool SearchObject(VECTOR pos1, VECTOR pos2, VECTOR* hit = nullptr);
};
