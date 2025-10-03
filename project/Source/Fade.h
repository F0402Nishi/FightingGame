#pragma once
#include "DxLib.h"

class Fade {
public:
	Fade();
	void Update();
	void FadeIn();
	void FadeOut();
	void Draw();

	bool IsFading() const { return fadingIn || fadingOut; }
	bool IsFadeInEnd() const { return alpha <= 0; }
	bool IsFadeOutEnd() const { return alpha >= 255; }

private:
	int alpha;
	int fadeSpped;

	bool fadingIn;
	bool fadingOut;
};
