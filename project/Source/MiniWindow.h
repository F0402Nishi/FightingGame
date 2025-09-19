#pragma once
#include <DxLib.h>
#include "Object3D.h"

class SelectScene;

class MiniWindow : public Object3D
{
public:
	MiniWindow(SelectScene* scene);
	~MiniWindow();
	void Update() override;
	void Draw() override;

private:
	SelectScene* selectScene;

	int scrollOffset;
	bool windowOpen;
};
