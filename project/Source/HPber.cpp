#include "HPber.h"
#include <DxLib.h>
#include <assert.h>

bool HPber::Init(const char* filename)
{
    HPImage = LoadGraph("data/2D/HpImage.png");
    assert(HPImage >= 0);

    // HPbarHeight = GetGraphHeight(HPImage) / HPbarCount;

    return false;
}

void HPber::SetHp(int hp)
{
}

void HPber::SetMaxHp(int hp)
{
}

void HPber::Draw(int x, int y)
{
}
