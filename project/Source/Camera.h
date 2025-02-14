#pragma once
#include "Object3D.h"

class Camera : public Object3D
{
public:
	Camera();
	~Camera();
	void Update() override;

	VECTOR Forward(); //�J�����̑O�����w���x�N�g��1��Ԃ�

private:
	VECTOR forward;
	VECTOR swing;
};
