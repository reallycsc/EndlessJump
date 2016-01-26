#pragma once
#include <string>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocostudio::timeline;
using namespace ui;
using namespace std;

#define NS_CSC_BEGIN	namespace CSCClass {
#define NS_CSC_END		}
#define USING_NS_CSC	using namespace CSCClass

const string EVENT_GAMECENTER_AUTHENTICATED = "event_gamecenter_authenticated";
const string EVENT_GAMECENTER_SCORERETRIVED = "event_gamecenter_scoreretrived_";
const string EVENT_PLARERDATA_SCOREUPDATED = "event_playerdata_scoreupdated_";
const string EVENT_PURCHASED = "event_purchased_";
const string EVENT_RESTORED = "event_restored_";