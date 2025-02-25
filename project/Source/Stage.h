#pragma once
#include "Object3D.h"

class Stage : public Object3D
{
public:
	Stage();
	~Stage();
	void Draw() override;
};
