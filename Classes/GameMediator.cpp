#include "GameMediator.h"
#include "CSCClass/CommonFunctions.h"
#include <complex>

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
	m_mGameText.clear();
	m_vGameLevelData.clear();
}

GameMediator::~GameMediator(void)
{
	m_mGameText.clear();
	m_vGameLevelData.clear();
}

bool GameMediator::init()
{
	bool bRet = false;
	do
	{
		m_languageType = Application::getInstance()->getCurrentLanguage();
		m_nCurGameLevel = 1;
		m_nGameLevelMax = 0;

		CC_BREAK_IF(!this->loadGameLevelFile());

		CC_BREAK_IF(!this->loadGameTextFile());

		bRet = true;
	} while (false);
	
	return bRet;
}

void GameMediator::reloadAllConfigFiles()
{
	m_nGameLevelMax = 0;

	CS_RETURN_IF(!this->loadGameLevelFile());

	CS_RETURN_IF(!this->loadGameTextFile());
}

void GameMediator::changeLanguageTo(LanguageType &type)
{
	CS_RETURN_IF(m_languageType == type);

	m_languageType = type;

	this->loadGameTextFile();
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
			int level = surface1->IntAttribute("level");
			data->setLevel(level);

			CC_BREAK_IF(!data->setRoomDataWithFile(surface1));

			m_vGameLevelData.push_back(data);
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
			surface1->SetAttribute("level", i + 1);
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

bool GameMediator::loadGameTextFile()
{
	bool bRet = false;
	do
	{
		tinyxml2::XMLDocument document;
		string filename;
		switch (m_languageType)
		{
		case LanguageType::CHINESE:
			filename = FileUtils::getInstance()->fullPathForFilename("config/GameText_Chinese.xml");
			break;
		case LanguageType::ENGLISH:
			filename = FileUtils::getInstance()->fullPathForFilename("config/GameText_English.xml");
			break;
		default:
			filename = FileUtils::getInstance()->fullPathForFilename("config/GameText_English.xml");
			break;
		}

        document.LoadFile(filename.c_str());
		XMLElement* root = document.RootElement();
		CC_BREAK_IF(!root);

		m_mGameText.clear();
		for (XMLElement* surface1 = root->FirstChildElement("Text"); surface1 != NULL;
		surface1 = surface1->NextSiblingElement("Text"))
		{
            string str = string(surface1->GetText());
            string oldStr = "\\n";
            string newStr = "\n";
			m_mGameText.insert(pair<string, string>(surface1->Attribute("id"), CSCClass::replace_all_distinct(str, oldStr, newStr)));
		}

		bRet = true;
	} while (false);
	return bRet;
}

void GameMediator::initGame()
{
	m_nCurGameLevel = 1;
}