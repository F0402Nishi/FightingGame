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
	int PoseLeftImage;
	int PoseRightImage;
	int TitleBackImage;
};
