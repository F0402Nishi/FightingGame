#include "HPber.h"
#include <DxLib.h>
#include <assert.h>

#define IMAGE_SCALE 0.4f
#define IMAGE_POSTION_X 320.0f
#define IMAGE_POSTION_Y 50.0f

static float posCx = 000.0f;
static float posCy = 200.0f;

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
    DrawRotaGraph(IMAGE_POSTION_X * 3, IMAGE_POSTION_Y, IMAGE_SCALE, 0.0f, HPImage, TRUE); // âE
    DrawRotaGraph(IMAGE_POSTION_X, IMAGE_POSTION_Y, IMAGE_SCALE, 0.0f, HPImage, TRUE, TRUE); // ç∂
}
