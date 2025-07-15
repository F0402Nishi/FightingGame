#define NOMINMAX
#include <Windows.h>
#include "HPber.h"
#include "Player.h"
#include <DxLib.h>
#include <assert.h>
#include <algorithm>

#define IMAGE_SCALE 0.4f
#define IMAGE_POSITION_X 320.0f
#define IMAGE_POSITION_Y 50.0f

HPber::HPber()
{
    HPImage = LoadGraph("data/2D/HpImage.png");
    assert(HPImage >= 0);

    player = nullptr;
    isLeftPlayer = true;

    if (player == nullptr || HPImage == -1) return;

    currenthp = 0;
    maxhp = 0;
}

bool HPber::Init(Player* target, bool isLeft)
{
    player = target;
    isLeftPlayer = isLeft;

    if (player) {
        currenthp = player->GetHp();
        maxhp = player->GetMaxHp();
        return true;
    }
    else { return false; }
}

void HPber::SetHp(int hp)
{
    currenthp = std::max(0, std::min(hp, maxhp));
}

void HPber::SetMaxHp(int hp)
{
    maxhp = std::max(1, hp);
}

void HPber::Update()
{
    if (player) {
        SetHp(player->GetHp());
        SetMaxHp(player->GetMaxHp());
    }
}

void HPber::Draw()
{
    int graphW = 0, graphH = 0;
    GetGraphSize(HPImage, &graphW, &graphH);

    float hpRatio = (float)currenthp / maxhp;
    int drawW = static_cast<int>(graphW * hpRatio * IMAGE_SCALE);
    int drawH = static_cast<int>(graphH * IMAGE_SCALE);

    if (isLeftPlayer) { DrawRectGraph(IMAGE_POSITION_X, IMAGE_POSITION_Y, 0.0f, 0.0f, drawW, drawH, HPImage, TRUE, TRUE);} // ç∂
    else { DrawRotaGraph(IMAGE_POSITION_X * 3, IMAGE_POSITION_Y, 0.0f, 0.0f, drawW, drawH, HPImage, TRUE);} // âE
}
