/*********************************************************************************
* Simple background image - can be set
* Created by Charalampos Koundourakis <1603155@abertay.ac.uk>
*********************************************************************************/
#include "StdAfx.h"
#include "BackGround.h"

BackGround::BackGround(std::string filename)	
{
	SpriteObject::Load(filename);
	assert(IsLoaded());
}


BackGround::~BackGround()
{
}


