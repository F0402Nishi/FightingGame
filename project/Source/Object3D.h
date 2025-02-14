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
	Transform transform;
	SphereCollder* collder;
};
