#pragma once
#include "DxLib.h"

class Fade {
public:
	Fade();
	void Update();
	void FadeIn();
	void FadeOut();
	void Draw();

private:
	int alpha;
	int fadeSpped;

	bool fadingIn;
	bool fadingOut;
};
