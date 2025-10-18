#include "Fade.h"

Fade::Fade()
{
	alpha = 0;
	fadeSpped = 7;

	fadingIn = false;
	fadingOut = false;
}

void Fade::Update()
{
	if (fadingIn) {
		alpha -= fadeSpped;
		if (alpha <= 0) {
			alpha = 0;
			fadingIn = false;
		}
	}
	else if (fadingOut) {
		alpha += fadeSpped;
		if (alpha >= 255) {
			alpha = 255;
			fadingOut = false;
		}
	}
}

void Fade::FadeIn()
{
	alpha = 255;
	fadingIn = true;
	fadingOut = false;
}

void Fade::FadeOut()
{
	alpha = 0;
	fadingIn = false;
	fadingOut = true;
}

void Fade::Draw()
{
	if (alpha <= 0) return; // Š®‘S“§–¾‚È‚ç•`‰æ‚µ‚È‚¢

	int screenW, screenH;
	GetDrawScreenSize(&screenW, &screenH);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, screenW, screenH, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Fade::ExclusiveFadeOut()
{
	FadeOut();
}
