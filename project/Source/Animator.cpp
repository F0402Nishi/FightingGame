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
		}
	}
	else {
		if (animTime > animMaxTime) {
			animTime = animMaxTime;
		}
	}
	MV1SetAttachAnimTime(model, attachID, animTime);

}

void Animator::Play(std::string filename, bool loop)
{
	if (currentFile == filename) return;

	if (attachID >= 0) {
		MV1DetachAnim(model, attachID);
		attachID = -1;
		MV1DeleteModel(handle);
		attachID = -1;
	}

	handle = MV1LoadModel(filename.c_str());
	assert(handle >= 0);
	attachID = MV1AttachAnim(model, 0, handle);
	animTime = 0.0f;
	animMaxTime = MV1GetAnimTotalTime(handle, 0);
	currentFile = filename;
	loopMode = loop;
}

bool Animator::IsFinish()
{
	if (loopMode == false) {
		if (animTime >= animMaxTime) { return true; }
	}

	return false;
}
