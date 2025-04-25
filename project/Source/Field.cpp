#include "Field.h"
#include <assert.h>
#include "../ImGui/imgui.h"

Field::Field()
{
    hModel = MV1LoadModel("data/Stage/TrainingStage_Background.mv1");
    assert(hModel >= 0);
    hitModel = MV1LoadModel("data/Stage/Ground_col.mv1");
    assert(hitModel >= 0);

    MV1SetupCollInfo(hitModel, 0, 12, 12, 12);
}

Field::~Field()
{
    MV1DeleteModel(hModel);
}

void Field::Update()
{
}

bool Field::SearchGround(VECTOR pos1, VECTOR pos2, VECTOR* hit)
{
    transform.matrix = MGetTranslate(transform.position);
    MV1SetMatrix(hitModel, MGetIdent());

    // デバッグ表示：当たり判定モデルを描画
    MV1DrawModel(hitModel);

    MV1_COLL_RESULT_POLY ret = MV1CollCheck_Line(hitModel, -1, pos1, pos2);
    DrawLine3D(pos1, pos2, GetColor(255, 0, 0));
    
    ImGui::Begin("Field");
    ImGui::InputInt("HitFlag", &ret.HitFlag);
    ImGui::InputFloat("position.x", &transform.position.x);
    ImGui::InputFloat("position.y", &transform.position.y);
    ImGui::End();
    
    if (ret.HitFlag) {
        if (hit != nullptr) {
            *hit = ret.HitPosition;
        }
        return true;
    }
    return false;
}
