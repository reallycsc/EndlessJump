#include "GameMediator.h"

static GameMediator _sharedContext;

GameMediator* GameMediator::getInstance()
{
	static bool s_bFirstUse = true; 
	if (s_bFirstUse) 
		{ 
		_sharedContext.init(); 
		s_bFirstUse = false; 
		} 
	return &_sharedContext; 
}

GameMediator::GameMediator(void)
{
	m_nGameLevelCount = 0;
	m_nMaxGameLevel = m_nCurGameLevel = 1;
	m_nCurGameRoom = 1;

	m_vGameLevelData.clear();
	m_vLevelMinDeadCount.clear();
}

GameMediator::~GameMediator(void)
{
	m_vGameLevelData.clear();
	m_vLevelMinDeadCount.clear();
}

bool GameMediator::init()
{
	bool bRet = false;
	do
	{
		// get level config file
		CC_BREAK_IF(!this->loadGameLevelFile());

		// get local save data
		UserDefault* user = UserDefault::getInstance();
		if (!user->getBoolForKey("isHaveSaveFileXml"))
		{
			user->setBoolForKey("isHaveSaveFileXml", true);
			user->setIntegerForKey("CurLevel", 1);
			user->setIntegerForKey("MaxLevel", 1);
			for (size_t i = 0; i < m_nGameLevelCount; i++)
			{
				string key = StringUtils::format("Level%d-DeadCount", i + 1);
				user->setIntegerForKey(key.c_str(), -1);
				m_vLevelMinDeadCount.push_back(-1);
			}
		}
		else
		{
			m_nCurGameLevel = MIN(user->getIntegerForKey("CurLevel", 1), m_nGameLevelCount);
			m_nMaxGameLevel = MIN(user->getIntegerForKey("MaxLevel", 1), m_nGameLevelCount);
			for (size_t i = 0; i < m_nGameLevelCount; i++)
			{
				string key = StringUtils::format("Level%d-DeadCount", i + 1);
				int deadCount = user->getIntegerForKey(key.c_str(), -1);
				m_vLevelMinDeadCount.push_back(deadCount);
			}
		}

		bRet = true;
	} while (false);
	
	return bRet;
}

bool GameMediator::loadGameLevelFile()
{
	bool bRet = false;
	do
	{
		tinyxml2::XMLDocument document;
        string filename = FileUtils::getInstance()->fullPathForFilename("config/LevelMake_Workstation.xml");
		document.LoadFile(filename.c_str());
		XMLElement* root = document.RootElement();
		CC_BREAK_IF(!root);

		// load workstation
		m_vGameLevelData.clear();
		m_nGameLevelCount = 0;
		for (XMLElement* surface1 = root->FirstChildElement("Level"); surface1 != NULL; surface1 = surface1->NextSiblingElement("Level"))
		{
			GameLevelData* data = GameLevelData::create();
			data->setLevel(surface1->IntAttribute("level"));
			data->setLevelName(surface1->Attribute("name"));
			data->setMaxDeadTime(surface1->IntAttribute("deadCount"));

			CC_BREAK_IF(!data->setRoomDataWithFile(surface1));

			m_vGameLevelData.push_back(data);
			m_nGameLevelCount++;
		}
		bRet = true;
	} while (false);

	return bRet;
}

bool GameMediator::saveGameLevelFile()
{
	bool bRet = false;
	do
	{
		tinyxml2::XMLDocument *document = new tinyxml2::XMLDocument();
		CC_BREAK_IF(!document);

		XMLDeclaration *declaration = document->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
		CC_BREAK_IF(!declaration);
		document->LinkEndChild(declaration);

		XMLElement *root = document->NewElement("GameLevel");
		document->LinkEndChild(root);

		m_nGameLevelCount = m_vGameLevelData.size();
		for (size_t i = 0; i < m_nGameLevelCount; i++)
		{
			XMLElement *surface1 = document->NewElement("Level");
			surface1->SetAttribute("level", m_vGameLevelData.at(i)->getLevel());
			surface1->SetAttribute("name", m_vGameLevelData.at(i)->getLevelName().c_str());
			surface1->SetAttribute("deadCount", m_vGameLevelData.at(i)->getMaxDeadTime());
			root->LinkEndChild(surface1);

			auto roomsData = m_vGameLevelData.at(i)->getRoomsData();
			for (auto iter2 = roomsData->begin(), preIter = roomsData->begin(), endIter2 = roomsData->end();
			iter2 != endIter2; ++iter2)
			{
				XMLElement *surface2 = document->NewElement("Room");
				surface2->SetAttribute("id", iter2->id);
				surface2->SetAttribute("width", iter2->size.width);
				surface2->SetAttribute("height", iter2->size.height);
				if (iter2 == roomsData->begin())
				{
					surface2->SetAttribute("x", 0);
					surface2->SetAttribute("y", 0);
				}
				else
				{
					surface2->SetAttribute("x", 0);
					surface2->SetAttribute("y", preIter->size.height + preIter->position.y);
				}
				preIter = iter2;
				surface2->SetAttribute("color_r", iter2->color.r);
				surface2->SetAttribute("color_g", iter2->color.g);
				surface2->SetAttribute("color_b", iter2->color.b);
				surface1->LinkEndChild(surface2);

				XMLElement *surface3 = document->NewElement("Player");
				surface3->SetAttribute("speed", iter2->player_speed);
				surface3->SetAttribute("jumpTime", iter2->player_jumpTime);
				// maybe need to change
				surface3->SetAttribute("color_r", iter2->player_color.r);
				surface3->SetAttribute("color_g", iter2->player_color.g);
				surface3->SetAttribute("color_b", iter2->player_color.b);
				surface2->LinkEndChild(surface3);

				surface3 = document->NewElement("Enemys");
				surface3->SetAttribute("color_r", iter2->enemy_color.r);
				surface3->SetAttribute("color_g", iter2->enemy_color.g);
				surface3->SetAttribute("color_b", iter2->enemy_color.b);
				surface2->LinkEndChild(surface3);
				auto enemysData = &(iter2->enemysData);
				for (size_t i1 = 0, length1 = enemysData->size(); i1 < length1; i1++)
				{
					auto enemy_data = enemysData->at(i1);
					XMLElement *surface4 = document->NewElement("Enemy");
					surface4->SetAttribute("id", i1 + 1);
					surface4->SetAttribute("width", enemy_data.size.width);
					surface4->SetAttribute("height", enemy_data.size.height);
					surface4->SetAttribute("x", static_cast<int>(enemy_data.position.x));
					surface4->SetAttribute("y", static_cast<int>(enemy_data.position.y));
					auto actionsData = &enemy_data.actionsData;
					for (size_t i2 = 0, length2 = actionsData->size(); i2 < length2; i2++)
					{
						auto action_data = actionsData->at(i2);
						auto action_type = action_data->getType();
						XMLElement *surface5 = document->NewElement("Action");
						surface5->SetAttribute("type", action_type);
						surface5->SetAttribute("isRepeat", action_data->getIsRepeat());
						surface5->SetAttribute("isReverse", action_data->getIsReverse());
						surface5->SetAttribute("delay", action_data->getDelay());
						switch (action_type)
						{
						case TYPE_ROTATE:
						{
							auto rotate_data = dynamic_cast<RotateActionData*>(action_data);
							surface5->SetAttribute("rotateDuration", rotate_data->getDuration());
							surface5->SetAttribute("angle", rotate_data->getAngle());
							surface5->SetAttribute("anchor_x", rotate_data->getAnchor().x);
							surface5->SetAttribute("anchor_y", rotate_data->getAnchor().y);
							
							break;
						}
						case TYPE_MOVE:
						{
							auto move_data = dynamic_cast<MoveActionData*>(action_data);
							surface5->SetAttribute("moveDuration", move_data->getDuration());
							surface5->SetAttribute("dest_x", static_cast<int>(move_data->getDestination().x));
							surface5->SetAttribute("dest_y", static_cast<int>(move_data->getDestination().y));
							surface4->LinkEndChild(surface5);
							break;
						}
						case TYPE_BLINK:
						{
							auto blink_data = dynamic_cast<BlinkActionData*>(action_data);
							surface5->SetAttribute("blinkDuration", blink_data->getDuration());
							surface5->SetAttribute("postDelay", blink_data->getPostDelay());
							break;
						}
						default:
							break;
						}
						surface4->LinkEndChild(surface5);
					}
					surface3->LinkEndChild(surface4);
				}
			}
		}

		string filename1 = FileUtils::getInstance()->fullPathForFilename("config/LevelMake_Workstation.xml");
		document->SaveFile(filename1.c_str());
		string filename2 = FileUtils::getInstance()->fullPathForFilename("../../Resources/config/LevelMake_Workstation.xml");
		document->SaveFile(filename2.c_str());

		// if has new level than update save file
		UserDefault* user = UserDefault::getInstance();
		for (size_t i = 0; i < m_nGameLevelCount; i++)
		{
			string key = StringUtils::format("Level%d-DeadCount", i + 1);
			if (user->getIntegerForKey(key.c_str(), -100) == -100)
				user->setIntegerForKey(key.c_str(), -1);
		}
		// if has new level than add new dead count
		for (size_t i = m_vLevelMinDeadCount.size(); i < m_nGameLevelCount; i++)
			m_vLevelMinDeadCount.push_back(-1);

		bRet = true;
	} while (false);

	return bRet;
}

void GameMediator::saveIntegerGameDataForKey(const char* key, int data)
{
	UserDefault::getInstance()->setIntegerForKey(key, data);
}

void GameMediator::setDeadCount(int deadCount)
{
	int minDeadCount = m_vLevelMinDeadCount.at(m_nCurGameLevel - 1);
	if (minDeadCount < 0 || deadCount < minDeadCount)
	{
		m_vLevelMinDeadCount.at(m_nCurGameLevel - 1) = deadCount;
		string key = StringUtils::format("Level%d-DeadCount", m_nCurGameLevel);
		this->saveIntegerGameDataForKey(key.c_str(), deadCount);
	}
}

void GameMediator::setMaxGameLevel()
{
	int nextLevel = m_nCurGameLevel + 1;
	CS_RETURN_IF(nextLevel > m_nGameLevelCount);

	if (nextLevel > m_nMaxGameLevel)
	{
		m_nMaxGameLevel = nextLevel;
		this->saveIntegerGameDataForKey("MaxLevel", m_nMaxGameLevel);
	}
}

void GameMediator::gotoNextGameLevel()
{
	if (m_nCurGameLevel < m_nGameLevelCount)
	{
		m_nCurGameLevel++;
		this->saveIntegerGameDataForKey("CurLevel", m_nCurGameLevel);
	}
	m_nCurGameRoom = 1;
}

void GameMediator::gotoNextGameRoom()
{
	m_nCurGameRoom++;
}

int GameMediator::getDeadCountAll(int level)
{
	int deadCountAll = 0;
	for (size_t i = 0, length = level; i < length; i++)
	{
		int deadCount = m_vLevelMinDeadCount.at(i);
		if (deadCount > 0)
			deadCountAll += deadCount;
	}
	return deadCountAll;
}
