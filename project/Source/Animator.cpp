#include "Animator.h"
#include <DxLib.h>
#include <assert.h>

Animator::Animator(int m)
{
	handle = -1;
	model = m;
	attachID = -1;
	animTime = 0;
	animMaxTime = 0;
	currentFile = "";
}

Animator::~Animator()
{
	if (attachID >= 0) {
		MV1DetachAnim(model, attachID);
		attachID = -1;
		MV1DeleteModel(handle);
		attachID = -1;
	}
}

void Animator::Update()
{
	if (CheckHitKey(KEY_INPUT_I)) {
		animTime += 0.05f;
	}
	else {
		animTime += 0.5f;
	}

	if (loopMode) {
		if (animTime >= animMaxTime) {
			animTime -= animMaxTime;
			finished = false;
		}
	}
	else {
		if (!finished && animTime > animMaxTime) {
			animTime = animMaxTime;
			finished = true;
		}
	}
	MV1SetAttachAnimTime(model, attachID, animTime);

}

void Animator::Play(std::string filename, bool loop) // , bool force ‚Ì‚¿‚É’Ç‰Á
{
	if (currentFile == filename) return; // !force && ‚Ì‚¿‚É’Ç‰Á

	if (attachID >= 0) {
		MV1DetachAnim(model, attachID);
		MV1DeleteModel(handle);
		attachID = -1;
		handle = -1;
	}

	handle = MV1LoadModel(filename.c_str());
	assert(handle >= 0);
	attachID = MV1AttachAnim(model, 0, handle);
	animTime = 0.0f;
	animMaxTime = MV1GetAnimTotalTime(handle, 0);
	currentFile = filename;
	loopMode = loop;
	finished = false;
}

bool Animator::IsFinish()
{
	if (loopMode == false) {
		if (animTime >= animMaxTime) { return finished = true; }
	}

	return finished;
}

void Animator::ResetTime()
{
	animTime = 0.0f;
	MV1SetAttachAnimTime(model, attachID, animTime);
}
