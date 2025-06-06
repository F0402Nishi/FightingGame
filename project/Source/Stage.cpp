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
	VECTOR nearest; //�����Ƃ��߂��ʒu
	VECTOR tmp; // �ꎞ�I�Ȓl�̕ۑ�
	float nearestDist = VSize(pos1 - pos2); // pos1����pos2�̒���
	Field* f = FindGameObject<Field>();

	if (f->SearchGround(pos1, pos2, &tmp)) { //Field�N���X����true���Ԃ��Ă�����(pos1,pos2,tmp�̐�������������)
		float nowDist = VSize(tmp - pos1); //pos1���瓖�������ʒu(tmp)�̒���
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
