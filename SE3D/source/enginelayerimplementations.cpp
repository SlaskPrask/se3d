#include "../include/enginelayer.h"
#include "../include/shader.h"
#include "../include/platformgraphics.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

using namespace _engineprivate;
using namespace _ENGINESPACE;

void(*EngineLayer::gameEndFunc)()=NULL;
void(*EngineLayer::gameStartFunc)()=NULL;
void(*EngineLayer::gameWindowResizeFunc)()=NULL;
void(*EngineLayer::gameFocusGainFunc)()=NULL;
void(*EngineLayer::gameFocusLossFunc)()=NULL;
void(*EngineLayer::gameGamesConnectedFunc)()=NULL;
void(*EngineLayer::gameGamesDisconnectedFunc)()=NULL;
void(*EngineLayer::gameGamesSuspendedFunc)()=NULL;

#ifdef ANDROID
const bool EngineLayer::mobile=1;
#else
const bool EngineLayer::mobile=0;
#endif
const int EngineLayer::DEFAULT_DEPTH=0;
const int EngineLayer::DEFAULT_DEPTH_RANGE=10000;
const unsigned int EngineLayer::DEFAULT_COLLISION_OBJECTS=10;
const int EngineLayer::DEFAULT_COLLISION_SIZE=1024;
const int EngineLayer::DEFAULT_COLLISION_DEPTH=5;
#ifndef ANDROID
const int EngineLayer::MAX_KEYBOARD_KEYS=sf::Keyboard::Key::KeyCount;
#else
const int EngineLayer::MAX_KEYBOARD_KEYS=1;
#endif

EngineLayer::EngineLayer()
{
	Log::log("Engine","Started");
	{
		std::string verstr="Version: ";
		verstr+=ENGINEVERSION;
		#ifdef DEBUG
		#ifdef STANDALONE
		verstr+="S";
		#endif
		#ifdef MOBILE
		verstr+="M";
		#endif
		verstr+="D";
		#endif
		verstr+=" ";
		#ifdef WINDOWS
		verstr+="Windows ";
		#endif
		#ifdef ANDROID
		verstr+="Android ";
		#endif
		#ifdef LINUX
		verstr+="Linux ";
		#endif
		#ifdef MAC
		verstr+="Mac ";
		#endif
		#ifdef UNIX
		verstr+="(Unix) ";
		#endif
		#ifdef DEBUG
		verstr+=_DEFSTR(_ENGINESPACE);
		verstr+="::";
		verstr+=_DEFSTR(_ENGINECLASS);
		#endif
		Log::log("Engine",verstr);
	}

	#ifndef ANDROID
	if (!Log::isPrinting())
	Log::notify("Engine","Unable to write log file");
	#endif

	pullResources=0;

	//graphics
	frameBuffer=frameBufferTexture=frameBufferDepth=0;
	drawBuffers[0]=GL_COLOR_ATTACHMENT0;
	internalWidth=internalHeight=0;
	userResolution=0;
	internalTexData[0]=(GLfloat)0;
	internalTexData[1]=(GLfloat)0;
	internalTexData[2]=(GLfloat)1;
	internalTexData[3]=(GLfloat)0;
	internalTexData[4]=(GLfloat)0;
	internalTexData[5]=(GLfloat)1;
	internalTexData[6]=(GLfloat)1;
	internalTexData[7]=(GLfloat)1;
	screenoff=0;
	defaultTexture=0;
	fps=60;
	defaultCamera=new Camera();
	activeCamera=defaultCamera;
	queueCamera=0;
	reqWidth=reqHeight=width=height=1;
	orientation=2;
	#ifndef ANDROID
	windowType=-1;
	defaultResolutionWidth=sf::VideoMode::getDesktopMode().width;
	defaultResolutionHeight=sf::VideoMode::getDesktopMode().height;
	#else
	windowType=0;
	defaultResolutionWidth=0;
	defaultResolutionHeight=0;
	#endif
	r=g=b=0;
	squareData[0]=0;
	squareData[1]=0;
	squareData[2]=1;
	squareData[3]=0;
	squareData[4]=0;
	squareData[5]=1;
	squareData[6]=1;
	squareData[7]=1;
	texData[0]=0;
	texData[1]=0;
	texData[2]=1;
	texData[3]=0;
	texData[4]=0;
	texData[5]=1;
	texData[6]=1;
	texData[7]=1;
	widthRatio=heightRatio=1;
	dominantRatio=0;
	resDirectory="";
	#ifndef ANDROID
	workDirectory="./";
	#else
	workDirectory="";
	#endif
	
	GLfloat vnear=1.0f,vfar=-1.0f,vtop=0.0f,vleft=0.0f,vright=(GLfloat)width,vbottom=(GLfloat)height;
	ortho[0]=2.0f/(vright-vleft);
	ortho[1]=0;
	ortho[2]=0;
	ortho[3]=-(vright+vleft)/(vright-vleft);
	ortho[4]=0;
	ortho[5]=2.0f/(vtop-vbottom);
	ortho[6]=0;
	ortho[7]=-(vtop+vbottom)/(vtop-vbottom);
	ortho[8]=0;
	ortho[9]=0;
	ortho[10]=-2.0f/(vfar-vnear);
	ortho[11]=-(vfar+vnear)/(vfar-vnear);
	ortho[12]=0;
	ortho[13]=0;
	ortho[14]=0;
	ortho[15]=1;
	rot[0]=(GLfloat)cos(0);
	rot[1]=-(GLfloat)sin(0);
	rot[2]=0;
	rot[3]=0;
	rot[4]=(GLfloat)sin(0);
	rot[5]=(GLfloat)cos(0);
	rot[6]=0;
	rot[7]=0;
	rot[8]=0;
	rot[9]=0;
	rot[10]=1;
	rot[11]=0;
	rot[12]=0;
	rot[13]=0;
	rot[14]=0;
	rot[15]=1;
	textured=0;
	
	posTrans[0]=posTrans[1]=0;
	colorSet[0]=colorSet[1]=colorSet[2]=colorSet[3]=1;
	colorizationSet[0]=colorizationSet[1]=colorizationSet[2]=colorizationSet[3]=0;
	halfsize[0]=halfsize[1]=0;

	#ifndef ANDROID
	settings.depthBits=24;
	settings.stencilBits=8;
	settings.antialiasingLevel=4;
	settings.majorVersion=3;
	settings.minorVersion=0;//3
	resolutions=sf::VideoMode::getFullscreenModes();

	#ifdef DEBUG
	if (resolutions.empty())
	Log::notify("Engine","0 resolutions available");
	else
	Log::log("Engine",std::string(to_string(resolutions.size()))+" resolutions available");
	#endif
	window=NULL;
	fontLib=new FT_Library();
	if (FT_Init_FreeType(fontLib))
	{
		Log::error("Engine","Unable to initialize fonts");
		fontLib=0;
	}
	#ifdef DEBUG
	else
	Log::log("Engine","Fonts initialized");
	#endif
	#endif
	vsync=1;
	title=_DEFAULT_TITLE;
	iconfile="";
	cursorOn=1;
	
	//engine
	masterCollision=new Collision(0,0,DEFAULT_COLLISION_SIZE,DEFAULT_COLLISION_SIZE,0,NULL);
	forcedRelease=0;
	firstDepth=0;
	scene=0;
	switchingScenes=0;
	frameTime=1;
	maxFrameTime=1;
	frameMultiplier=1;
	#ifdef DEBUG
	debugTimeScale=1;
	debugTimeStep=0;
	#endif
	handleObjDeleted=0;
	started=0;
	running=1;
	stopped=0;
	backButton=0;
	windowResized=0;
	focusGained=focusLost=0;
	adChange=0;
	adSize=0;
	statusHeight=0;
	gamesConnected=gamesSuspended=gamesDisconnected=0;
	#ifdef ANDROID
	screenw=screenh=1;
	#else
	sf::VideoMode vm=sf::VideoMode::getDesktopMode();
	state.screenw=screenw=vm.width;
	state.screenh=screenh=vm.height;//TODO: UPDATE IN RES
	#endif
	deviceScale=1;
	resumed=paused=0;
	textsubmit=0;
	focus=1;
	keyboardinput="";
	srand((unsigned int)time(NULL));
	passedTime=0;

	key=new int[MAX_KEYBOARD_KEYS];
	keyboardstr="";
	touchClick=0;
	touchTranslation=0;
	touchNoPosition=0;
	touchNoPositionX=-1;
	touchNoPositionY=-1;
	anykeycount=0;
	anykeyreleased=anykeypressed=0;
	anykeyheld=0;

	for (int i=0;i<MAX_KEYBOARD_KEYS;i++)
	key[i]=0;

	for(int i=0;i<_MAX_MOUSES;i++)
	{
		mouseX[i]=mouseY[i]=0;
		mouseTX[i]=mouseTY[i]=0;
		mousePresses[i]=-1;
		mouseWheelUp[i]=0;
		mouseWheelDown[i]=0;
		for(int j=0;j<_MAX_MOUSE_BUTTONS;j++)
		{
			mouseState[i][j]=MouseEvent::Unheld;
			mouseBufferRelease[i][j]=0;
		}
	}

	//debug
	#ifdef DEBUG
	debugShowCollision=0;
	debugShowTouchables=0;
	#endif

	Log::log("Engine","Initialized");
}

bool EngineLayer::checkTextures()
{
	return (glIsTexture(defaultTexture)!=0);
}

void EngineLayer::updateDefaultTexture()
{
	if (!checkTextures())
	{
		GLubyte data[16*16*4]={0xFF};
		for(int i=0;i<16;i++)
		for (int j=0;j<16;j++)
		{
			if ((j<8&&i<8)||(j>=8&&i>=8))
			{
				data[(i+j*16)*4+0]=0x00;
				data[(i+j*16)*4+1]=0x00;
				data[(i+j*16)*4+2]=0x00;
				data[(i+j*16)*4+3]=0xFF;
			}
		}
		_engine::generateTexture(&defaultTexture,16,16,data,GL_RGBA,1);

		frameBuffer=frameBufferTexture=frameBufferDepth=0;//ignore delete
		setupInternalResolution(userResolution,internalWidth,internalHeight);
	}
}

void EngineLayer::setupInternalResolution(bool enable,unsigned int w,unsigned int h)
{
	queueCamera=1;
	userResolution=enable;

	if (!userResolution)
	{
		if (((double)getCameraW()/(double)getCameraH())>((double)width/(double)height))//window is taller than view
		{
			w=width;
			h=(unsigned int)round((double)width/getCameraW()*getCameraH());
		}
		else
		{
			h=height;
			w=(unsigned int)round((double)height/getCameraH()*getCameraW());
		}
	}

	if ((w==internalWidth&&h==internalHeight)&&(frameBuffer&&frameBufferTexture&&frameBufferDepth))
	return;

	if (frameBuffer)
	{
		glDeleteFramebuffers(1,&frameBuffer);
		frameBuffer=0;
		EngineLayer::printGLErrors("Clear internal resolution buffer");
	}
	if (frameBufferTexture)
	{
		glDeleteTextures(1,&frameBufferTexture);
		frameBufferTexture=0;
		EngineLayer::printGLErrors("Clear internal resolution texture");
	}
	if (frameBufferDepth)
	{
		glDeleteRenderbuffers(1,&frameBufferDepth);
		frameBufferDepth=0;
		EngineLayer::printGLErrors("Clear internal resolution depth");
	}
	
	internalWidth=w;
	internalHeight=h;

	glGenFramebuffers(1,&frameBuffer);
	EngineLayer::printGLErrors("Internal resolution buffer creation");
	glBindFramebuffer(GL_FRAMEBUFFER,frameBuffer);
	EngineLayer::printGLErrors("Internal resolution buffer bind");
		
	glGenRenderbuffers(1,&frameBufferDepth);
	EngineLayer::printGLErrors("Internal resolution depth creation");
	glBindRenderbuffer(GL_RENDERBUFFER,frameBufferDepth);
	EngineLayer::printGLErrors("Internal resolution depth bind");
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,internalWidth,internalHeight);
	EngineLayer::printGLErrors("Internal resolution depth store");
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,frameBufferDepth);
	EngineLayer::printGLErrors("Internal resolution depth");

	_engine::generateTexture(&frameBufferTexture,internalWidth,internalHeight,NULL,GL_RGBA,0);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
	{
		Log::error("Graphics",std::string("Failed to setup internal resolution (")+to_string(internalWidth)+","+to_string(internalHeight)+") "+(userResolution?"on demand":"automatically"));
	}
}

int EngineLayer::runGame(int argc,char* argv[])
{
	#ifndef ANDROID
	instance()->state.windowResized=1;
	if (argc>0)
	{
		instance()->workDirectory=getDirectory(argv[0]);
		if (workDirectory=="")
		workDirectory="./";
		DEBUGFUNC(Log::log("Engine",std::string("Working directory: ")+workDirectory));
		for (int i=1;i<argc-1;i++)
		instance()->launchParameter(argv[i]);
	}
	DEBUGFUNC(Log::log("Engine","Game starting"));
	run(1);
	#endif
	return 0;
}

void EngineLayer::launchParameter(char* str)
{
	DEBUGFUNC(Log::log("Engine",std::string("Launch parameter: ")+str));
	//TODO: implement
}

std::string EngineLayer::getDirectory(const std::string &file)
{
	unsigned int pos=file.find_last_of("/\\");
	if (pos!=std::string::npos)
	return file.substr(0,pos)+"/";
	else
	return "";
}

std::string EngineLayer::resourceDirectory(const std::string &file)
{
	return std::string(resDirectory+file);
}

std::string EngineLayer::workingDirectory(const std::string &file)
{
	return std::string(workDirectory+file);
}

void EngineLayer::setResourceDirectory(const std::string &str)
{
	//disabled for android
	#ifndef ANDROID
	resDirectory=workDirectory;
	resDirectory+=str;
	resDirectory+='/';
	DEBUGFUNC(Log::log("Engine",std::string("Resource directory changed to: ")+resDirectory));
	#else
	DEBUGFUNC(Log::notify("Engine","Resource directory not changed"));
	#endif
}

void EngineLayer::gamesDisconnect()
{
	games.reset();
}

void EngineLayer::gamesSuspend()
{
	games.online=0;
}

void EngineLayer::gamesProfile(int id,std::string nick,std::string realname)
{
	games.online=1;

	games.id=id;
	games.nick=nick;
	games.realname=realname;
}

void EngineLayer::addKeyboardChar(std::string *s,bool newlines,unsigned int valuelimit)
{
	for (unsigned int i=0;i<keyboardstr.size();i++)
	{
		//unsigned int value=keyboardstr[i]<0?256-keyboardstr[i]:keyboardstr[i];
		int value=keyboardstr[i];
		if (value<512)//TODO hardcoded value limit
		{
			if (value<32&&value>=0)//special characters
			switch (value)
			{
				default:
					break;
				case '\n'://newline
				case '\r'://carriage return
					if (newlines)
					if (s->length()<valuelimit)
					*s+='\n';
					break;
				case '\b'://backspace
					if (!s->empty())
					*s=s->substr(0,s->size() - 1);
					break;
			}
			else
			switch (value)
			{
				case 127://delete
					break;
				default:
					if (s->length()<valuelimit)
					*s+=value;
					break;
			}
		}
	}
}

void EngineLayer::setRotation(GLfloat angle,GLfloat w,GLfloat h)
{
	halfsize[0]=w/2.0f;
	halfsize[1]=h/2.0f;
	angle=(GLfloat)(angle*M_PI/180.0f);
	rot[0]=(GLfloat)cos(angle);
	rot[1]=-(GLfloat)sin(angle);
	rot[4]=(GLfloat)sin(angle);
	rot[5]=(GLfloat)cos(angle);
	glUniformMatrix4fv(drawRotation,1,0,rot);
	glUniform2fv(drawHalfSize,1,halfsize);
}

void EngineLayer::setOrtho(GLfloat left,GLfloat top,GLfloat right,GLfloat bottom)
{
	ortho[0]=2.0f/(right-left);
	ortho[3]=-(right+left)/(right-left);
	ortho[5]=2.0f/(top-bottom);
	ortho[7]=-(top+bottom)/(top-bottom);
}

void EngineLayer::closeGraphics()
{
	#ifdef ANDROID
	return;
	#else
	if (!window)
	return;
	DEBUGFUNC(Log::log("Graphics","Closing graphics"));
	window->close();
	delete window;
	window=0;
	printGLErrors("Closing");
	#endif
}

void EngineLayer::setOrientation(int orientation)
{
	this->orientation=orientation;
	CallbackSetOrientation(orientation);
}

bool EngineLayer::setFullscreen()
{
	if (setFullscreen(defaultResolutionWidth,defaultResolutionHeight))
	{
		windowType=2;
		return 1;
	}
	else
	return 0;
}

bool EngineLayer::setWindowed()
{
	#ifdef ANDROID
	if (CallbackUnsetFullscreen())
	{
		windowType=0;
		return 1;
	}
	else
	return 0;
	#endif
	return 0;
}

bool EngineLayer::setFullscreen(int w,int h)
{
	reqWidth=w;
	reqHeight=h;
	#ifdef ANDROID
	if (CallbackSetFullscreen())
	{
		windowType=2;
		return 1;
	}
	else
	return 0;
	#else
	bool valid=0;
	for (unsigned int i=0;i<resolutions.size();i++)
	if (resolutions[i].width==w&&resolutions[i].height==h)
	{
		valid=1;
		break;
	}

	if (!valid)
	{
		Log::notify("Engine",std::string("Unable to go fullscreen, incompatible resolution ")+to_string(w)+","+to_string(h)+"g iven");
		return 0;
	}

	closeGraphics();

	#ifdef MAC
	window = new sf::RenderWindow(sf::VideoMode(w,h),getTitle(),sf::Style::Fullscreen);
	#else
	window = new sf::RenderWindow(sf::VideoMode(w,h),getTitle(),sf::Style::Fullscreen,settings);
	#endif
	printGLErrors("Window creation");

	//TODO: state.deviceScale
	if (!window)
	{
		Log::error("Graphics","Unable to go fullscreen, window failed");
		windowType=-1;
		return 0;
	}
	else
	{
		Log::log("Graphics",std::string("Changed to fullscreen ")+to_string(w)+","+to_string(h));
		if (iconfile!="")
		CallbackSetIcon(iconfile);
		setSize(w,h);
		windowType=2;
		return 1;
	}
	#endif
}

bool EngineLayer::setWindowed(int w,int h)
{
	reqWidth=w;
	reqHeight=h;
	#ifdef ANDROID
	if (CallbackUnsetFullscreen())
	{
		windowType=0;
		return 1;
	}
	else
	return 0;
	#else
	closeGraphics();

	#ifdef MAC
	window = new sf::RenderWindow(sf::VideoMode(w,h),getTitle(),sf::Style::Default);
	#else
	window = new sf::RenderWindow(sf::VideoMode(w,h),getTitle(),sf::Style::Default,settings);
	#endif
	printGLErrors("Window creation");

	//TODO: state.deviceScale
	if (!window)
	{
		Log::error("Graphics","Unable to go windowed, window failed");
		windowType=-1;
		return 0;
	}
	else
	{
		Log::log("Graphics",std::string("Changed to windowed ")+to_string(w)+","+to_string(h));
		if (iconfile!="")
		CallbackSetIcon(iconfile);
		setSize(w,h);
		windowType=0;
		return 1;
	}
	#endif
}

bool EngineLayer::setFullscreenWindowed()
{
	reqWidth=defaultResolutionWidth;
	reqHeight=defaultResolutionHeight;
	#ifdef ANDROID
	if (CallbackSetFullscreen())
	{
		windowType=1;
		return 1;
	}
	else
	return 0;
	#else
	

	closeGraphics();

	#ifdef MAC
	window = new sf::RenderWindow(sf::VideoMode(w,h),getTitle(),sf::Style::None);
	#else
	window = new sf::RenderWindow(sf::VideoMode(defaultResolutionWidth,defaultResolutionHeight),getTitle(),sf::Style::None,settings);
	#endif
	printGLErrors("Window creation");

	window->setPosition(sf::Vector2i(0,0));

	//TODO: state.deviceScale
	if (!window)
	{
		Log::error("Graphics","Unable to go fullscreen windowed, window failed");
		windowType=-1;
		return 0;
	}
	else
	{
		Log::log("Graphics",std::string("Changed to fullscreen windowed ")+to_string(defaultResolutionWidth)+","+to_string(defaultResolutionHeight));
		if (iconfile!="")
		CallbackSetIcon(iconfile);
		setSize(defaultResolutionWidth,defaultResolutionHeight);
		windowType=1;
		return 1;
	}
	#endif
}

void EngineLayer::pushLoaderData(GLuint *destination,int texwidth,int texheight,GLubyte *data,GLenum type,bool smooth)
{
	EngineLayer *i=instance();

	i->loadlock.lock();
	i->loaderdata.push_back(LoaderData(destination,texwidth,texheight,data,type,smooth));
	i->pullResources=1;
	i->loadlock.unlock();
}

void EngineLayer::loaderLoad()
{
	if (!loader.load())
	{
		Log::error("Engine","Loader thread failed.");
		loader.forceLoad();
	}
}

void EngineLayer::reapplyGL()
{
	#ifndef ANDROID
	if (window)
	{
		setVSync(vsync);
		setFPS(fps);
		setMouseCursor(cursorOn);

		window->setActive();
	}
	else
	Log::error("Graphics",std::string("Setting graphics without a window "));
	#else
	setVSync(vsync);
	setFPS(fps);
	setMouseCursor(cursorOn);

	switch (windowType)
	{
		default:
			break;
		case 0:
			setWindowed();
			break;
		case 1:
			setFullscreenWindowed();
			break;
		case 2:
			setFullscreen();
			break;
	}
	#endif
	CallbackSetOrientation(orientation);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	DEBUGFUNC(printGLErrors("GL Alpha"));
	glEnable(GL_BLEND);
	DEBUGFUNC(printGLErrors("GL Blend"));
	printGLErrors("GL settings");
}

void EngineLayer::initGL()
{
	#ifndef ANDROID
	glewExperimental=TRUE;
	GLenum result=glewInit();
	if (result!=GLEW_OK)
	{
		Log::error("Graphics",std::string("Unable initialize GLEW ")+(char*)glewGetString(GLEW_VERSION)+": "+(const char*)glewGetErrorString(result));
	}

	#ifdef DEBUG
	else
		Log::log("Graphics",std::string("Initialized GLEW ")+(const char*)glewGetString(GLEW_VERSION));
	#endif
	printGLErrors("Glew initialization");
	#endif
	
	updateDefaultTexture();
	
	drawProgram=glProgram(VERTEX_SHADER,FRAGMENT_SHADER);
	printGLErrors("GL program");
	drawTrans=glGetUniformLocation(drawProgram,"ftrans");
	printGLErrors("GL var ftrans");
	drawOffset=glGetUniformLocation(drawProgram,"foffset");
	printGLErrors("GL var foffset");
	drawProjection=glGetUniformLocation(drawProgram,"fprojection");
	printGLErrors("GL var fprojection");
	drawRotation=glGetUniformLocation(drawProgram,"frotation");
	printGLErrors("GL var frotation");
	drawColor=glGetUniformLocation(drawProgram,"fcolor");
	printGLErrors("GL var fcolor");
	drawTex=glGetUniformLocation(drawProgram,"ftex");
	printGLErrors("GL var ftex");
	drawColorization=glGetUniformLocation(drawProgram,"fcolorization");
	printGLErrors("GL var fcolorization");
	drawTextured=glGetUniformLocation(drawProgram,"ftextured");
	printGLErrors("GL var ftextured");
	drawHalfSize=glGetUniformLocation(drawProgram,"fhalfsize");
	printGLErrors("GL var fhalfsize");

	glViewport(0,0,internalWidth,internalHeight);
	printGLErrors("GL viewport");
	
	internalProgram=glProgram(VERTEX_SHADER_INTERNAL_OUT,FRAGMENT_SHADER_INTERNAL_OUT);
	printGLErrors("GL internal out program");
	internalDrawTex=glGetUniformLocation(internalProgram,"ftex");
	printGLErrors("GL internal out var ftex");
}

GLuint EngineLayer::glProgram(const char* vShader,const char* fShader)
{
	GLuint vertexShader=glShader(vShader,GL_VERTEX_SHADER);
	if (!vertexShader)
	Log::error("Graphics","Unable create vertex shader");
	printGLErrors("Vertex shader");
	
	GLuint fragmentShader=glShader(fShader,GL_FRAGMENT_SHADER);
	if (!vertexShader)
	Log::error("Graphics","Unable create fragment shader");
	
	GLuint program=glCreateProgram();
	GLint success=0;

	glAttachShader(program,vertexShader);
	glAttachShader(program,fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program,GL_LINK_STATUS,&success);

	if (success==GL_FALSE)
	{
		GLint len=0;
		glGetProgramiv(program,GL_INFO_LOG_LENGTH,&len);
		GLchar *log=new GLchar[len+1];
		glGetProgramInfoLog(program,len,NULL,log);

		Log::error("Graphics",std::string("Program link error: ")+log);
		delete[] log;
		return 0;
	}

	return program;
}

GLuint EngineLayer::glShader(const char* str,GLenum type)
{
	GLuint shader=glCreateShader(type);
	GLint success=0;
	
	int l[1]={(int)(std::string(str).length())};
	glShaderSource(shader,1,&str,l);
	printGLErrors("GL shader source");
	glCompileShader(shader);
	printGLErrors("GL shader compile");

	glGetShaderiv(shader,GL_COMPILE_STATUS,&success);

	if (success==GL_FALSE)
	{
		GLint len=0;
		glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&len);
		GLchar *log=new GLchar[len+1];
		glGetShaderInfoLog(shader,len,NULL,log);

		Log::error("Graphics",std::string("Shader compiler error: ")+log);
		delete[] log;
		return 0;
	}

	return shader;
}

Camera* EngineLayer::getCamera()
{
	return (activeCamera==defaultCamera?NULL:activeCamera);
}

void EngineLayer::setCamera(Camera *cam)
{
	if (cam!=activeCamera)
	{
		refreshCamera();
		updateRatios();
		if (cam==NULL)
		activeCamera=defaultCamera;
		else
		activeCamera=cam;
		
		if (!userResolution)
		setupInternalResolution(0);
	}
}

void EngineLayer::setSize(int w,int h)
{
	width=w;
	height=h;
	defaultCamera->setSize(w,h);
	
	updateRatios();
	
	refreshCamera();

	if (!userResolution)
	setupInternalResolution(0);

	reapplyGL();
}

void EngineLayer::updateRatios()
{
	widthRatio=(double)width/activeCamera->getWidth();
	heightRatio=(double)height/activeCamera->getHeight();
	
	dominantRatio=(activeCamera->getWidth()/activeCamera->getHeight()>(double)width/(double)height);
}

void EngineLayer::setRenderSize()
{
	queueCamera=0;
	
	DEBUGFUNC(printGLErrors("GL viewport size"));
	
	setOrtho(0,0,(GLfloat)activeCamera->getWidth(),(GLfloat)activeCamera->getHeight());

	if (((double)internalWidth/(double)internalHeight)>((double)width/(double)height))
	{
		internalSquareData[2]=internalSquareData[6]=1;
		internalSquareData[5]=internalSquareData[7]=(GLfloat)(((double)internalHeight/(double)internalWidth)/((double)height/(double)width));
	}
	else
	{
		internalSquareData[2]=internalSquareData[6]=(GLfloat)(((double)internalWidth/(double)internalHeight)/((double)width/(double)height));
		internalSquareData[5]=internalSquareData[7]=1;
	}

	internalSquareData[0]=internalSquareData[4]=-internalSquareData[6];
	internalSquareData[1]=internalSquareData[3]=-internalSquareData[7];
}

double EngineLayer::getCameraX()
{
	return (activeCamera?activeCamera->x:defaultCamera->x);
}
double EngineLayer::getCameraY()
{
	return (activeCamera?activeCamera->y:defaultCamera->y);
}
double EngineLayer::getCameraW()
{
	return (activeCamera?activeCamera->getWidth():defaultCamera->getWidth());
}
double EngineLayer::getCameraH()
{
	return (activeCamera?activeCamera->getHeight():defaultCamera->getHeight());
}

double EngineLayer::getKeyboardSize()
{
	double move=-screenoff/(double)height*getCameraH()/getVerRatio();
	/*if (verBars*2<move)
	move-=verBars;
	else
	if (move>=verBars)
	move=verBars;*/

	return move;
}

void EngineLayer::drawBegin()
{
	if (queueCamera)
	{
		if (!userResolution)
		setupInternalResolution(0);
		setRenderSize();
	}

	posOffset[0]=(GLfloat)activeCamera->x;
	posOffset[1]=(GLfloat)activeCamera->y;
	halfsize[0]=halfsize[1]=0;
	
	/*FRAME BUFFER*/
	glBindFramebuffer(GL_FRAMEBUFFER,frameBuffer);
	DEBUGFUNC(printGLErrors("Draw bind internal frame buffer"));
	glClearColor(r,g,b,0.0f);//transparent buffer
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	DEBUGFUNC(printGLErrors("Draw buffer clear"));

	glUseProgram(drawProgram);
	DEBUGFUNC(printGLErrors("Draw program"));
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,squareData);
	DEBUGFUNC(printGLErrors("Draw square"));
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,texData);
	DEBUGFUNC(printGLErrors("Draw data"));
	glUniform2fv(drawTrans,1,posTrans);
	DEBUGFUNC(printGLErrors("Draw position"));
	glUniform2fv(drawOffset,1,posOffset);
	DEBUGFUNC(printGLErrors("Draw offset"));
	glUniform2fv(drawHalfSize,1,halfsize);
	DEBUGFUNC(printGLErrors("Draw halfsize"));
	glUniform1i(drawTex,0);
	DEBUGFUNC(printGLErrors("Draw tex"));
	glUniformMatrix4fv(drawProjection,1,0,ortho);
	DEBUGFUNC(printGLErrors("Draw ortho"));
	glUniformMatrix4fv(drawRotation,1,0,rot);
	DEBUGFUNC(printGLErrors("Draw rotation"));
	glEnableVertexAttribArray(0);
	DEBUGFUNC(printGLErrors("Draw attrib 1"));
	glEnableVertexAttribArray(1);
	DEBUGFUNC(printGLErrors("Draw attrib 2"));
	printGLErrors("Draw begin");
	
	#ifdef ANDROID
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,frameBufferTexture,0);//shader index 0
	#else
	glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,frameBufferTexture,0);//shader index 0
	#endif
	EngineLayer::printGLErrors("Draw begin resolution buffer");
	#ifndef ANDROID
	glDrawBuffers(1,drawBuffers);
	#endif
	EngineLayer::printGLErrors("Draw begin resolution draw buffer");
	
	glViewport(0,0,internalWidth,internalHeight);
}

void EngineLayer::drawEnd()
{
	glViewport(0,0,width,height);
		
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	DEBUGFUNC(printGLErrors("Draw internal remove buffer"));
		
	glUseProgram(internalProgram);
	DEBUGFUNC(printGLErrors("Draw internal program"));
		
	glClearColor(r,g,b,1.0f);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	
	GLfloat move=screenoff/(GLfloat)height*2.0f; //(*2 because range -1 to 1)
	internalSquareData[1]+=move;
	internalSquareData[3]+=move;
	internalSquareData[5]+=move;
	internalSquareData[7]+=move;
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,internalSquareData);
	DEBUGFUNC(printGLErrors("Draw internal square"));
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,internalTexData);
	DEBUGFUNC(printGLErrors("Draw internal data"));
		
	printGLErrors("Draw internal frame buffer1");
	glActiveTexture(GL_TEXTURE0);
	printGLErrors("Draw internal frame buffer2");
	glBindTexture(GL_TEXTURE_2D,frameBufferTexture);
	printGLErrors("Draw internal frame buffer3");
	glUniform1i(internalDrawTex,0);
	printGLErrors("Draw internal frame buffer4");

	glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	printGLErrors("Draw internal frame buffer");
	internalSquareData[1]-=move;
	internalSquareData[3]-=move;
	internalSquareData[5]-=move;
	internalSquareData[7]-=move;

	#ifndef ANDROID
	if (window)
	window->display();

	printGLErrors("Draw end");
	#endif
}

void EngineLayer::printGLErrors(std::string pos)
{
	#ifdef DEBUG
	pos+=" error: ";
	GLenum result=glGetError();
	if (result!=GL_NO_ERROR)
	{
		#ifdef ANDROID
		//todo ERRORSTR
		Log::error("Graphics",pos+to_string(result));
		#else
		Log::error("Graphics",pos+(const char*)gluErrorString(result));
		#endif
		result=glGetError();
	}
	#else
	GLenum result=glGetError();
	if (result!=GL_NO_ERROR)
	#ifdef ANDROID
	//todo ERRORSTR
	Log::error("Graphics",pos+to_string(result));
	#else
	Log::error("Graphics",pos+(const char*)gluErrorString(result));
	#endif
	#endif
}

void EngineLayer::reloadSprites()
{
	DEBUGFUNC(Log::log("Engine","Reloading sprites"));
	for(std::vector<Sprite*>::iterator it=loadedSprites.begin();it!=loadedSprites.end();++it)
	(*it)->reload();
}

void EngineLayer::listLoadedSprite(Sprite *s)
{
	std::vector<Sprite*>::iterator it=std::find(loadedSprites.begin(),loadedSprites.end(),s);
	if (it==loadedSprites.end())//optional if
	loadedSprites.push_back(s);
}

void EngineLayer::unlistLoadedSprite(Sprite *s)
{
	std::vector<Sprite*>::iterator it=std::find(loadedSprites.begin(),loadedSprites.end(),s);
	if (it!=loadedSprites.end())
	loadedSprites.erase(it);
}

void EngineLayer::reloadFonts()
{
	DEBUGFUNC(Log::log("Engine","Reloading fonts"));
	for(std::vector<Font*>::iterator it=loadedFonts.begin();it!=loadedFonts.end();++it)
	(*it)->reload();
}

void EngineLayer::listLoadedFont(Font *s)
{
	std::vector<Font*>::iterator it=std::find(loadedFonts.begin(),loadedFonts.end(),s);
	if (it==loadedFonts.end())//optional if
	loadedFonts.push_back(s);
}

void EngineLayer::unlistLoadedFont(Font *s)
{
	std::vector<Font*>::iterator it=std::find(loadedFonts.begin(),loadedFonts.end(),s);
	if (it!=loadedFonts.end())
	loadedFonts.erase(it);
}

double EngineLayer::getScreenRatio()
{
	return dominantRatio?widthRatio:heightRatio;
}

double EngineLayer::getAdHeight()
{
	return adSize*getCameraH();
}

double EngineLayer::getAdWindowHeight()
{
	return adSize*getHeight();
}

void EngineLayer::setColorization(double r,double g,double b)
{
	colorizationSet[0]=(GLfloat)r;
	colorizationSet[1]=(GLfloat)g;
	colorizationSet[2]=(GLfloat)b;
	//colorizationSet[3]=0;
	glUniform4fv(drawColorization,1,colorizationSet);
}

void EngineLayer::drawRectangle(double x,double y,double w,double h,double rot,double r,double g,double b,double a,bool wire)
{
	setRotation((GLfloat)rot,(GLfloat)w,(GLfloat)h);
	
	setColor(r,g,b,a);
	
	textured=0;
	posTrans[0]=(GLfloat)x;
	posTrans[1]=(GLfloat)y;
	squareData[0]=squareData[4]=0;
	squareData[1]=squareData[3]=0;
	squareData[2]=squareData[6]=(GLfloat)w;
	squareData[5]=squareData[7]=(GLfloat)h;
	glUniform2fv(drawTrans,1,posTrans);
	glUniform1i(drawTextured,textured);
	glDrawArrays(wire?GL_LINE_STRIP:GL_TRIANGLE_STRIP,0,4);
	
	restoreColor();
}

void EngineLayer::drawPoly(double x1,double y1,double x2,double y2,double x3,double y3,double rot,double r,double g,double b,double a,bool wire)
{
	setRotation((GLfloat)rot);
	double cx=(x1+x2+x3)/3.0f;
	double cy=(y1+y2+y3)/3.0f;
	
	setColor(r,g,b,a);
	
	textured=0;
	posTrans[0]=(GLfloat)cx;
	posTrans[1]=(GLfloat)cy;
	squareData[0]=(GLfloat)(x1-cx);
	squareData[1]=(GLfloat)(y1-cy);
	squareData[2]=(GLfloat)(x2-cx);
	squareData[3]=(GLfloat)(y2-cy);
	squareData[4]=(GLfloat)(x3-cx);
	squareData[5]=(GLfloat)(y3-cy);
	glUniform2fv(drawTrans,1,posTrans);
	glUniform1i(drawTextured,textured);
	glDrawArrays(wire?GL_LINE_STRIP:GL_TRIANGLE_STRIP,0,3);
	
	restoreColor();
}

void EngineLayer::drawSpriteFinal(Sprite* sprite,double x,double y,double w,double h,double texx1,double texy1,double texx2,double texy2,double texx3,double texy3,double texx4,double texy4,double rot,double r,double g,double b,double a)
{
	setRotation((GLfloat)rot,(GLfloat)w,(GLfloat)h);
	
	setColor(r,g,b,a);
	
	textured=1;
	posTrans[0]=(GLfloat)x;
	posTrans[1]=(GLfloat)y;
	squareData[0]=squareData[4]=0;
	squareData[1]=squareData[3]=0;
	squareData[2]=squareData[6]=(GLfloat)w;
	squareData[5]=squareData[7]=(GLfloat)h;
	texData[0]=(GLfloat)texx1;
	texData[1]=(GLfloat)texy1;
	texData[2]=(GLfloat)texx2;
	texData[3]=(GLfloat)texy2;
	texData[4]=(GLfloat)texx3;
	texData[5]=(GLfloat)texy3;
	texData[6]=(GLfloat)texx4;
	texData[7]=(GLfloat)texy4;
	glActiveTexture(GL_TEXTURE0);
	sprite->bind();
	glUniform1i(drawTex,0);
	glUniform2fv(drawTrans,1,posTrans);
	glUniform1i(drawTextured,textured);
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	
	restoreColor();
}

void EngineLayer::drawText(Font *font,const std::string &str,double x,double y,double size,double lineSpacing,double r,double g,double b,double a,int align)
{
	if (font==NULL)
		return;

	setColor(r,g,b,a);
	setRotation(0);

	GLfloat scale=(GLfloat)font->ratio;
	GLfloat heightmultiplier=(GLfloat)font->yratio;
	GLfloat advscale=(GLfloat)(size/(double)font->size);
	GLfloat xoff=(GLfloat)font->xoff;
	GLfloat yoff=(GLfloat)font->yoff;

	textured=1;
	squareData[0]=squareData[4]=xoff*(GLfloat)advscale;
	squareData[1]=squareData[3]=yoff*(GLfloat)advscale-(GLfloat)(size*scale)*(heightmultiplier-1);
	squareData[2]=squareData[6]=xoff*(GLfloat)advscale+(GLfloat)(size*scale);
	squareData[5]=squareData[7]=yoff*(GLfloat)advscale+(GLfloat)(size*scale);

	std::vector<GLfloat> linew;
	int c=0,lines=0;
	std::stringstream ss(str);
	std::string ln;
	while (std::getline(ss,ln,'\n'))
	{
		GLfloat offset=0;
		int mode=0,modelength=0;
		for (unsigned int i=0;i<ln.size();i++)
		{
			c=ln.at(i)<0?256-ln.at(i):ln.at(i);

			if (modelength>0)
			{
				modelength--;
				if (modelength==0)
					mode=0;
				continue;
			}
			else
				if (c=='\x1b')
				{
					mode=1;
					modelength=6;
					continue;
				}

			if (c<0||(unsigned int)c>=font->characters)
				continue;

			offset+=(GLfloat)font->charw[c];
		}
		linew.push_back(offset*advscale);
		lines++;
	}

	if (lines==0)
		align=0;

	int line=0;
	switch (align)
	{
		default:
		case _FONT_ALIGN_LEFT:
			posTrans[0]=(GLfloat)x;
			break;
		case _FONT_ALIGN_MIDDLE:
			posTrans[0]=(GLfloat)x-linew[0]/2.0f;
			break;
		case _FONT_ALIGN_RIGHT:
			posTrans[0]=(GLfloat)x-linew[0];
			break;
	}
	posTrans[1]=(GLfloat)(y-size*scale+font->fonth*advscale);

	for(unsigned int i=0;i<str.length();i++)
	{
		c=str.at(i);
		if (c<0)
		c=256+c;//+ because negative

		if (c<0||(unsigned int)c>=font->characters)
		continue;

		if (c=='\n')
		{
			line++;
			switch (align)
			{
				default:
				case _FONT_ALIGN_LEFT:
					posTrans[0]=(GLfloat)x;
					break;
				case _FONT_ALIGN_MIDDLE:
					posTrans[0]=(GLfloat)x-linew[line]/2.0f;
					break;
				case _FONT_ALIGN_RIGHT:
					posTrans[0]=(GLfloat)x-linew[line];
					break;
			}
			posTrans[1]=(GLfloat)(y-size*scale+font->fonth*advscale+line*(font->lineh+lineSpacing)*advscale);
			continue;
		}
		else
		if (c=='\x1b'&&i+6<str.length())
		{
			setColor(hexStrToInt(str.substr(i+1,2))/255.0f,hexStrToInt(str.substr(i+3,2))/255.0f,hexStrToInt(str.substr(i+5,2))/255.0f,a);
			i+=6;
			continue;
		}
		texData[0]=texData[4]=(GLfloat)((c%16)*0.0625);
		texData[1]=texData[3]=(GLfloat)(floor((c%_FONT_SET_CHARACTERS)/16)*0.0625);
		texData[2]=texData[6]=(GLfloat)((c%16+1)*0.0625);
		texData[5]=texData[7]=(GLfloat)(floor((c%_FONT_SET_CHARACTERS)/16+1)*0.0625);
		glActiveTexture(GL_TEXTURE0);

		font->bind((unsigned int)floor((double)c/(double)_FONT_SET_CHARACTERS));

		glUniform1i(drawTex,0);
		glUniform2fv(drawTrans,1,posTrans);
		glUniform1i(drawTextured,textured);
		glDrawArrays(GL_TRIANGLE_STRIP,0,4);
		posTrans[0]+=(GLfloat)(font->charw[c]*advscale);
	}

	restoreColor();
}

void EngineLayer::getTextMetrics(Font *font,const std::string &str,double size,double lineSpacing,double *w,double *h)
{
	if (font==NULL)
	{
		*w=0;
		*h=0;
		return;
	}

	GLfloat scale=(GLfloat)font->ratio;
	GLfloat advscale=(GLfloat)(size/(double)font->size);

	GLfloat textw=0;
	int c=0,lines=0;
	std::stringstream ss(str);
	std::string ln;
	while (std::getline(ss,ln,'\n'))
	{
		GLfloat offset=0;
		int mode=0,modelength=0;
		for (unsigned int i=0;i<ln.size();i++)
		{
			c=ln.at(i)<0?256+ln.at(i):ln.at(i);

			if (modelength>0)
			{
				modelength--;
				if (modelength==0)
					mode=0;
				continue;
			}
			else
			if (c=='\x1b')
			{
				mode=1;
				modelength=6;
				continue;
			}

			if (c<0||(unsigned int)c>=font->characters)
				continue;

			offset+=(GLfloat)font->charw[c];
		}
		if (textw<offset*advscale)
		textw=offset*advscale;
		lines++;
	}

	*h=lines*(font->lineh+lineSpacing)*advscale;
	*w=(double)textw;
}

void EngineLayer::setGameStartFunc(void (*func)())
{
	EngineLayer::gameStartFunc=func;
}

void EngineLayer::setGameEndFunc(void(*func)())
{
	EngineLayer::gameEndFunc=func;
}

void EngineLayer::setGameWindowResizeFunc(void(*func)())
{
	EngineLayer::gameWindowResizeFunc=func;
}

void EngineLayer::setGameFocusGainFunc(void(*func)())
{
	EngineLayer::gameFocusGainFunc=func;
}

void EngineLayer::setGameFocusLossFunc(void(*func)())
{
	EngineLayer::gameFocusLossFunc=func;
}

static void setGameGamesConnectedFunc(void(*func)())
{
	EngineLayer::gameGamesConnectedFunc=func;
}
static void setGameGamesDisconnectedFunc(void(*func)())
{
	EngineLayer::gameGamesDisconnectedFunc=func;
}
static void setGameGamesSuspendedFunc(void(*func)())
{
	EngineLayer::gameGamesSuspendedFunc=func;
}

void EngineLayer::createObject(Object *o)
{
	objects.add(o);
	
	for(std::vector<Tag*>::iterator it=activeTags.begin();it!=activeTags.end();++it)
	o->addTag(*it);
	
	if (scene)
	{
		o->_persistent=0;
		o->_scene=scene;
		tieObjectToScene(scene,o);
	}
}

ResourceSet* EngineLayer::createResourceSet(ResourceSet *set)
{
	set->engine_id=resourcesets.size();
	resourcesets.push_back(set);
	return set;
}

ResourceSet* EngineLayer::resourceSet(unsigned int i)
{
	if (i<0||i>=resourcesets.size())
	return NULL;
	
	return resourcesets[i];
}

void EngineLayer::resolveUnloadQueue()
{
	while(unload_sets.size())
	{
		unload_sets.back()->_unload();
		unload_sets.pop_back();
	}
}

void EngineLayer::resolveDepthQueue()
{
	while(depthQueue.size())
	{
		attachDepth(depthQueue.back()->_copyDepth());
		depthQueue.pop_back();
	}
}

void EngineLayer::queueDepth(Object *o)
{
	std::vector<Object*>::iterator it=std::find(depthQueue.begin(),depthQueue.end(),o);
	if (it==depthQueue.end())
	depthQueue.push_back(o);
}

void EngineLayer::resolveDepthChangeQueue()
{
	while(depthChangeQueue.size())
	{
		depthChangeQueue.back()->_performDepthMove();
		depthChangeQueue.pop_back();
	}
}

void EngineLayer::queueDepthChange(Object *o)
{
	std::vector<Object*>::iterator it=std::find(depthChangeQueue.begin(),depthChangeQueue.end(),o);
	if (it==depthChangeQueue.end())
	depthChangeQueue.push_back(o);
}

void EngineLayer::detachDepth(Object *o)
{
	for(std::vector<Object*>::iterator it=depthQueue.begin();it!=depthQueue.end();++it)
	if (o==(*it))
	{
		depthQueue.erase(it);
		return;
	}

	if (o->_depthItem==firstDepth)
	firstDepth=o->_depthItem->getNext();
	
	delete (o->_depthItem);
}

void EngineLayer::attachDepth(Object *o)
{
	if (!firstDepth)
	firstDepth=new DepthItem(o);
	else
	(new DepthItem(o))->addAbove(firstDepth);
}

void EngineLayer::switchScene(Scene *scn)
{
	switchingScenes=1;
	if (scene)
	{
		scene->_deathMark=1;
		scene->_deleted=1;
		delete scene;
	}
	scene=scn;
}

void EngineLayer::beginTag(Tag *t) 
{
	std::vector<Tag*>::iterator it=std::find(activeTags.begin(),activeTags.end(),t);
	if (it==activeTags.end())
	activeTags.push_back(t);
}
void EngineLayer::endTag(Tag *t)
{
	std::vector<Tag*>::iterator it=std::find(activeTags.begin(),activeTags.end(),t);
	if (it!=activeTags.end())
	activeTags.erase(it);
}

void EngineLayer::removeTouchable(Touchable *t)
{
	std::vector<Touchable*>::iterator it=std::find(touchables.begin(),touchables.end(),t);
	if (it!=touchables.end())
	touchables.erase(it);
}

void EngineLayer::parseTouchables()
{
	if (touchables.empty())
	return;
	
	for(std::vector<Touchable*>::iterator it=touchables.begin();it!=touchables.end();++it)
	{
		(*it)->fixPosition();//ok to have this above if's?
		if ((*it)->pointer==NULL||(*it)->pointer->isRunEnabled())
		if ((*it)->mouse!=-1)
		{
			if (getMouseIdle((*it)->mouse))
			(*it)->hook(-1);
			else
			(*it)->inside=(getMouseTranslatedX((*it)->mouse)>=(*it)->getX()&&getMouseTranslatedX((*it)->mouse)<(*it)->getX()+(*it)->w&&getMouseTranslatedY((*it)->mouse)>=(*it)->getY()&&getMouseTranslatedY((*it)->mouse)<(*it)->getY()+(*it)->h);
		}
	}
	
	int t;
	Touchable *touch=NULL;
	for(int i=0;i<_MAX_MOUSES;i++)
	{
		t=getMousePresses(i);
		if (t==-1)
		break;
		
		for(std::vector<Touchable*>::iterator it=touchables.begin();it!=touchables.end();++it)
		if ((*it)->enabled)
		if (((*it)->mouse==-1)&&(touch==NULL||touch->getDepth()>(*it)->getDepth()))
		if (getMouseTranslatedX(t)>=(*it)->getX()&&getMouseTranslatedX(t)<(*it)->x+(*it)->w&&getMouseTranslatedY(t)>=(*it)->getY()&&getMouseTranslatedY(t)<(*it)->getY()+(*it)->h)
		touch=(*it);
		
		if (touch!=NULL)
		touch->hook(t);
	}
}

void EngineLayer::deleteAllObjects()
{
	DEBUGFUNC(Log::log("Engine","Deleting all objects"));
	LinkedList<Object> *obj=objects.first();
	while(obj)
	{
		((Object*)obj)->_deathMark=1;
		delete obj;
		obj=objects.first();
	}
}

void EngineLayer::deleteAllSets()
{
	DEBUGFUNC(Log::log("Engine","Deleting all resource sets"));
	for (unsigned int i=0;i<resourcesets.size();i++)
	if (resourcesets[i]!=NULL)
	{
		if (resourcesets[i]->loads!=0)
		Log::notify("Engine","Resource set "+to_string(resourcesets[i]->engine_id)+"("+to_string(resourcesets[i])+") left loaded");
		resourcesets[i]->_deathMark=1;
		delete resourcesets[i];
	}
	resourcesets.clear();
}

void EngineLayer::deleteExceptionResourceSet(ResourceSet *set)
{
	for (unsigned int i=0;i<resourcesets.size();i++)
	if (resourcesets[i]==set)
	{
		resourcesets[i]=NULL;
		break;
	}
}

void EngineLayer::objAddTag(Object* o,Tag* t)
{
	if (t->attachObj(o))
	{
		o->_tags.push_back(t);
		o->_refreshTagRuns(t->isRunning());
		o->_refreshTagDraws(t->isDrawing());
	}
}

void EngineLayer::objRemoveTag(Object* o,Tag* t)
{
	if (t->detachObj(o))
	{
		std::vector<Tag*>::iterator it=std::find(o->_tags.begin(),o->_tags.end(),t);
		o->_tags.erase(it);
		o->_refreshTagRuns(1);
		o->_refreshTagDraws(1);
	}
}

void EngineLayer::objUnlinkTag(Object* o,Tag* t)
{
	std::vector<Tag*>::iterator it=std::find(o->_tags.begin(),o->_tags.end(),t);
	o->_tags.erase(it);
	o->_refreshTagRuns(1);
	o->_refreshTagDraws(1);
}

void EngineLayer::untieObjectFromScene(Scene *scn,Object *obj)
{
	std::vector<Object*>::iterator it=std::find(scn->_objects.begin(),scn->_objects.end(),obj);
	scn->_objects.erase(it);
}

void EngineLayer::setMouseCursor(bool enabled)
{
	#ifndef ANDROID
	cursorOn=enabled;
	if (window)
	window->setMouseCursorVisible(enabled);
	#endif
}

void EngineLayer::setVSync(bool enabled)
{
	#ifndef ANDROID
	vsync=enabled;
	if (window)
	window->setVerticalSyncEnabled(enabled);
	#else
	//unsupported
	vsync=1;
	#endif
}

void EngineLayer::setTitle(const std::string &label)
{
	title=label;
	#ifndef ANDROID
	if (window)
	window->setTitle(title);
	#else
	CallbackSetTitle(title);
	#endif
}

void EngineLayer::setIcon(const std::string &file)
{
	iconfile=file;
	#ifndef ANDROID
	if (window)
	#endif
	CallbackSetIcon(resourceDirectory(file));
}

double EngineLayer::strToD(const std::string &str)
{
	double i;
	std::stringstream ss(str);
	ss>>i;
	return i;
}

int EngineLayer::strToI(const std::string &str)
{
	int i;
	std::stringstream ss(str);
	ss>>i;
	return i;
}

EngineLayer::~EngineLayer()
{
	Log::log("Engine","Closing");

	#ifndef ANDROID
	if (window)
	{
		closeGraphics();
	}
	if (fontLib)
	{
		FT_Done_FreeType(*fontLib);
		delete fontLib;
		fontLib=0;
	}
	#endif
	delete defaultCamera;

	#ifndef ANDROID
	DEBUGFUNC(Log::log("Audio","Closing audio"));
	for (std::vector<SoundInstance*>::iterator it=audioSound.begin();it!=audioSound.end();++it)
	{
		if (*it!=NULL)
		delete (*it);
	}
	for (std::vector<SoundStorage*>::iterator it=audioBuffer.begin();it!=audioBuffer.end();++it)
	{
		if (*it!=NULL)
		delete (*it);
	}
	#endif

	delete[] key;
	Log::log("Engine","Finished");
}


#ifdef DEBUG
void EngineLayer::dumpObjects()
{
	Log::log("Debug","OBJECTS:");
	int i=0;
	LinkedList<Object> *obj = objects.first();
	while (obj)
	{
		LinkedList<Object> *nextObj= obj->getNext();
		std::ostringstream s;
		s << to_string(i) << ": " << ((Object*)obj)->objectName() << " (" << (Object*)obj << ")";
		Log::log("Debug",s.str().c_str());
		i++;
		obj = nextObj;
	}

	Log::log("Debug","");
}
void EngineLayer::dumpObjectDepths()
{
	Log::log("Debug","OBJECT DEPTHS:");
	DepthItem *di,*diNext;
	int i=0;
	if (di=firstDepth)
	while (di)
	{
		diNext = di->getNext();
		std::ostringstream s;
		s << to_string(i) << ": " << di << " " << di->get()->objectName();
		s << " (" << di->get() << ":" << di->getDepth() << ") ";
		Log::log("Debug",s.str().c_str());
		i++;
		di=diNext;
	}

	Log::log("Debug","");
}
void EngineLayer::dumpObjects(unsigned int id)
{
	Log::log("Debug","OBJECTS:");
	int i=0;
	LinkedList<Object> *obj=objects.first();
	while (obj)
	if (((Object*)obj)->objectId()==id)
	{
		LinkedList<Object> *nextObj=obj->getNext();
		std::ostringstream s;
		s << to_string(i) << ": " << ((Object*)obj)->objectName() << " (" << (Object*)obj << ")";
		Log::log("Debug",s.str().c_str());
		i++;
		obj=nextObj;
	}

	Log::log("Debug","");
}
void EngineLayer::dumpDepths()
{
	Log::log("Debug","DEPTHS:");
	DepthItem *di,*diNext;
	int i=0;
	if (di=firstDepth)
		while (di)
		{
			diNext=di->getNext();
			std::ostringstream s;
			//i
			s << to_string(i) << ": ";

			//prev
			if (di->getPrevious())
				s << di->getPrevious() << "(" << di->getPrevious()->get() << ":" << di->getPrevious()->getDepth() << ") -> ";
			else
				s << ((Object*)NULL) << "(" << ((Object*)NULL) << ":-) -> ";

			//this
			s << di << "(" << di->get() << ":" << di->getDepth() << ")";

			//next
			if (di->getNext())
				s << " -> " << di->getNext() << "(" << di->getNext()->get() << ":" << di->getNext()->getDepth() << ")";
			else
				s << " -> " << ((Object*)NULL) << "(" << ((Object*)NULL) << ":-)";

			Log::log("Debug",s.str().c_str());
			i++;

			di=diNext;
		}

	Log::log("Debug","");
}
void EngineLayer::dumpTouchables()
{
	Log::log("Debug","TOUCHABLES:");

	int i=0;
	for (std::vector<Touchable*>::iterator it=touchables.begin();it!=touchables.end();++it)
	{
		std::ostringstream s;
		s << to_string(i) << ": " << &(*it) << " (" << (*it)->getDepth() << ")" << " (" << (*it)->getX() << "," << (*it)->getY() << ")[" << (*it)->getWidth() << "," << (*it)->getHeight() << "]";
		if ((*it)->pointer)
		{
			s << " " << ((*it)->pointer) << " " << ((*it)->pointer)->objectName();
			s << " (" << ((*it)->pointer->_depthItem) << ":" << ((*it)->pointer->_depth) << ")";
		}
		if (!(*it)->isEnabled())
		s << " Disabled";
		Log::log("Debug",s.str().c_str());
		i++;
	}

	Log::log("Debug","");
}
void EngineLayer::dumpDepthQueue()
{
	Log::log("Debug","DEPTH QUEUE:");
	for (unsigned int i=0;i<depthQueue.size();i++)
	{
		std::ostringstream s;
		s << to_string(i) << ": " << depthQueue[i] << " " << depthQueue[i]->_depth << "->" << depthQueue[i]->_qdepth;
		Log::log("Debug",s.str().c_str());
		depthQueue[i];
	}

	Log::log("Debug","");
}
void EngineLayer::dumpDepthChangeQueue()
{
	Log::log("Debug","DEPTH CHANGE QUEUE:");
	for (unsigned int i=0;i<depthChangeQueue.size();i++)
	{
		std::ostringstream s;
		s << to_string(i) << ": " << depthChangeQueue[i] << " " << depthChangeQueue[i]->_depth << "->" << depthChangeQueue[i]->_qdepth;
		Log::log("Debug",s.str().c_str());
		depthChangeQueue[i];
	}

	Log::log("Debug","");
}
void EngineLayer::dumpObj(Object *obj)
{
	std::ostringstream s;
	s << "OBJECT: " << obj->objectName() << " (" << obj << ")";
	Log::log("Debug",s.str().c_str());
	s.str("");
	s << "DEPTH OBJ: " << obj->_depthItem;
	Log::log("Debug",s.str().c_str());
	s.str("");
	s << "DEPTH: " << to_string(obj->_depth) << "->" << to_string(obj->_qdepth);
	Log::log("Debug",s.str().c_str());
	s.str("");
	s << "X,Y: " << to_string(obj->x) << "," << to_string(obj->y);
	Log::log("Debug",s.str().c_str());
	s.str("");
	s << (obj->_visible?"VISIBLE":"INVISIBLE");
	Log::log("Debug",s.str().c_str());

	Log::log("Debug","");
}
void EngineLayer::debugToggleCollision()
{
	debugShowCollision=!debugShowCollision;
}
void EngineLayer::debugToggleTouchables()
{
	debugShowTouchables=!debugShowTouchables;
}
void EngineLayer::dumpResources()
{
	std::ostringstream s;
	s << "RESOURCE SETS: " << resourcesets.size();
	Log::log("Debug",s.str().c_str());
	int set=0;
	for (std::vector<ResourceSet*>::iterator it=resourcesets.begin();it!=resourcesets.end();++it)
	{
		s.str("");
		if ((*it)->loads==0)
		s << "SET: " << set << " unloaded";
		else
		s << "SET: " << set << " loaded " << (*it)->loads << ((*it)->loads==1?" time":" times");
		Log::log("Debug",s.str().c_str());

		for (unsigned int i=0;i<(*it)->getSprites();i++)
		{
			s.str("");
			s << "Sprite " << i << " (" << resourceSet(set)->getSprite(i) << ") " << "\"" << resourceSet(set)->getSpriteResource(i)->getFile() << "\"";
			Log::log("Debug",s.str().c_str());
		}

		for (unsigned int i=0;i<(*it)->getSounds();i++)
		{
			s.str("");
			s << "Sound " << i << " (" << resourceSet(set)->getSound(i) << ") "  << "\"" << resourceSet(set)->getSoundResource(i)->getFile() << "\"";
			Log::log("Debug",s.str().c_str());
		}

		for (unsigned int i=0;i<(*it)->getFonts();i++)
		{
			s.str("");
			s << "Font " << i << " (" << resourceSet(set)->getFont(i) << ") "  << "\"" << resourceSet(set)->getFontResource(i)->getFile() << "\"";
			Log::log("Debug",s.str().c_str());
		}

		set++;
	}

	Log::log("Debug","");
}
void EngineLayer::dumpLoadedSounds()
{
	#ifndef ANDROID
	std::ostringstream s;
	s << "LOADED SOUNDS";
	Log::log("Debug",s.str().c_str());

	int i=0;
	for (std::vector<SoundStorage*>::iterator it=audioBuffer.begin();it!=audioBuffer.end();++it)
	{
		s.str("");
		s << i << ": " << (*it)->sound;
		Log::log("Debug",s.str().c_str());

		i++;
	}

	Log::log("Debug","");
	#endif
}
void EngineLayer::dumpPlayingSounds()
{
	#ifndef ANDROID
	std::ostringstream s;
	s << "PLAYING SOUNDS";
	Log::log("Debug",s.str().c_str());

	int i=0;
	for (std::vector<SoundInstance*>::iterator it=audioSound.begin();it!=audioSound.end();++it)
	{
		s.str("");
		if (*it&&(*it)->sound->getStatus()!=sf::SoundSource::Status::Stopped)
		s << i << ": " << (*it)->soundres << " priority: " << (*it)->priority << " age: " << (*it)->age << ((*it)->sound->getStatus()==sf::SoundSource::Status::Paused?" paused":"");
		else
		s << i << ": " << (void*)NULL;
		Log::log("Debug",s.str().c_str());

		i++;
	}

	Log::log("Debug","");
	#endif
}
void EngineLayer::debugHandler()
{
	#ifndef ANDROID
	if (getKeyHeld(sf::Keyboard::Key::LControl)||getKeyHeld(sf::Keyboard::Key::RControl))
	{
		if (getKeyPress(sf::Keyboard::Key::F12))
		dumpObjects();
		if (getKeyPress(sf::Keyboard::Key::F11))
		dumpObjectDepths();
		if (getKeyPress(sf::Keyboard::Key::F10))
		dumpDepths();
		if (getKeyPress(sf::Keyboard::Key::F9))
		dumpTouchables();

		if (getKeyPress(sf::Keyboard::Key::F8))
		debugToggleCollision();
		if (getKeyPress(sf::Keyboard::Key::F7))
		debugToggleTouchables();

		if (getKeyPress(sf::Keyboard::Key::Numpad0))
		{
			if (!(!(debugTimeScale==0.0f)^!(debugTimeStep==0)))
			{
				debugTimeScale=1;
				Log::log("Debug","Time scale: Normal");
			}
			else
			{
				debugTimeScale=0;
				Log::log("Debug","Time scale: Paused");
			}
			debugTimeStep=0;
		}
		if (getKeyPress(sf::Keyboard::Key::Add))
		{
			debugTimeStep++;
			debugTimeScale=debugTimeStep<0?1.0f/(pow(2,-debugTimeStep)):pow(2,debugTimeStep);
			Log::log("Debug",std::string("Time scale: ")+to_string(debugTimeScale));
		}
		if (getKeyPress(sf::Keyboard::Key::Subtract))
		{
			debugTimeStep--;
			debugTimeScale=debugTimeStep<0?1.0f/(pow(2,-debugTimeStep)):pow(2,debugTimeStep);
			Log::log("Debug",std::string("Time scale: ")+to_string(debugTimeScale));
		}
	}
	else
	if (getKeyHeld(sf::Keyboard::Key::LShift)||getKeyHeld(sf::Keyboard::Key::RShift))
	{
		if (getKeyPress(sf::Keyboard::Key::F12))
		dumpDepthChangeQueue();
		if (getKeyPress(sf::Keyboard::Key::F11))
		dumpDepthQueue();

		if (getKeyPress(sf::Keyboard::Key::F5))
		dumpResources();
		if (getKeyPress(sf::Keyboard::Key::F6))
		dumpLoadedSounds();
		if (getKeyPress(sf::Keyboard::Key::F7))
		dumpPlayingSounds();
	}
	#endif
}
#endif