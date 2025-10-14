#include "SceneFactory.h"
#include <windows.h>
#include <assert.h>
#include "BootScene.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "PlayScene.h"
#include "ResultScene.h"

SceneBase* SceneFactory::CreateFirst()
{
	return new BootScene();
}

SceneBase * SceneFactory::Create(const std::string & name)
{
	if (name == "TITLE")
	{
		return new TitleScene();
	}
	if (name == "SELECT")
	{
		return new SelectScene();
	}
	if (name == "PLAY")
	{
		return new PlayScene();
	}
	if (name == "RESULT")
	{
		return new ResultScene();
	}
	MessageBox(NULL, ("éüÇÃÉVÅ[ÉìÇÕÇ†ÇËÇ‹ÇπÇÒ\n" + name).c_str(), "SceneFactory", MB_ICONERROR | MB_OK);
	assert(false);
	return nullptr;
}
