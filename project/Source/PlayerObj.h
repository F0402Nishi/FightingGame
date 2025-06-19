#pragma once
#include "../Library/GameObject.h"
#include "Object3D.h"
#include "Player.h"



class PlayerObj : public GameObject
{
public:
	PlayerObj();
	~PlayerObj();
	void Update() override;
	void Draw() override;
	

private:
	

protected:
	
};
