#include "Object3D.h"

Object3D::Object3D()
{
	hModel = -1;
	hitModel = -1;
	transform.position = VGet(0, 0, 0);
	transform.rotation = VGet(0, 0, 0);
	transform.scale = VGet(1, 1, 1);
	S_Head_collder = nullptr;
	E_Body_collder = nullptr;
	E_collder = nullptr;
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
	MV1SetMatrix(hitModel,transform.matrix);
	MV1DrawModel(hModel);
	MV1DrawModel(hitModel);

	if (S_Head_collder != nullptr) {
		DrawSphere3D(S_Head_collder->center + transform.position, S_Head_collder->radius, 20, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
	}
	if (E_Body_collder != nullptr) {
		DrawCapsule3D(E_Body_collder->top + transform.position, E_Body_collder->down + transform.position, E_Body_collder->radius, 20, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
	}
	if (E_collder != nullptr) {
		DrawCapsule3D(E_collder->top + transform.position, E_collder->down + transform.position, E_collder->radius, 20, GetColor(255, 255, 255), GetColor(255, 0, 0), TRUE);
	}
}
