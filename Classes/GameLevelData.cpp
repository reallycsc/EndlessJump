#include "GameLevelData.h"
#include "CommonFunctions.h"

GameLevelData::GameLevelData(void)
{
	m_vRoomsData.clear();
}

GameLevelData::~GameLevelData(void)
{
	m_vRoomsData.clear();
}

GameLevelData* GameLevelData::create(GameLevelData* gamedata)
{	
	GameLevelData* pRet = new(std::nothrow) GameLevelData();
	if (pRet && pRet->init(gamedata))
	{
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool GameLevelData::init(GameLevelData* gamedata)
{
	bool bRet = false;
	do
	{
		if (gamedata)
		{
			m_nLevel = gamedata->getLevel();

			auto inRoomsData = gamedata->getRoomsData();
			for (auto iter = inRoomsData->begin(), endIter = inRoomsData->end(); iter != endIter; ++iter)
			{
				m_vRoomsData.push_back(*iter);
			}
		}
		else
		{
			Size visibleSize = Director::getInstance()->getVisibleSize();

			// level data
			m_nLevel = 1;
		}
		
		bRet = true;
	} while (0);

	return bRet;
}

bool GameLevelData::setRoomDataWithFile(XMLElement* surface)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!surface);
		m_vRoomsData.clear();
		for (XMLElement* surface1 = surface->FirstChildElement("Room"); surface1 != NULL; surface1 = surface1->NextSiblingElement("Room"))
		{
			RoomData room;
			// room
			room.id = surface1->IntAttribute("id");
			room.size = Size(surface1->IntAttribute("width"), surface1->IntAttribute("height"));
			room.position = Point(surface1->IntAttribute("x"), surface1->IntAttribute("y"));
			room.color = Color3B(surface1->IntAttribute("color_r"),
				surface1->IntAttribute("color_g"),
				surface1->IntAttribute("color_b"));
			// player
			XMLElement* surface2 = surface1->FirstChildElement("Player");
			room.player_speed = surface2->FloatAttribute("speed");
			room.player_jumpTime = surface2->FloatAttribute("jumpTime");
			room.player_color = Color3B(surface2->IntAttribute("color_r"),
				surface2->IntAttribute("color_g"),
				surface2->IntAttribute("color_b"));
			// enemys
			surface2 = surface1->FirstChildElement("Enemys");
			room.enemy_color = Color3B(surface2->IntAttribute("color_r"),
				surface2->IntAttribute("color_g"),
				surface2->IntAttribute("color_b"));
			for (XMLElement* surface3 = surface2->FirstChildElement("Enemy"); surface3 != NULL; surface3 = surface3->NextSiblingElement("Enemy"))
			{
				EnemyData enemy;
				enemy.id = surface3->IntAttribute("id");
				enemy.size = Size(surface3->IntAttribute("width"), surface3->IntAttribute("height"));
				enemy.position = Point(surface3->IntAttribute("x"), surface3->IntAttribute("y"));
				room.enemysData.push_back(enemy);
			}
			m_vRoomsData.push_back(room);
		}

		bRet = true;
	} while (false);

	return bRet;
}