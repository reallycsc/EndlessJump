#pragma once
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "tinyxml2/tinyxml2.h"

USING_NS_CC;
using namespace cocostudio::timeline;
using namespace ui;
using namespace std;
using namespace tinyxml2;

// flags
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#define LEVEL_MAKER_MODE
	#define SHOW_ALL_ROOM_MODE
#endif

// physics mask  
const int MASK_PLAYER = 1 << 0; // player
const int MASK_ENEMY = 1 << 1; // enemy 