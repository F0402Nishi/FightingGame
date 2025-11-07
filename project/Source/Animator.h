#pragma once
#include <string>

class Animator
{
public:
	Animator(int m);
	~Animator();
	void Update();
	void Play(std::string filename, bool loop); // , bool force ‚Ì‚¿‚É’Ç‰Á
	bool IsFinish();
	void ResetTime();
	
	int GetModelHandle() const { return model; }
	float CurrentAnimTime() const { return static_cast<int> (animTime); }
	float TotalTime() const { return static_cast<int> (animMaxTime); }
	float NormalizedTime() const {
		if (animMaxTime > 0.0f) { return animTime / animMaxTime; }
		return 0.0f;
	}
	const std::string& GetCurrentFile() const { 
		if (currentFile.empty()) { return "data/Character/Player/Fight_Idle.mv1";}
		return currentFile;
	}

private:
	std::string currentFile;
	
	int handle;
	int attachID;
	int animMaxTime;
	int model;
	
	float animTime;
	
	bool loopMode;
	bool finished;
};
