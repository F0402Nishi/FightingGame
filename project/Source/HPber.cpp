#define NOMINMAX
#include <Windows.h>
#include <DxLib.h>
#include <assert.h>
#include <algorithm>
#include "../ImGui/imgui.h"
#include "HPber.h"
#include "Player.h"

#define IMAGE_SCALE 1.0f
#define IMAGE_POSITION_LEFT_X -10.0f
#define IMAGE_POSITION_RIGHT_X 590.0f
#define IMAGE_POSITION_Y 0.0f

HPber::HPber()
{
    HPbackImage = LoadGraph("data/2D/HPImage_1.png");
    assert(HPbackImage >= 0);
    HPImage = LoadGraph("data/2D/HP_FrontImage.png");
    assert(HPImage >= 0);

    player = nullptr;

    currenthp = 0;
    maxhp = 0;
}

bool HPber::Init(Player* target)
{
    player = target;

    if (player) {
        currenthp = player->GetHp();
        maxhp = player->GetMaxHp();
        isLeftPlayer = player->GetisPlayer();
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
    ImGui::InputInt("currenthpÅF", &currenthp);
    // ImGui::InputInt("graphHÅF", &graphH);
    ImGui::End();
}

void HPber::Draw()
{
    if (player == nullptr || HPbackImage == -1) return;

    graphW = 0, graphH = 0;
    GetGraphSize(HPImage, &graphW, &graphH);

    float hpRatio = static_cast<float>(currenthp) / static_cast<float>(maxhp);
    int barW = static_cast<int>(graphW * hpRatio);
    int scaleBarW = static_cast<int>(barW * IMAGE_SCALE);

    int drawW = static_cast<int>(graphW * IMAGE_SCALE);
    int drawH = static_cast<int>(graphH * IMAGE_SCALE);

    

    if (isLeftPlayer) { 
        DrawTurnGraph(IMAGE_POSITION_LEFT_X, IMAGE_POSITION_Y, HPbackImage, TRUE);
        DrawRectExtendGraph(IMAGE_POSITION_LEFT_X + scaleBarW, IMAGE_POSITION_Y, IMAGE_POSITION_LEFT_X, IMAGE_POSITION_Y + drawH, 0.0f, 0.0f, barW, graphH, HPImage, TRUE);
    }
    else if (!isLeftPlayer) { 
        DrawGraph(IMAGE_POSITION_RIGHT_X, IMAGE_POSITION_Y, HPbackImage, TRUE);
        DrawRectExtendGraph(IMAGE_POSITION_RIGHT_X, IMAGE_POSITION_Y, IMAGE_POSITION_RIGHT_X + scaleBarW, IMAGE_POSITION_Y + drawH, 0.0f, 0.0f, barW, graphH, HPImage, TRUE);
    }
}
