#pragma once
#include "../Library/SceneBase.h"
#include "Screen.h"

/// <summary>
/// タイトルシーン
/// 
/// タイトルを表示して、キーを押したらプレイシーンに移行する。
/// </summary>
class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene();
	void Update() override;
	void Draw() override;

private:
	int MechImage;
	int BrawlersImage;
	int TitleBackImage;
	int MechSe;
	int BrawlersSe;
	int TitleBgm;

	float Mechscale; // 開始時の縮小率
	float Brawlersscale; // 開始時の縮小率
	float scaleSpeed; // 拡大スピード

	bool SeCount;
	bool ScaleCount;
	bool playSe;
	bool playedSecond;
};
