#include "StdAfx.h"
#include "BackGround.h"

BackGround::BackGround()	
{
	SpriteObject::Load("images/background.jpg");
	assert(IsLoaded());
}


BackGround::~BackGround()
{
}


