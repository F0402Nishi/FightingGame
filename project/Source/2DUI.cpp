#define NOMINMAX
#include <Windows.h>
#include <DxLib.h>
#include <assert.h>
#include <algorithm>
#include "../ImGui/imgui.h"
#include "2DUI.h"
#include "Character.h"
#include "Player.h"

#define IMAGE_SCALE 1.0f
#define IMAGE_POSITION_LEFT_X 5.0f
#define IMAGE_POSITION_RIGHT_X 690.0f
#define IMAGE_POSITION_Y 0.0f
#define HPIMAGE_POSITION_Y 14.0f

UI2D::UI2D()
{
    HPbackImage = LoadGraph("data/2D/HPImage_1.png");
    assert(HPbackImage >= 0);
    HPImage = LoadGraph("data/2D/HP_FrontImage.png");
    assert(HPImage >= 0);
    HPImageLeft = LoadGraph("data/2D/HP_FrontImage_Left.png");
    assert(HPImageLeft >= 0);
    WinImage = LoadGraph("data/2D/WIN.png");
    assert(WinImage >= 0);

    character = nullptr;

    currenthp = 0;
    maxhp = 0;
    displayHp = 0;
    blueY = -720;
    blueSpeed = 20;
    fullTank = 0;
    recoveryDelayTimer = 0;

    angle = 0.0f;
}

bool UI2D::Init(Character* target)
{
    character = target;

    if (character) {
        currenthp = character->GetHp();
        maxhp = character->GetMaxHp();
        displayHp = currenthp;
        isLeftPlayer = character->GetisPlayey();
        isRightCpu = character->GetisCpu();
        return true;
    }
    else { return false; }
}

void UI2D::SetHp(int hp)
{
    currenthp = std::max(0, std::min(hp, maxhp));
}

void UI2D::SetMaxHp(int hp)
{
    maxhp = std::max(1, hp);
}

void UI2D::Update()
{
    // === HPbarの処理 ===

    if (character) {
        fullTank = currenthp;
        SetHp(character->GetHp());
        SetMaxHp(character->GetMaxHp());

        if (currenthp < fullTank) {
            recoveryDelayTimer = 180;
        }
    }

    if (displayHp > currenthp) { // 見た目用HPを徐々にcurrenthpに近づける
        displayHp -= std::max(1, (displayHp - currenthp) / 10); // 徐々に減る
    }

    // --- トレーニングモードの回復処理 ---

    if (!isRightCpu) {
        if (recoveryDelayTimer > 0) { recoveryDelayTimer--; }
        else {
            int damageHp = fullTank - currenthp;
            if (currenthp < maxhp) {
                currenthp += damageHp; // 回復量
                displayHp += currenthp;
                currenthp = std::min(currenthp, maxhp);
                SetHp(currenthp);
            }
        }
    }

    ImGui::Begin("HPbar");
    ImGui::Text("hp = %d", character->GetHp());
    ImGui::InputInt("currenthp", &currenthp);
    ImGui::InputInt("graphH", &graphH);
    ImGui::InputInt("recoveryDelayTimer", &recoveryDelayTimer);
    ImGui::End();
}

void UI2D::Draw()
{
    HPbar();

    // === Resultの処理 ===

    if (character->GetWinner()) { // 勝ち
        DrawExtendString(500, 100, 5, 5, "YOU WIN", GetColor(190, 0, 63));
    }
    else if (!character->GetWinner() && character->GetHp() <= 0) { // 負け
        DrawExtendString(500, 100, 5, 5, "YOU LOSE", GetColor(0, 0, 190));
    }
}

void UI2D::HPbar()
{
    if (character == nullptr || HPbackImage == -1) return;

    graphW = 0, graphH = 0;
    GetGraphSize(HPImage, &graphW, &graphH);

    float hpRatio = static_cast<float>(displayHp) / static_cast<float>(maxhp);
    int barW = static_cast<int>(graphW * hpRatio);
    int scaleBarW = static_cast<int>(barW * IMAGE_SCALE);

    int drawW = static_cast<int>(graphW * IMAGE_SCALE);
    int drawH = static_cast<int>(graphH * IMAGE_SCALE);

    if (isRightCpu) {
        DrawGraph(IMAGE_POSITION_RIGHT_X - 100.0f, IMAGE_POSITION_Y, HPbackImage, TRUE); // 背景とHPを合わせるためにX座標を調整

        // DrawRectExtendGraph(左端, 上, 右端, 下,,,)
        DrawRectExtendGraph(IMAGE_POSITION_RIGHT_X, HPIMAGE_POSITION_Y, IMAGE_POSITION_RIGHT_X + scaleBarW, HPIMAGE_POSITION_Y + drawH, 0.0f, 0.0f, barW, graphH, HPImage, TRUE);
    }
    else
    {
        if (isLeftPlayer) {
            DrawTurnGraph(IMAGE_POSITION_LEFT_X - 15.0f, IMAGE_POSITION_Y, HPbackImage, TRUE); // 背景とHPを合わせるためにX座標を調整
            DrawRectExtendGraph((IMAGE_POSITION_LEFT_X + drawW) - scaleBarW, HPIMAGE_POSITION_Y, IMAGE_POSITION_LEFT_X + drawW, HPIMAGE_POSITION_Y + drawH, graphW - barW, 0.0f, barW, graphH, HPImageLeft, TRUE);
        }
        else if (!isLeftPlayer) {
            DrawGraph(IMAGE_POSITION_RIGHT_X - 100.0f, IMAGE_POSITION_Y, HPbackImage, TRUE);
            DrawRectExtendGraph(IMAGE_POSITION_RIGHT_X, HPIMAGE_POSITION_Y, IMAGE_POSITION_RIGHT_X + scaleBarW, HPIMAGE_POSITION_Y + drawH, 0.0f, 0.0f, barW, graphH, HPImage, TRUE);
        }
    }
}