#pragma once
#include <string>

class Animator
{
public:
	Animator(int m);
	~Animator();
	void Update();
	void Play(std::string filename, bool loop, bool force);
	bool IsFinish();
	
	int GetModelHandle() const { return model; }
	float CurrentAnimTime() const { return static_cast<int> (animTime); }
	float TotalTime() const { return static_cast<int> (animMaxTime); }
	float NormalizedTime() const {
		if (animMaxTime > 0.0f) { return animTime / animMaxTime; }
		return 0.0f;
	}

private:
	int handle;
	int attachID;
	float animTime;
	int animMaxTime;
	int model;
	std::string currentFile;
	bool loopMode;
};
