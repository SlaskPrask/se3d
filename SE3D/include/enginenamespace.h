#pragma once

#include "enginesettings.h"
#include <string>
#include <climits>
#include <cmath>
#include "enginelayer.h"
#include "font.h"
#include "enginenamespaceext.h"

namespace _ENGINESPACE
{
	using namespace _engineprivate;
	
	const int ALIGN_LEFT=0;
	const int ALIGN_CENTER=1;
	const int ALIGN_RIGHT=2;
	//const int ALIGN_TOP=0;
	//const int ALIGN_BOTTOM=2;

	int run(int argc,char* argv[],void(*func)());
	inline int run(void(*func)())
	{
		return run(0,NULL,func);
	}
	inline void setAdsFlexible(const std::string &adKey,bool onTop=1)
	{
		EngineLayer::instance()->startAds(adKey,3,onTop);
	}
	inline void setAdsLarge(const std::string &adKey,bool onTop=1)
	{
		EngineLayer::instance()->startAds(adKey,2,onTop);
	}
	inline void setAdsMedium(const std::string &adKey,bool onTop=1)
	{
		EngineLayer::instance()->startAds(adKey,1,onTop);
	}
	inline void setAdsSmall(const std::string &adKey,bool onTop=1)
	{
		EngineLayer::instance()->startAds(adKey,0,onTop);
	}
	inline void setAds(const std::string &adKey,bool onTop=1)
	{
		setAdsSmall(adKey,onTop);
	}
	inline void unsetAds()
	{
		EngineLayer::instance()->endAds();
	}
	inline bool fileExists(const std::string &file)
	{
		return EngineLayer::instance()->fileExists(file);
	}
	inline bool fileDelete(const std::string &file)
	{
		return EngineLayer::instance()->fileDelete(file);
	}
	inline bool fileWrite(const std::string &file,const std::string &text)
	{
		return EngineLayer::instance()->fileWrite(file,text);
	}
	inline bool fileRead(const std::string &file,std::string *text)
	{
		return EngineLayer::instance()->fileRead(file,text);
	}
	inline bool isFocusGained()
	{
		return EngineLayer::instance()->getFocusEvent();
	}
	inline bool isTextSubmitted()
	{
		return EngineLayer::instance()->getTextSubmitted();
	}
	inline bool isFocusLost()
	{
		return EngineLayer::instance()->getUnfocusEvent();
	}
	inline bool isWindowResized()
	{
		return EngineLayer::instance()->getResizeEvent();
	}
	inline void loaderAdd(ResourceSet *set)
	{
		EngineLayer::instance()->loaderAdd(set);
	}
	inline void loaderAddId(unsigned int i)
	{
		EngineLayer::instance()->loaderAdd(EngineLayer::instance()->resourceSet(i));
	}
	inline void loaderAddId(int i)
	{
		EngineLayer::instance()->loaderAdd(EngineLayer::instance()->resourceSet(i));
	}
	inline unsigned int loaderGetItems()
	{
		return EngineLayer::instance()->loaderItems();
	}
	inline unsigned int loaderGetLoadedItems()
	{
		return EngineLayer::instance()->loaderLoadedItems();
	}
	inline void loaderClear()
	{
		EngineLayer::instance()->loaderClear();
	}
	inline void loaderLoad()
	{
		EngineLayer::instance()->loaderLoad();
	}
	inline bool loaderIsDone()
	{
		return EngineLayer::instance()->loaderDone();
	}
	inline std::string getResourceFilename(const std::string &str)
	{
		return EngineLayer::instance()->resourceDirectory(str);
	}
	inline std::string getFilename(const std::string &str)
	{
		return EngineLayer::instance()->workingDirectory(str);
	}
	inline void setResourceDirectory(const std::string &str)
	{
		EngineLayer::instance()->setResourceDirectory(str);
	}
	inline double getAdHeight()
	{
		return EngineLayer::instance()->getAdHeight();
	}
	inline double getDeviceScale()
	{
		return EngineLayer::instance()->getDeviceScale();
	}
	inline double getAdWindowHeight()
	{
		return EngineLayer::instance()->getAdWindowHeight();
	}
	inline Scene* getScene()
	{
		return EngineLayer::instance()->getCurrentScene();
	}
	template<class T>
	T* getObject(Object *object)
	{
		return object->objectId()==EngineLayer::instance()->getObjectId<T>()?static_cast<T*>(object):NULL;
	}
	template<class T>
	T* getScene(Scene *scene)
	{
		return scene->sceneId()==EngineLayer::instance()->getSceneId<T>()?static_cast<T*>(scene):NULL;
	}
	template<class T>
	T* getScene()
	{
		return (!getScene()?NULL:(getScene()->sceneId()==EngineLayer::instance()->getSceneId<T>()?static_cast<T*>(getScene()):NULL));
	}
	template<class T>
	unsigned int getObjectId()
	{
		return EngineLayer::instance()->getObjectId<T>();
	}
	template<class T>
	unsigned int getSceneId()
	{
		return EngineLayer::instance()->getSceneId<T>();
	}
	inline bool setFullscreen()
	{
		return EngineLayer::instance()->setFullscreen();
	}
	inline bool setWindowed()
	{
		return EngineLayer::instance()->setWindowed();
	}
	inline bool setFullscreenWindowed()
	{
		return EngineLayer::instance()->setFullscreenWindowed();
	}
	inline bool setFullscreen(int w,int h)
	{
		return EngineLayer::instance()->setFullscreen(w,h);
	}
	inline bool setWindowed(int w,int h)
	{
		return EngineLayer::instance()->setWindowed(w,h);
	}
	inline void enableVSync()
	{
		EngineLayer::instance()->setVSync(1);
	}
	inline void disableVSync()
	{
		EngineLayer::instance()->setVSync(0);
	}
	inline std::string getTitle()
	{
		 return EngineLayer::instance()->getTitle();
	}
	inline bool getVSync()
	{
		return EngineLayer::instance()->getVSync();
	}
	inline void setTitle(std::string title)
	{
		EngineLayer::instance()->setTitle(title);
	}

	inline int getResolutions()
	{
		return EngineLayer::instance()->getResolutions();
	}
	inline int getResolutionWidth(unsigned int i)
	{
		return EngineLayer::instance()->getResolutionWidth(i);
	}
	inline int getResolutionHeight(unsigned int i)
	{
		return EngineLayer::instance()->getResolutionHeight(i);
	}
	
	inline ResourceSet* getResourceSet(unsigned int i)
	{
		return EngineLayer::instance()->resourceSet(i);
	}
	inline ResourceSet* resourceSet(unsigned int i)
	{
		return getResourceSet(i);
	}
	inline void setInternalResolution(unsigned int w,unsigned int h)
	{
		EngineLayer::instance()->setupInternalResolution(1,w,h);
	}
	inline void unsetInternalResolution()
	{
		EngineLayer::instance()->setupInternalResolution(0);
	}
	inline void getTextSize(Font *font,const std::string &str,double size,double *w,double *h,double lineSpacing=0)
	{
		EngineLayer::instance()->getTextMetrics(font,str,size,lineSpacing,w,h);
	}
	inline double getTextWidth(Font *font,const std::string &str,double size)
	{
		double w,h;
		EngineLayer::instance()->getTextMetrics(font,str,size,0,&w,&h);
		return w;
	}
	inline double getTextHeight(Font *font,const std::string &str,double size,double lineSpacing=0)
	{
		double w,h;
		EngineLayer::instance()->getTextMetrics(font,str,size,lineSpacing,&w,&h);
		return h;
	}
	inline void drawText(Font *font,const std::string &str,double x,double y,double size,double r=1,double g=1,double b=1,double a=1,double lineSpacing=0)
	{
		EngineLayer::instance()->drawText(font,str,x,y,size,lineSpacing,r,g,b,a,_FONT_ALIGN_LEFT);
	}
	inline void drawTextLeft(Font *font,const std::string &str,double x,double y,double size,double r=1,double g=1,double b=1,double a=1,double lineSpacing=0)
	{
		drawText(font,str,x,y,size,r,g,b,a,lineSpacing);
	}
	inline void drawTextCentered(Font *font,const std::string &str,double x,double y,double size,double r=1,double g=1,double b=1,double a=1,double lineSpacing=0)
	{
		EngineLayer::instance()->drawText(font,str,x,y,size,lineSpacing,r,g,b,a,_FONT_ALIGN_MIDDLE);
	}
	inline void drawTextRight(Font *font,const std::string &str,double x,double y,double size,double r=1,double g=1,double b=1,double a=1,double lineSpacing=0)
	{
		EngineLayer::instance()->drawText(font,str,x,y,size,lineSpacing,r,g,b,a,_FONT_ALIGN_RIGHT);
	}
	
	inline void drawSprite(Sprite* sprite,double x,double y)
	{
		EngineLayer::instance()->drawSprite(sprite,x,y,sprite->getWidth(),sprite->getHeight(),0,0,1,1,0,1,1,1,1);
	}
	inline void drawSpriteRot(Sprite* sprite,double x,double y,double rot)
	{
		EngineLayer::instance()->drawSprite(sprite,x,y,sprite->getWidth(),sprite->getHeight(),0,0,1,1,rot,1,1,1,1);
	}
	inline void drawSprite(Sprite* sprite,double x,double y,double w,double h)
	{
		EngineLayer::instance()->drawSprite(sprite,x,y,w,h,0,0,1,1,0,1,1,1,1);
	}
	inline void drawSpriteRot(Sprite* sprite,double x,double y,double w,double h,double rot)
	{
		EngineLayer::instance()->drawSprite(sprite,x,y,w,h,0,0,1,1,rot,1,1,1,1);
	}
	inline void drawSpriteColorRot(Sprite* sprite,double x,double y,double w,double h,double rot,double r,double g,double b,double a)
	{
		EngineLayer::instance()->drawSprite(sprite,x,y,w,h,0,0,1,1,rot,r,g,b,a);
	}
	inline void drawSpriteColor(Sprite* sprite,double x,double y,double w,double h,double r,double g,double b,double a)
	{
		EngineLayer::instance()->drawSprite(sprite,x,y,w,h,0,0,1,1,0,r,g,b,a);
	}
	inline void drawSpritePart(Sprite* sprite,double x,double y,double w,double h,double fromx,double fromy,double tox,double toy)
	{
		EngineLayer::instance()->drawSprite(sprite,x,y,w,h,fromx,fromy,tox,toy,0,1,1,1,1);
	}
	inline void drawSpriteExt(Sprite* sprite,double x,double y,double w,double h,double fromx,double fromy,double tox,double toy,double rot,double r,double g,double b,double a)
	{
		EngineLayer::instance()->drawSprite(sprite,x,y,w,h,fromx,fromy,tox,toy,rot,r,g,b,a);
	}
	inline void drawSpriteExt2(Sprite* sprite,double x,double y,double w,double h,double texx1,double texy1,double texx2,double texy2,double texx3,double texy3,double texx4,double texy4,double rot,double r,double g,double b,double a)
	{
		EngineLayer::instance()->drawSpriteFinal(sprite,x,y,w,h,texx1,texy1,texx2,texy2,texx3,texy3,texx4,texy4,rot,r,g,b,a);
	}
	/*inline void drawSpritePoly(Sprite* sprite,double x1,double y1,double x2,double y2,double x3,double y3,double texx1,double texy1,double texx2,double texy2,double texx3,double texy3)
	{
		EngineLayer::instance()->drawSpritePoly(sprite,x1,y1,x2,y2,x3,y3,texx1,texy1,texx2,texy2,texx3,texy3,0,1,1,1,1);
	}
	inline void drawSpritePolyRot(Sprite* sprite,double x1,double y1,double x2,double y2,double x3,double y3,double texx1,double texy1,double texx2,double texy2,double texx3,double texy3,double rot)
	{
		EngineLayer::instance()->drawSpritePoly(sprite,x1,y1,x2,y2,x3,y3,texx1,texy1,texx2,texy2,texx3,texy3,rot,1,1,1,1);
	}*/
	inline void drawPoly(double x1,double y1,double x2,double y2,double x3,double y3,double r,double g,double b,double a)
	{
		EngineLayer::instance()->drawPoly(x1,y1,x2,y2,x3,y3,0,r,g,b,a);
	}
	inline void drawPolyRot(double x1,double y1,double x2,double y2,double x3,double y3,double rot,double r,double g,double b,double a)
	{
		EngineLayer::instance()->drawPoly(x1,y1,x2,y2,x3,y3,rot,r,g,b,a);
	}
	inline void drawPolyExt(double x1,double y1,double x2,double y2,double x3,double y3,double rot,double r,double g,double b,double a)
	{
		EngineLayer::instance()->drawPoly(x1,y1,x2,y2,x3,y3,rot,r,g,b,a);
	}
	inline void drawRectangle(double x,double y,double w,double h,double r,double g,double b,double a)
	{
		EngineLayer::instance()->drawRectangle(x,y,w,h,0,r,g,b,a);
	}
	inline void drawRectangleRot(double x,double y,double w,double h,double rot,double r,double g,double b,double a)
	{
		EngineLayer::instance()->drawRectangle(x,y,w,h,rot,r,g,b,a);
	}
	/*inline void drawEllipse(double x,double y,double w,double h,double r,double g,double b,double a)
	{
		EngineLayer::instance()->drawEllipse(x,y,w,h,0,r,g,b,a);
	}
	inline void drawEllipse(double x,double y,double w,double h,double rot,double r,double g,double b,double a)
	{
		EngineLayer::instance()->drawEllipse(x,y,w,h,rot,r,g,b,a);
	}*/

	inline void setStayAwake(bool awake=1)
	{
		EngineLayer::instance()->setStayAwake(awake);
	}
	inline void unsetStayAwake()
	{
		EngineLayer::instance()->setStayAwake(0);
	}

	inline void drawAnimationExt(Animation* anim,double x,double y,double w,double h,double rot,double fromx,double fromy,double tox,double toy,double r,double g,double b,double a)
	{
		EngineLayer::instance()->drawSprite(anim->getSprite(),x,y,w,h,transform(fromx,anim->getFromX(),anim->getToX()),transform(fromy,anim->getFromY(),anim->getToY()),transform(tox,anim->getFromX(),anim->getToX()),transform(toy,anim->getFromY(),anim->getToY()),rot,r,g,b,a);
	}
	inline void drawAnimationColorRot(Animation* anim,double x,double y,double w,double h,double rot,double r,double g,double b,double a)
	{
		drawAnimationExt(anim,x,y,w,h,rot,0,0,1,1,r,g,b,a);
	}
	inline void drawAnimationPart(Animation* anim,double x,double y,double w,double h,double fromx,double fromy,double tox,double toy)
	{
		drawAnimationExt(anim,x,y,w,h,0,fromx,fromy,tox,toy,1,1,1,1);
	}
	inline void drawAnimation(Animation* anim,double x,double y)
	{
		drawAnimationExt(anim,x,y,anim->getWidth(),anim->getHeight(),0,0,0,1,1,1,1,1,1);
	}
	inline void drawAnimation(Animation* anim,double x,double y,double w,double h)
	{
		drawAnimationExt(anim,x,y,w,h,0,0,0,1,1,1,1,1,1);
	}
	inline void drawAnimationRot(Animation* anim,double x,double y,double w,double h,double rot)
	{
		drawAnimationExt(anim,x,y,w,h,rot,0,0,1,1,1,1,1,1);
	}
	inline void setColorization(double r,double g,double b)
	{
		EngineLayer::instance()->setColorization(r,g,b);
	}
	
	inline bool getBackButton()
	{
		return EngineLayer::instance()->getBackButton();
	}
	
	template<class T>
	T* at(T* t,double x,double y)
	{
		t->at(x,y);
		return t;
	}
	template<class T>
	T* getInstance()
	{
		return static_cast<T*>(T::instance());
	}
	template<class T>
	T* getNextInstance()
	{
		return static_cast<T*>(T::nextInstance());
	}
	
	inline int playMusic(Sound *sound,double vol=1,double speed=1)
	{
		return EngineLayer::instance()->sndPlay(sound,vol,vol,INT_MAX,1,speed);
	}
	inline int playSound(Sound *sound,double vol=1,bool loop=0,int prio=1,double speed=1)
	{
		return EngineLayer::instance()->sndPlay(sound,vol,vol,prio,loop,speed);
	}
	inline int loopSound(Sound *sound,double vol=1,int prio=1,double speed=1)
	{
		return EngineLayer::instance()->sndPlay(sound,vol,vol,prio,1,speed);
	}
	inline int playSound3D(Sound *sound,double left=1,double right=1,bool loop=0,int prio=1,double speed=1)
	{
		return EngineLayer::instance()->sndPlay(sound,left,right,prio,loop,speed);
	}
	inline int loopSound3D(Sound *sound,double left=1,double right=1,int prio=1,double speed=1)
	{
		return EngineLayer::instance()->sndPlay(sound,left,right,prio,1,speed);
	}
	inline void stopSound(int i)
	{
		EngineLayer::instance()->sndStop(i);
	}
	inline void stopSound(Sound *sound)
	{
		EngineLayer::instance()->sndStop(sound);
	}
	inline void setSoundVolume(int i,double vol)
	{
		EngineLayer::instance()->sndVolume(i,vol,vol);
	}
	inline void setSoundVolume3D(int i,double left,double right)
	{
		EngineLayer::instance()->sndVolume(i,left,right);
	}
	inline void setSoundSpeed(int i,double s)
	{
		EngineLayer::instance()->sndSpeed(i,s);
	}
	inline void unpauseSound(int i)
	{
		EngineLayer::instance()->sndUnpause(i);
	}
	inline void resumeSound(int i)
	{
		unpauseSound(i);
	}
	inline void pauseSound(int i)
	{
		EngineLayer::instance()->sndPause(i);
	}
	
	/*double textWidth(Font *font,const char* str,double size,double lineSpacing=0);
	inline double textWidth(Font *font,const std::string &str,double size,double lineSpacing=0)
	{
		return textWidth(font,str.c_str(),size,lineSpacing);
	}
	double textHeight(Font *font,const char* str,double size,double lineSpacing=0);
	inline double textHeight(Font *font,const std::string &str,double size,double lineSpacing=0)
	{
		return textHeight(font,str.c_str(),size,lineSpacing);
	}*/
	inline bool openURL(const std::string &uri)
	{
		return EngineLayer::instance()->openURL(uri);
	}
	int getSceneWidth();
	int getSceneHeight();
	inline double getFrameTime()
	{
		return EngineLayer::instance()->getDelta();
	}
	inline double delta()
	{
		return getFrameTime();
	}
	inline double getTimeScale()
	{
		return EngineLayer::instance()->getTimeScale();
	}
	inline void setTimeScale(double scale=1)
	{
		EngineLayer::instance()->setTimeScale(scale);
	}
	inline void showMessage(const std::string &str)
	{
		EngineLayer::instance()->sendMessage(str);
	}
	inline void requestInput(bool newlines=0)
	{
		EngineLayer::instance()->requestInput(newlines);
	}
	inline void requestInputText(bool newlines=0)
	{
		requestInput(newlines);
	}
	inline void requestInputNumber()
	{
		EngineLayer::instance()->requestInputNumber();
	}
	inline void requestInputEmail()
	{
		EngineLayer::instance()->requestInputEmail();
	}

	inline double getMouseX(int i=0)
	{
		return (i<0||i>=_MAX_MOUSES?0:EngineLayer::instance()->getMouseTranslatedX(i));
	}
	inline double getMouseY(int i=0)
	{
		return (i<0||i>=_MAX_MOUSES?0:EngineLayer::instance()->getMouseTranslatedY(i));
	}
	inline int getMouseWindowX(int i=0)
	{
		return (i<0||i>=_MAX_MOUSES?0:EngineLayer::instance()->getMouseX(i));
	}
	inline int getMouseWindowY(int i=0)
	{
		return (i<0||i>=_MAX_MOUSES?0:EngineLayer::instance()->getMouseY(i));
	}
	inline bool getMousePress(int button=0,int i=0)
	{
		return (i<0||i>=_MAX_MOUSES?0:EngineLayer::instance()->getMousePress(button,i));
	}
	inline bool getMouseRelease(int button=0,int i=0)
	{
		return (i<0||i>=_MAX_MOUSES?0:EngineLayer::instance()->getMouseRelease(button,i));
	}
	inline bool getMouseHeld(int button=0,int i=0)
	{
		return (i<0||i>=_MAX_MOUSES?0:EngineLayer::instance()->getMouseHeld(button,i));
	}
	inline bool getMouseIdle(int button=0,int i=0)
	{
		return (i<0||i>=_MAX_MOUSES?0:EngineLayer::instance()->getMouseUnheld(button,i));
	}
	inline double getTouchX(int i=0)
	{
		return getMouseX(i);
	}
	inline double getTouchY(int i=0)
	{
		return getMouseY(i);
	}
	inline int getTouchWindowX(int i=0)
	{
		return getTouchWindowX(i);
	}
	inline int getTouchWindowY(int i=0)
	{
		return getTouchWindowY(i);
	}
	inline bool getTouchPress(int i=0)
	{
		return getMousePress(i,0);
	}
	inline bool getTouchRelease(int i=0)
	{
		return getMouseRelease(i,0);
	}
	inline bool getTouchHeld(int i=0)
	{
		return getMouseHeld(i,0);
	}
	inline bool getTouchIdle(int i=0)
	{
		return getMouseIdle(i,0);
	}
	inline void getTouchPresses(int i[_MAX_MOUSES])//get a list of pressed mouse numbers
	{
		EngineLayer::instance()->getMousePresses(i);
	}
	inline int getTouchPresses(int i)//get number of pressed mouse in list of mouses
	{
		return EngineLayer::instance()->getMousePresses(i);
	}
	inline bool getTouchPresses()//any?
	{
		return EngineLayer::instance()->getMousePresses();
	}
	inline bool getKeyPress(int i)
	{
		return EngineLayer::instance()->getKeyPress(i);
	}
	inline bool getKeyRelease(int i)
	{
		return EngineLayer::instance()->getKeyRelease(i);
	}
	inline bool getKeyHeld(int i)
	{
		return EngineLayer::instance()->getKeyHeld(i);
	}
	inline bool getKey(int i)
	{
		return getKeyHeld(i);
	}
	inline bool getKeyIdle(int i)
	{
		return EngineLayer::instance()->getKeyIdle(i);
	}
	inline int getMouseWheelUp(int i=0)
	{
		return EngineLayer::instance()->getMouseWheelUp(i);
	}
	inline int getMouseWheelDown(int i=0)
	{
		return EngineLayer::instance()->getMouseWheelDown(i);
	}
	inline void keyboardTextBuffer(std::string *str,unsigned int valuelimit=256,bool newlines=0)
	{
		EngineLayer::instance()->addKeyboardChar(str,newlines,valuelimit);
	}
	inline bool getAnyKeyPress()
	{
		return EngineLayer::instance()->getAnyKeyPress();
	}
	inline bool getAnyKeyRelease()
	{
		return EngineLayer::instance()->getAnyKeyRelease();
	}
	inline double getKeyboardSize()
	{
		return EngineLayer::instance()->getKeyboardSize();
	}

	//???
	inline bool getMouse(int i)
	{
		return getTouchHeld(i);
	}
	inline void destroyAllObjects()
	{
		EngineLayer::instance()->deleteAllObjects();
	}
	template<class T>
	void destroyAll()
	{
		T *t=getInstance<T>();
		while (t)
		{
			t->destroy();
			t=getInstance<T>();
		}
	}
	inline void exitGame()
	{
		EngineLayer::instance()->gameEnd();
	}
	inline void setGameStartFunction(void(*func)(void))
	{
		EngineLayer::instance()->setGameStartFunc(func);
	}
	inline void setGameEndFunction(void(*func)(void))
	{
		EngineLayer::instance()->setGameEndFunc(func);
	}
	inline void setGameWindowResizeFunction(void(*func)(void))
	{
		EngineLayer::instance()->setGameWindowResizeFunc(func);
	}
	inline void setGameFocusGainFunction(void(*func)(void))
	{
		EngineLayer::instance()->setGameFocusGainFunc(func);
	}
	inline void setGameFocusLossFunction(void(*func)(void))
	{
		EngineLayer::instance()->setGameFocusLossFunc(func);
	}
	inline void setGameProfileConnectedFunction(void(*func)(void))
	{
		EngineLayer::instance()->setGameGamesConnectedFunc(func);
	}
	inline void setGameProfileDisconnectedFunction(void(*func)(void))
	{
		EngineLayer::instance()->setGameGamesDisconnectedFunc(func);
	}
	inline void setGameProfileSuspendedFunction(void(*func)(void))
	{
		EngineLayer::instance()->setGameGamesSuspendedFunc(func);
	}
	inline void setOrientationLandscape()
	{
		EngineLayer::instance()->setOrientation(0);
	}
	inline void setOrientationPortrait()
	{
		EngineLayer::instance()->setOrientation(1);
	}
	inline void setOrientationFree()
	{
		EngineLayer::instance()->setOrientation(2);
	}
	inline void setIcon(const std::string &file)
	{
		EngineLayer::instance()->setIcon(file);
	}
	inline int getAllObjects()
	{
		return EngineLayer::instance()->objectCountAll();
	}
	template <class T>
	inline int getObjects()
	{
		return EngineLayer::instance()->objectCount<T>();
	}
	inline bool isMobile()
	{
		return EngineLayer::mobile;
	}
	inline void setFPS(int fps)
	{
		EngineLayer::instance()->setFPS(fps);
	}
	inline int getFPS()
	{
		return EngineLayer::instance()->getFPS();
	}
	inline void setMaxFrameTime(double max)
	{
		EngineLayer::instance()->setMaxFrameTime(max);
	}
	inline void setMaxDelta(double max)
	{
		setMaxFrameTime(max);
	}
	inline double getMaxFrameTime()
	{
		return EngineLayer::instance()->getMaxFrameTime();
	}
	inline double getMaxDelta()
	{
		return getMaxFrameTime();
	}
	inline int getCurrentFPS()
	{
		return EngineLayer::instance()->getCurrentFPS();
	}
	template<typename T>
	inline T random(T max)
	{
		return max>0?rand()%(max):0;
	}
	inline double random(double max)
	{
		int full=(int)max;
		double dec=max-full;
		if (full>=1)
		{
			full--;
			dec+=1;
		}
		return (double)(full>0?rand()%full:0)+(rand()%((int)(dec*100.0f)))/100.0f;
	}
	inline float random(float max)
	{
		int full=(int)max;
		float dec=max-full;
		if (full>=1)
		{
			full--;
			dec+=1;
		}
		return (float)(full>0?rand()%full:0)+(rand()%((int)(dec*100.0f)))/100.0f;
	}
	inline void beginTag(Tag *t)
	{
		EngineLayer::instance()->beginTag(t);
	}
	inline void endTag(Tag *t)
	{
		EngineLayer::instance()->endTag(t);
	}
	inline void windowScalingFit()
	{
		EngineLayer::instance()->scaleFit();
	}
	inline void windowScalingStretch()
	{
		EngineLayer::instance()->scaleStretch();
	}
	inline void deactivateCamera()
	{
		EngineLayer::instance()->setCamera(NULL);
	}
	inline Camera* getCamera()
	{
		return EngineLayer::instance()->getCamera();
	}
	inline double getCameraWidth()
	{
		return EngineLayer::instance()->getCameraW();
	}
	inline double getCameraHeight()
	{
		return EngineLayer::instance()->getCameraH();
	}
	inline double getCameraX()
	{
		return EngineLayer::instance()->getCameraX();
	}
	inline double getCameraY()
	{
		return EngineLayer::instance()->getCameraY();
	}
	inline int getWindowWidth()
	{
		return EngineLayer::instance()->getWidth();
	}
	inline int getWindowHeight()
	{
		return EngineLayer::instance()->getHeight();
	}
	inline double getScreenRatio()
	{
		return EngineLayer::instance()->getScreenRatio();
	}

	inline void profileConnect()
	{
		return EngineLayer::instance()->gamesRequestConnect();
	}
	inline void profileDisconnect()
	{
		return EngineLayer::instance()->gamesRequestDisconnect();
	}
	inline bool isProfileConnected()
	{
		return EngineLayer::instance()->getGamesConnected();
	}
	inline bool isProfileDisconnected()
	{
		return EngineLayer::instance()->getGamesDisconnected();
	}
	inline bool isProfileSuspended()
	{
		return EngineLayer::instance()->getGamesSuspended();
	}
	inline bool getProfileOnline()
	{
		return EngineLayer::instance()->getGamesOnline();
	}
	inline std::string getProfileNick()
	{
		return EngineLayer::instance()->getGamesNick();
	}
	inline std::string getProfileName()
	{
		return getProfileNick();
	}
	inline std::string getProfileRealName()
	{
		return EngineLayer::instance()->getGamesRealName();
	}
	inline int getProfileId()
	{
		return EngineLayer::instance()->getGamesId();
	}

	std::string getKeyName(int keycode);
	std::string getMouseName(int i);

	namespace Mouse
	{
		enum MouseButton{Mouse1,Mouse2,Mouse3,Mouse4,Mouse5};
	};
	namespace Mouse
	{
		enum MouseButtonNames {MouseLeft,MouseRight,MouseMiddle};
	};

	namespace Key
	{
		enum Key
		{
			A=0,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
			Num0,Num1,Num2,Num3,Num4,Num5,Num6,Num7,Num8,Num9,
			Escape,
			LControl,LShift,LAlt,LSystem,
			RControl,RShift,RAlt,RSystem,
			Menu,
			LBracket,RBracket,
			SemiColon,
			Comma,
			Period,
			Quote,
			Slash,
			BackSlash,
			Tilde,
			Equal,
			Dash,
			Space,
			Enter,
			BackSpace,
			Tab,
			PageUp,PageDown,
			End,Home,
			Insert,
			Delete,
			Add,
			Subtract,
			Multiply,
			Divide,
			Left,Right,Up,Down,
			Numpad0,Numpad1,Numpad2,Numpad3,Numpad4,Numpad5,Numpad6,Numpad7,Numpad8,Numpad9,
			F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12,F13,F14,F15,
			Pause
		};
	};
}
