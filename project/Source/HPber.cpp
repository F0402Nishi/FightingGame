
#define NOMINMAX
#include <Windows.h>
#include <DxLib.h>
#include <assert.h>
#include <algorithm>
#include "../ImGui/imgui.h"
#include "HPber.h"
#include "Player.h"

#define IMAGE_SCALE 2.0f
#define IMAGE_POSITION_X 480.0f
#define IMAGE_POSITION_Y 0.0f

HPber::HPber()
{
    HPImage = LoadGraph("data/2D/HpImage_4.png");
    assert(HPImage >= 0);

    player = nullptr;
    isLeftPlayer = true;

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

    ImGui::Begin("HPbar");
    ImGui::InputInt("graphWÅF", &graphW);
    ImGui::InputInt("graphHÅF", &graphH);
    ImGui::End();
}

void HPber::Draw()
{
    if (player == nullptr || HPImage == -1) return;

    graphW = 0, graphH = 0;
    GetGraphSize(HPImage, &graphW, &graphH);

    float hpRatio = (float)currenthp / (float)maxhp;
    int drawW = static_cast<int>(graphW * IMAGE_SCALE);
    int drawH = static_cast<int>(graphH * IMAGE_SCALE);
    int barW = static_cast<int>(graphW * hpRatio);

    // if (isLeftPlayer) { DrawRectExtendGraph(IMAGE_POSITION_X, IMAGE_POSITION_Y, IMAGE_POSITION_X + barW, IMAGE_POSITION_Y + drawH, HPImage, TRUE, TRUE);} // ç∂
    // else { DrawRotaGraph(IMAGE_POSITION_X * 3, IMAGE_POSITION_Y, 0.0f, 0.0f, drawW, drawH, HPImage, TRUE);} // âE
    DrawRectExtendGraph(IMAGE_POSITION_X, IMAGE_POSITION_Y, IMAGE_POSITION_X + drawW, IMAGE_POSITION_Y + drawH, 0.0f, 0.0f, drawW, drawH, HPImage, TRUE);
    // DrawRectExtendGraph(IMAGE_POSITION_X, IMAGE_POSITION_Y, IMAGE_POSITION_X + drawW, IMAGE_POSITION_Y + drawH, 0.0f, 0.0f, drawW, drawH, HPImage, FALSE);
}
