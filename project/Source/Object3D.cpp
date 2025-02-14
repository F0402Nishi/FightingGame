#include "Object3D.h"

Object3D::Object3D()
{
	hModel = -1;
	transform.position = VGet(0, 0, 0);
	transform.rotation = VGet(0, 0, 0);
	transform.scale = VGet(1, 1, 1);
	collder = nullptr;
}

Object3D::~Object3D()
{
}

void Object3D::Update()
{
}

void Object3D::Draw()
{
	// MV1SetPosition(hModel, transform.position);
	// MV1SetRotationXYZ(hModel, transform.rotation);

	MATRIX scale = MGetScale(transform.scale);
	MATRIX rotX = MGetRotY(transform.rotation.x);
	MATRIX rotY = MGetRotY(transform.rotation.y);
	MATRIX rotZ = MGetRotY(transform.rotation.z);
	MATRIX trans = MGetTranslate(transform.position);
	transform.matrix = scale * rotZ * rotY * rotX * trans;

	MV1SetMatrix(hModel,transform.matrix);
	MV1DrawModel(hModel);

	if (collder != nullptr) {
		DrawSphere3D(collder->center + transform.position, collder->radius, 20, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
	}
}
