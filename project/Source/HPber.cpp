#include "HPber.h"
#include <DxLib.h>
#include <assert.h>

#define IMAGE_SCALE 0.3f
#define IMAGE_POSTION_X 270.0f
#define IMAGE_POSTION_Y 450.0f

HPber::HPber()
{
    HPImage = LoadGraph("data/2D/HpImage.png");
    assert(HPImage >= 0);
}

bool HPber::Init(const char* filename)
{
    return false;
}

void HPber::SetHp(int hp)
{
}

void HPber::SetMaxHp(int hp)
{
}

void HPber::Draw()
{
    VECTOR cameraPos = GetCameraPosition();

    VECTOR leftHpBarPos = VAdd(cameraPos, VGet(0.0f, 0.0f, 0.0f));   // ç∂è„
    VECTOR rightHpBarPos = VAdd(cameraPos, VGet(0.0f, 0.0f, 0.0f));   // âEè„

    DrawRotaGraph3D(leftHpBarPos.x, IMAGE_POSTION_Y, 0.0f, IMAGE_SCALE, 0.0f, HPImage, TRUE); // ç∂
    DrawRotaGraph3D(-rightHpBarPos.x, IMAGE_POSTION_Y, 0.0f, IMAGE_SCALE, 0.0f, HPImage, TRUE, TRUE); // âE
}
