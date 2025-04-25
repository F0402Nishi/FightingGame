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
	VECTOR nearest;
	VECTOR tmp;
	float nearestDist = VSize(pos1 - pos2);
	Field* f = FindGameObject<Field>();

	if (f->SearchGround(pos1, pos2, &tmp)) {
		float nowDist = VSize(tmp - pos1);
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
