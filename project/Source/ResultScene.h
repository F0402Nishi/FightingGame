#pragma once
#include "../Library/SceneBase.h"
#include "Fade.h"

class ResultScene : public SceneBase
{
public:
	ResultScene();
	~ResultScene();
	void Update() override;
	void Draw() override;

private:
	Fade* fade;
};
