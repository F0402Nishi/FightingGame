#pragma once
#include <string>

class Animator
{
public:
	Animator(int m);
	~Animator();
	void Update();
	void Play(std::string filename, bool loop);
	bool IsFinish();
	float CurrentAnimTime() { return animTime; }

private:
	int handle;
	int attachID;
	float animTime;
	int animMaxTime;
	int model;
	std::string currentFile;
	bool loopMode;
};
