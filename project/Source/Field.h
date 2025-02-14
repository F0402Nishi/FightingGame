#pragma once
#include "Object3D.h"

class Field : public Object3D
{
public:
	Field();
	~Field();
	void Update() override;

	/// <summary>
	/// 地面とRayの当たり判定をする
	/// </summary>
	/// <param name="pos1">Rayの始点</param>
	/// <param name="pos2">Rayの終点</param>
	/// <param name="hit">当たったら座標を返す</param>
	/// <returns>当たっていればtrue</returns>
	bool SearchGround(VECTOR pos1, VECTOR pos2, VECTOR* hit = nullptr);
};
