#include "Object3D.h"

Object3D::Object3D()
{
	hModel = -1;
	hitModel = -1;
	transform.position = VGet(0, 0, 0);
	transform.rotation = VGet(0, 0, 0);
	transform.scale = VGet(1, 1, 1);
	E_collder = nullptr;
	// S_headcollider = nullptr;
	// S_bodycollider = nullptr;
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

#if 0
	if (S_headcollider != nullptr) {
		DrawSphere3D(S_headcollider->localOffset + transform.position, S_headcollider->radius, 20, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
	}
	if (S_bodycollider != nullptr) {
		DrawSphere3D(S_bodycollider->localOffset + transform.position, S_bodycollider->radius, 20, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
	}
#endif // 0

	if (E_collder != nullptr) {
		EllipseCollider(transform.position, transform.position, TRUE);
		// DrawCapsule3D(E_collder->left + transform.position, E_collder->right + transform.position, E_collder->radius, 20, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
	}
}
