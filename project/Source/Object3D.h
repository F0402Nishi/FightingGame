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
	VECTOR localOffset;
	float radius;
	std::string partName;
	SphereCollder(VECTOR offset, float r, const std::string& name) {
		localOffset = offset;
		radius = r;
		partName = name;
	}

	VECTOR GetWorldCenter(const VECTOR& playerPos) const { return playerPos + localOffset; }
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
	EllipseCollider* E_collder;
	SphereCollder* S_headcollider;
	SphereCollder* S_bodycollider;
};
