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
//#define DEBUG_MODE

// physics mask  
const int MASK_PLAYER = 1 << 0; // player
const int MASK_ENEMY = 1 << 1; // enemy 