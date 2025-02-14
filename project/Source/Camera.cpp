#include "Camera.h"
#include "Player.h"

static const float XRotMax = DegToRad(85.0f);
static const float XRotMin = DegToRad(-45.0f);

Camera::Camera()
{
	transform.rotation.x = DegToRad(30.0f);
}

Camera::~Camera()
{
}

void Camera::Update()
{
	if (CheckHitKey(KEY_INPUT_L)) {
		swing = VGet(0, 10.0f, 0);
	}
	else {
		swing = VGet(0, 0, 0);
	}

	Player* p = FindGameObject<Player>();
	VECTOR pPos = p->GetTransform().position;
	VECTOR pRot = p->GetTransform().rotation;
	VECTOR position = VGet(0, 300, -300);
	VECTOR target = VGet(0, 100, 500);

	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		transform.rotation.y += DegToRad(3.0f);
	}
	if (CheckHitKey(KEY_INPUT_LEFT)) {
		transform.rotation.y -= DegToRad(3.0f);
	}
	if (CheckHitKey(KEY_INPUT_UP)) {
		transform.rotation.x += DegToRad(3.0f);
		if (transform.rotation.x >= XRotMax) {
			transform.rotation.x = XRotMax;
		}
	}
	if (CheckHitKey(KEY_INPUT_DOWN)) {
		transform.rotation.x -= DegToRad(3.0f);
		if (transform.rotation.x <= XRotMin) {
			transform.rotation.x = XRotMin;
		}
	}

	VECTOR offset = VGet(0, 0, -500);
	offset = offset * MGetRotX(transform.rotation.x);
	offset = offset * MGetRotY(transform.rotation.y);
	position = offset + pPos;
	target = VGet(0, 150.0f, 0) + pPos;

#if 0
	// ƒJƒƒ‰‚ÌˆÊ’u
	VECTOR position = VGet(0, 300.0f, -500.0f) * MGetRotY(transform.rotation.y) + transform.position;
	VECTOR target = VGet(0, 200.0f, 500.0f) * MGetRotY(transform.rotation.y) + transform.position;

	if (CheckHitKey(KEY_INPUT_U)) {
		position = VGet(0, 1500.0f, -300.0f) * MGetRotY(transform.rotation.y) + transform.position;
	}
#endif // 0

	forward = VGet(0, 0, 1) * MGetRotY(transform.rotation.y);
	SetCameraPositionAndTarget_UpVecY(position, target);
}

VECTOR Camera::Forward()
{
	return forward;
}
