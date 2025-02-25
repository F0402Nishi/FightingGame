#include "Stage.h"
#include <assert.h>

Stage::Stage()
{
	hModel = MV1LoadModel("data/Map/BackGround/TrainingStage_Background.mv1");
	assert(hModel >= 0);
}

Stage::~Stage()
{
}

void Stage::Draw()
{
	Object3D::Draw();
}
