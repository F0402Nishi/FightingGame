#pragma once
#include "Object3D.h"

class Camera : public Object3D
{
public:
	Camera();
	~Camera();
	void Update() override;

	VECTOR Forward(); //カメラの前方を指すベクトル1を返す

private:
	VECTOR forward;
	VECTOR swing;
};
