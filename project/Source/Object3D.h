#pragma once
#include "../Library/GameObject.h"

class Transform
{
public:
	VECTOR position;
	VECTOR rotation;
	VECTOR scale;
	MATRIX matrix;
};

class SphereCollder
{
public:
	VECTOR center;
	float radius;
	SphereCollder(VECTOR c, float r) {
		center = c;
		radius = r;
	}
};

class EllipseCollder
{
public:
	VECTOR top;
	VECTOR down;
	float radius;
	EllipseCollder(VECTOR t, VECTOR d, float r) {
		top = t;
		down = d;
		radius = r;
	}
};

class Object3D : public GameObject
{
public:
	Object3D();
	virtual ~Object3D();
	virtual void Update() override;
	virtual void Draw() override;
	Transform GetTransform() { return transform; }

protected:
	int hModel;
	int hitModel;
	Transform transform;
	SphereCollder* S_Head_collder;
	EllipseCollder* E_Body_collder;
	EllipseCollder* E_collder;
};
