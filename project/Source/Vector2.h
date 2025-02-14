#pragma once

struct VECTOR2 { // ç\ë¢ëÃ
	float x; // ç¿ïW
	float y;
	VECTOR2() { x = 0; y = 0; }
	VECTOR2(float _x, float _y) {
		x = _x; y = _y;
	}
};

float VSize(const VECTOR2& v);

VECTOR2 VSub(const VECTOR2& v1, const VECTOR2& v2);

VECTOR2 VAdd(const VECTOR2& v1, const VECTOR2& v2);

VECTOR2 operator +(const VECTOR2& v1, const VECTOR2& v2);

VECTOR2 operator -(const VECTOR2& v1, const VECTOR2& v2);

const VECTOR2& operator +=(VECTOR2& v1, const VECTOR2& v2);

const VECTOR2& operator -=(VECTOR2& v1, const VECTOR2& v2);

VECTOR2 VScale(const VECTOR2& v, float s);

VECTOR2 operator *(const VECTOR2& v1, float s);

VECTOR2 operator /(const VECTOR2& v1, float s);

const VECTOR2& operator *=(VECTOR2& v1, float s);

const VECTOR2& operator /=(VECTOR2& v1, float s);

VECTOR2 VNorm(const VECTOR2& v);

bool CircleHit(const VECTOR2& obj1, const VECTOR2& obj2, float r);
