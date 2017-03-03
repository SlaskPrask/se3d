#include "../include/sprite.h"
#include "../include/enginecallback.h"
#include "../include/enginelayer.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

Sprite::Sprite()
{
	init();
}

void Sprite::init()
{
	w=h=0;
	texture=0;
	resource="";
}

Sprite::Sprite(const std::string &file,volatile bool threaded)
{
	init();
	load(file,threaded);
}

void Sprite::load(const std::string &file,volatile bool threaded)
{
	unload();
	
	texture=CallbackLoadPNG(file,&w,&h,threaded,&texture);
	
	if (texture==0)
	w=h=0;
	else
	{
		resource=file;
		EngineLayer::instance()->listLoadedSprite(this);
	}
}

void Sprite::reload()
{
	texture=CallbackLoadPNG(resource,&w,&h);
	
	if (texture==0)
	{
		w=h=0;
		resource="";
	}
}

void Sprite::unload()
{
	if (texture!=0)
	{
		glDeleteTextures(1,&texture);
		EngineLayer::instance()->unlistLoadedSprite(this);
	}
	resource="";
	w=h=0;
	texture=0;
}

Sprite::~Sprite()
{
	unload();
}