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
	m_vGameLevelData.clear();
}

GameMediator::~GameMediator(void)
{
	m_vGameLevelData.clear();
}

bool GameMediator::init()
{
	bool bRet = false;
	do
	{
		m_nMaxGameLevel = 0;
		m_nCurGameLevel = 1;
		// get local save data
		UserDefault* user = UserDefault::getInstance();
		if (!user->getBoolForKey("isHaveSaveFileXml"))
		{
			user->setBoolForKey("isHaveSaveFileXml", true);
			user->setIntegerForKey("CurLevel", 1);
		}
		else
		{
			m_nCurGameLevel = user->getIntegerForKey("CurLevel", 1);
		}

		CC_BREAK_IF(!this->loadGameLevelFile());

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
		for (XMLElement* surface1 = root->FirstChildElement("Level"); surface1 != NULL; surface1 = surface1->NextSiblingElement("Level"))
		{
			GameLevelData* data = GameLevelData::create();
			data->setLevel(surface1->IntAttribute("level"));
			data->setLevelName(surface1->Attribute("name"));
			data->setMaxDeadTime(surface1->IntAttribute("deadTime"));

			CC_BREAK_IF(!data->setRoomDataWithFile(surface1));

			m_vGameLevelData.push_back(data);
			m_nMaxGameLevel++;
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

		for (size_t i = 0, j = m_vGameLevelData.size(); i < j; i++)
		{
			XMLElement *surface1 = document->NewElement("Level");
			surface1->SetAttribute("level", m_vGameLevelData.at(i)->getLevel());
			surface1->SetAttribute("name", m_vGameLevelData.at(i)->getLevelName().c_str());
			surface1->SetAttribute("deadTime", m_vGameLevelData.at(i)->getMaxDeadTime());
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
				for (size_t i1 = 0, j1 = enemysData->size(); i1 < j1; i1++)
				{
					XMLElement *surface4 = document->NewElement("Enemy");
					surface4->SetAttribute("id", i1 + 1);
					Size size = enemysData->at(i1).size;
					surface4->SetAttribute("width", size.width);
					surface4->SetAttribute("height", size.height);
					Point pos = enemysData->at(i1).position;
					surface4->SetAttribute("x", static_cast<int>(pos.x));
					surface4->SetAttribute("y", static_cast<int>(pos.y));
					surface3->LinkEndChild(surface4);
				}
			}
		}

		string filename = FileUtils::getInstance()->fullPathForFilename("config/LevelMake_Workstation.xml");
		document->SaveFile(filename.c_str());
		bRet = true;
	} while (false);

	return bRet;
}

void GameMediator::saveGameData()
{
	UserDefault* user = UserDefault::getInstance();
	user->setIntegerForKey("CurLevel", m_nCurGameLevel);
}