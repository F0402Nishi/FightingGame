#include "Field.h"
#include <assert.h>

Field::Field()
{
    hModel = MV1LoadModel("data/Map/BackGround/TrainingStage_Background.mv1");
    assert(hModel >= 0);
}

Field::~Field()
{
}

void Field::Update()
{
}

bool Field::SearchGround(VECTOR pos1, VECTOR pos2, VECTOR* hit)
{
    MV1_COLL_RESULT_POLY ret = MV1CollCheck_Line(hModel, -1, pos1, pos2);
    if (ret.HitFlag) {
        if (hit != nullptr) {
            *hit = ret.HitPosition;
        }
        return true;
    }
    return false;
}
