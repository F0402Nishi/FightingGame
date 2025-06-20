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

class EllipseCollider
{
public:
	// VECTOR top;
	// VECTOR down;
	VECTOR left;
	VECTOR right;
	float radius;
	EllipseCollider(VECTOR l, VECTOR r, float ra) {
		left = l;
		right = r;
		radius = ra;
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
	EllipseCollider* GetCollder() { return E_collder; }
	
protected:
	int hModel;
	int hitModel;
	Transform transform;
	SphereCollder* S_Head_collder;
	EllipseCollider* E_collder;
};
