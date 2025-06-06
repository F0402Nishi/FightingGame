#include "Stage.h"
#include <assert.h>
#include "Field.h"

Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::Draw()
{
}

bool Stage::SearchObject(VECTOR pos1, VECTOR pos2, VECTOR* hit)
{
	bool ret = false;
	VECTOR nearest; //もっとも近い位置
	VECTOR tmp; // 一時的な値の保存
	float nearestDist = VSize(pos1 - pos2); // pos1からpos2の長さ
	Field* f = FindGameObject<Field>();

	if (f->SearchGround(pos1, pos2, &tmp)) { //Fieldクラスからtrueが返ってきたら(pos1,pos2,tmpの数字が揃ったら)
		float nowDist = VSize(tmp - pos1); //pos1から当たった位置(tmp)の長さ
		if (nowDist < nearestDist){
			nearestDist = nowDist;
			nearest = tmp;
			ret = true;
		}
	}

	if (ret && hit != nullptr) {
		*hit = nearest;
	}


	return ret;
}
