#define NOMINMAX
#include <Windows.h>
#include <DxLib.h>
#include <assert.h>
#include <algorithm>
#include <vector> 
#include "../ImGui/imgui.h"
#include "2DUI.h"
#include "Character.h"
#include "Player.h"
#include "MiniWindow.h"

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

    battle = Battle::None;
    uitype = UIType::HP;
    mini = new MiniWindow();

    character = nullptr;

    currenthp = 0;
    maxhp = 0;
    displayHp = 0;
    blueY = -720;
    blueSpeed = 20;
    fullTank = 0;
    recoveryTimer = 0;
    recoveryHp = 0;
    recoveryFrame = 180;
    battleTime = 0;
    battleFrame = 0;
    damageHp = 0;

    angle = 0.0f;

    isLeftPlayer = false;
    isRightCpu = false;
}

bool UI2D::Init(Character* target, int Type, UIType _uitype)
{
    character = target;
    gameType = Type;
    uitype = _uitype;

    if (uitype == UIType::HP) {
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
    else if (uitype == UIType::B_Font) { return true; }

    return false;
}

void UI2D::Update()
{
    // === Battleの状態の管理 ===

    if (battle != Battle::None) {
        battleTime++;
        if (battleTime > battleFrame) { battle = Battle::None; }
    }

    // === HPbarの処理 ===

    if (character && uitype == UIType::HP) {
        
        // === 座標取得 ===
        Cpos = character->GetTransform().position;
        Cpos.y += 350.0f;

        fullTank = currenthp;
        SetHp(character->GetHp());
        SetMaxHp(character->GetMaxHp());

        // === トレーニングモードの回復処理.1 ===
        if (!isRightCpu && gameType == 1 && currenthp < fullTank) {
            recoveryTimer = 180;
            damageHp = fullTank - currenthp;
            recoveryHp += damageHp;
            recoveryTimer = recoveryFrame;
        }
    }

    if (displayHp > currenthp) { // 見た目用HPを徐々にcurrenthpに近づける
        displayHp -= std::max(1, (displayHp - currenthp) / 10); // 徐々に減る
    }

    // --- トレーニングモードの回復処理.2 ---

    if (!isRightCpu && gameType == 1 && character) {
        if (recoveryTimer > 0) { recoveryTimer--; }
        else {
            // 一気に全回復
            currenthp = maxhp;
            character->SetHp(maxhp);

            // 表示用HPも即追従させたいならこっち
            displayHp = maxhp;

            // もう一度ダメージを受けた時に備えてタイマーをリセットしたいなら
            recoveryTimer = recoveryHp;
        }
    }

    if (character && uitype == UIType::HP) {
        //ImGui::Begin("HPbar");
        //ImGui::Checkbox("isRightCpu", &isRightCpu);
        //ImGui::Checkbox("isLeftPlayer", &isLeftPlayer);
        //ImGui::Text("hp = %d", character->GetHp());
        //ImGui::InputInt("currenthp", &currenthp);
        //ImGui::InputInt("graphH", &graphH);
        //ImGui::InputInt("recoveryTimer", &recoveryTimer);
        //ImGui::End();
    }
}

void UI2D::Draw()
{
    // === バトル開始の文字表示 & Resultの処理 ===

    GameFont();

    // === トレーニングモードの情報を乗せる背景 

    if (isLeftPlayer && gameType == 1) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 189);

        // === 攻撃情報表示の背景 ===

        int startY = 70;      // 最初のY位置
        int height = 30;      // ボックスの高さ（110 - 70）
        int gap = 10;         // ボックス同士の間隔

        for (int i = 0; i < 3; i++) {
            int y1 = startY + (height + gap) * i;
            int y2 = y1 + height;
            DrawBox(440, y1, 840, y2, GetColor(0, 0, 0), TRUE);
        }

        // === 入力情報表示の背景 ===
        
        DrawBox(80, 130, 130, 560, GetColor(0, 0, 0), TRUE);
        
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        DrawExtendString(630, 78, 1, 1, "HP", GetColor(255, 255, 255));
        DrawExtendString(600, 118, 1, 1, "ダメージ", GetColor(255, 255, 255));
        DrawExtendString(600, 158, 1, 1, "モード", GetColor(255, 255, 255));
        DrawExtendString(90, 140, 1, 1, "入力", GetColor(255, 255, 255));
    }

    // === HPbar用の関数を呼び出し ===
    // === 名前タグ（PLAYER / CPU）描画 ===
    // キャラクターが存在するUIだけHPバーを描画

    if (character && uitype == UIType::HP) { 
        NameFont();
        HPbar(); 

        if (gameType == 1) {
            AttackInformation(); 
        }
    }
}

void UI2D::SetHp(int hp)
{
    currenthp = std::max(0, std::min(hp, maxhp));
}

void UI2D::SetMaxHp(int hp)
{
    maxhp = std::max(1, hp);
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

void UI2D::GameFont()
{
    const char* battleFontText = "";
    float battleFontSize = 15.0f;
    int battleFontColor = GetColor(255, 255, 255);

    switch (battle) {
    case Battle::Training:
        battleFontText = "TRAINING";
        battleFontColor = GetColor(255, 255, 255);
        DrawExtendString(100, 200, battleFontSize, battleFontSize, battleFontText, battleFontColor); // 文字を大きめに中央に描画
        break;
    case Battle::Ready:
        battleFontText = "READY";
        battleFontColor = GetColor(255, 255, 255);
        DrawExtendString(300, 200, battleFontSize, battleFontSize, battleFontText, battleFontColor); // 文字を大きめに中央に描画
        break;
    case Battle::Fight:
        battleFontText = "FIGHT";
        battleFontColor = GetColor(255, 255, 255);
        DrawExtendString(300, 200, battleFontSize, battleFontSize, battleFontText, battleFontColor);
        break;
    case Battle::KO:
        battleFontText = "K.O.";
        battleFontColor = GetColor(255, 255, 255);
        DrawExtendString(450, 200, battleFontSize, battleFontSize, battleFontText, battleFontColor);
        break;
    }
}

void UI2D::NameFont()
{
    VECTOR screenPos = ConvWorldPosToScreenPos(Cpos);

    const char* nameText = nullptr;
    int nameColor = GetColor(255, 255, 255);

    // --- プレイヤー1 ---
    if (isLeftPlayer) {
        nameText = "PLAYER";
        nameColor = GetColor(190, 0, 63); // 赤
    }

    // --- 対人戦の2P ---
    else if (!isLeftPlayer && !isRightCpu) {
        if (gameType == 1) { nameText = "CPU"; }
        else { nameText = "PLAYER2"; }
        nameColor = GetColor(0, 80, 255); // 青
    }

    // --- CPU ---
    else if (isRightCpu) {
        nameText = "CPU";
        nameColor = GetColor(0, 80, 255); // 青
    }

    if (nameText) {
        // 文字の影（黒）
        DrawString((int)screenPos.x - 19, (int)screenPos.y + 1, nameText, GetColor(0, 0, 0));
        // 本体
        DrawString((int)screenPos.x - 20, (int)screenPos.y, nameText, nameColor);
    }
}

void UI2D::AttackInformation()
{
    // === HP数値表示 ===
    char hpText[64];
    sprintf_s(hpText, "%d", character->GetHp(), maxhp);
    // DrawFormatString(100, 100, GetColor(255, 255, 255), hpText);

    
    if (isLeftPlayer) { 
        DrawFormatString(480, 78, GetColor(255, 255, 255), hpText); 
        DrawFormatString(500, 118, GetColor(255, 255, 255), "%d", damageHp);
    }
    else if (!isRightCpu) {
        DrawFormatString(760, 78, GetColor(255, 255, 255), hpText);
        DrawFormatString(760, 118, GetColor(255, 255, 255), "%d", damageHp);

        if (character->IsGuardOn()) {
            DrawFormatString(750, 158, GetColor(255, 255, 255), "ガード");
        }
        else
        {
            DrawFormatString(750, 158, GetColor(255, 255, 255), "待機");
        }
    }
}

void UI2D::DrawInputHistory(const std::vector<int>& inputHistory)
{
    // if (!isLeftPlayer) return; // トレーニングモードのみ描画

    int startX = 100;  // 描画開始X座標
    int startY = 170;  // 描画Y座標
    int spacing = 40;  // アイコン間の間隔

    for (int i = 0; i < (int)inputHistory.size(); i++)
    {
        int y = startY + i * spacing;
        int key = inputHistory[i];
        const char* icon = "?";

        switch (key)
        {
        case NEUTRAL:
            icon = "N";
            break;
        case KEY_INPUT_A:
            icon = "←";
            break;
        case KEY_INPUT_D:
            icon = "→";
            break;
        case KEY_INPUT_U:
            icon = "弱P";
            startX = 90;
            break;
        case KEY_INPUT_I:
            icon = "中P";
            startX = 90;
            break;
        case KEY_INPUT_O:
            icon = "強P";
            startX = 90;
            break;
        case KEY_INPUT_J:
            icon = "弱K";
            startX = 90;
            break;
        case KEY_INPUT_K:
            icon = "中K";
            startX = 90;
            break;
        case KEY_INPUT_L:
            icon = "強K";
            startX = 90;
            break;
        case KEY_INPUT_H:
            icon = "G";
            break;
        }

        DrawFormatString(startX, y, GetColor(255, 255, 255), icon);
    }
}

void UI2D::SetMessage(Battle newBattle, int BattleFrame)
{
    battle = newBattle;
    battleFrame = BattleFrame;
    battleTime = 0;
}
