#include "StageObj.h"
#include <assert.h>

// Å¶égópó\íËÇ»Çµ

StageObj::StageObj(VECTOR pos, VECTOR rot, VECTOR sca)
{
	char buf[256];
	sprintf_s<256>(buf, "data/Stage/TrainingStage_Background.mv1");
	hModel = MV1LoadModel(buf);
	sprintf_s<256>(buf, "data/Stage/Ground_col.mv1");
	hitModel = MV1LoadModel(buf);
	assert(hitModel > 0);


	transform.position = pos;
	transform.rotation = rot;
	transform.scale = sca;
}

StageObj::~StageObj()
{
}

bool StageObj::SearchObject(VECTOR pos1, VECTOR pos2, VECTOR* hit)
{
	MV1SetMatrix(hModel, MGetIdent());
	MV1RefreshCollInfo(hitModel);
	MV1_COLL_RESULT_POLY ret = MV1CollCheck_Line(hitModel, -1, pos1, pos2);
	if (ret.HitFlag) {
		if (hit != nullptr) {
			*hit = ret.HitPosition;
		}
		return true;
	}
	return false;
}

VECTOR StageObj::CollderObject(VECTOR pos, SphereCollder col)
{
	MV1_COLL_RESULT_POLY_DIM dim = MV1CollCheck_Sphere(hitModel, -1, col.center + pos, col.radius);
	if (dim.HitNum == 0) return pos;
	for (int i = 0; i < dim.HitNum; i++) {
		VECTOR hitPos = dim.Dim[i].HitPosition;
		VECTOR hitNorm = dim.Dim[i].Normal;
	}
	return VECTOR();
}
