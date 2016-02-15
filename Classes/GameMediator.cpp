#include "GameMediator.h"
#include "CSCClass/CSC_IOSHelper.h"
#include "CSCClass/AudioCtrl.h"
#include "CSCClass/CSCData/CSC_Encryption.h"

USING_NS_CSC;

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

GameMediator::GameMediator(void): m_nGameLevelCount(0), m_nCurGameLevel(0), m_nMaxGameLevel(0), m_nCurGameRoom(0), m_nTotalDeadCount(0), m_nFirstStoryMaxGameLevel(0)
{
	m_vGameLevelData.clear();
	m_vLevelMinDeadCount.clear();
	m_mLevelStorys.clear();
	m_mEndStorys.clear();
}

GameMediator::~GameMediator(void)
{
	m_vGameLevelData.clear();
	m_vLevelMinDeadCount.clear();
	m_mLevelStorys.clear();
	m_mEndStorys.clear();
}

bool GameMediator::init()
{
	bool bRet = false;
	do
	{        
		// login-in GameCenter
		auto helper = CSC_IOSHelper::getInstance();
		helper->GameCenter_authenticateLocalUser();
#ifdef IAP_TEST
		helper->IAP_requestAllPurchasedProducts(true);
#else
		helper->IAP_requestAllPurchasedProducts(false);
#endif

		// get level config file
		CC_BREAK_IF(!this->loadGameLevelFile());

		// get level story file
		CC_BREAK_IF(!this->loadGameLevelStoryFile());

		// load music
		auto audio_engine = AudioCtrl::getInstance();
		audio_engine->addBackgroundMusic("Alan Walker - Faded.mp3");
		audio_engine->addBackgroundMusic("Alan Walker - Spectre.mp3");
		audio_engine->addBackgroundMusic("Arty - Worlds Collide.mp3");
		audio_engine->addBackgroundMusic("Calvin Harris - Iron.mp3");
		audio_engine->addBackgroundMusic("Deorro - Play.mp3");
		audio_engine->addBackgroundMusic("Hardwell - Jumper.mp3");
		audio_engine->addBackgroundMusic("James Egbert - The Glory.mp3");

		// set music volume
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0);
		audio_engine->playBackgroundMusicList(true);
		
		// make key string
		// get local save data
		UserDefault* user = UserDefault::getInstance();
		if (!user->getBoolForKey("isHaveSaveFileXml"))
		{
			user->setBoolForKey("isHaveSaveFileXml", true);
			user->setBoolForKey("Music", true);
			this->saveDataForKey(CURRENT_LEVEL, 1);
			this->saveDataForKey(MAX_LEVEL, 1);
			this->saveDataForKey(TOTAL_DEAD, -1);
			for (size_t i = 0; i < m_nGameLevelCount; i++)
			{
				this->saveDataForKey(LEVEL_DEAD_START + i, -1);
				m_vLevelMinDeadCount.push_back(-1);
			}
			m_nCurGameLevel = 1;
			m_nMaxGameLevel = 1;
			m_nTotalDeadCount = -1;
		}
		else
		{
			// pause the music due to user config
			if (!user->getBoolForKey("Music", true))
				audio_engine->stopBackgroundMusic();

			auto totalDead = 0;
			auto maxDeadLevel = 1;
			for (size_t i = 0; i < m_nGameLevelCount; i++)
			{
				auto levelDead = this->loadDataForKey(LEVEL_DEAD_START + i, -1);
				m_vLevelMinDeadCount.push_back(levelDead);
				if (levelDead > 0)
				{
					totalDead += levelDead;
					maxDeadLevel++;
				}
			}
			m_nTotalDeadCount = this->loadDataForKey(TOTAL_DEAD, -1);
			if (m_nTotalDeadCount != totalDead) // if have bug or player cheat
			{
				m_nTotalDeadCount = totalDead;
				this->saveDataForKey(TOTAL_DEAD, m_nTotalDeadCount);
			}
			m_nCurGameLevel = this->loadDataForKey(CURRENT_LEVEL, 1);
			if (m_nCurGameLevel > maxDeadLevel) // if delete some level or have bug
			{
				m_nCurGameLevel = maxDeadLevel;
				this->saveDataForKey(CURRENT_LEVEL, m_nCurGameLevel);
			}
			m_nMaxGameLevel = this->loadDataForKey(MAX_LEVEL, 1);
		}

		// set current room 
		m_nCurGameRoom = 1;
		
		bRet = true;
	} while (false);
	
	return bRet;
}

void GameMediator::saveDataForKey(const int& key, const int& data)
{
	int data_encode = data^key; // XOR
	UserDefault::getInstance()->setStringForKey(StringUtils::format("CSC%d", key).c_str(), csc_encode_base64(StringUtils::format("%d", data_encode)));
}

void GameMediator::resetData()
{
	for (size_t i = 0; i < m_nGameLevelCount; i++)
	{
		this->saveDataForKey(LEVEL_DEAD_START + i, -1);
		m_vLevelMinDeadCount.at(i) = -1;
	}
	this->saveDataForKey(TOTAL_DEAD, -1);
	m_nTotalDeadCount = -1;
}

int GameMediator::loadDataForKey(const int& key, const int& default_data)
{
	int default_data_encode = default_data^key; // XOR
	int data_decode = atoi(csc_decode_base64(UserDefault::getInstance()->getStringForKey(StringUtils::format("CSC%d", key).c_str(), csc_encode_base64(StringUtils::format("%d", default_data_encode)))));
	return data_decode^key; // XOR
}

bool GameMediator::loadGameLevelFile()
{
	bool bRet = false;
	do
	{
		tinyxml2::XMLDocument document;
		// Decryption
        //string filename = FileUtils::getInstance()->fullPathForFilename("config/GameLevels.xml");
		//document.LoadFile(filename.c_str());
		string filename = FileUtils::getInstance()->fullPathForFilename("config/GameLevels_Encryption.dat");
		string file_string = FileUtils::getInstance()->getStringFromFile(filename);
		unsigned char* buffer;
		int decoded_length = base64Decode((unsigned char*)file_string.c_str(), file_string.length(), &buffer);
		string xml_data_decoded = csc_decode_aes(buffer, decoded_length);
		document.Parse(xml_data_decoded.c_str(), xml_data_decoded.size());
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

		m_nGameLevelCount = (int)m_vGameLevelData.size();
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
				for (int i1 = 0, length1 = (int)enemysData->size(); i1 < length1; i1++)
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
						surface5->SetAttribute("postDelay", action_data->getPostDelay());
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
							break;
						}
						case TYPE_SCALE:
						{
							auto scale_data = dynamic_cast<ScaleActionData*>(action_data);
							surface5->SetAttribute("scaleDuration", scale_data->getDuration());
							surface5->SetAttribute("scale_x", scale_data->getScaleX());
							surface5->SetAttribute("scale_y", scale_data->getScaleY());
							surface5->SetAttribute("anchor_x", scale_data->getAnchor().x);
							surface5->SetAttribute("anchor_y", scale_data->getAnchor().y);
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

		auto fileCtrl = FileUtils::getInstance();
		document->SaveFile(fileCtrl->fullPathForFilename("config/GameLevels.xml").c_str());
		document->SaveFile(fileCtrl->fullPathForFilename("../../Resources/config/GameLevels.xml").c_str());

		// if has new level than update save file
		UserDefault* user = UserDefault::getInstance();
		for (size_t i = 0; i < m_nGameLevelCount; i++)
		{
			if (this->loadDataForKey(LEVEL_DEAD_START + i, -100) == -100)
				this->saveDataForKey(LEVEL_DEAD_START + i, -1);
		}
		// if has new level than add new dead count
		for (size_t i = m_vLevelMinDeadCount.size(); i < m_nGameLevelCount; i++)
			m_vLevelMinDeadCount.push_back(-1);

		bRet = true;
	} while (false);

	return bRet;
}

bool GameMediator::loadGameLevelStoryFile()
{
	bool bRet = false;
	do
	{
		tinyxml2::XMLDocument document;
		//string filename = FileUtils::getInstance()->fullPathForFilename("config/GameStorys.xml");
		//document.LoadFile(filename.c_str());
		string filename = FileUtils::getInstance()->fullPathForFilename("config/GameStorys_Encryption.dat");
		string file_string = FileUtils::getInstance()->getStringFromFile(filename);
		unsigned char* buffer;
		int decoded_length = base64Decode((unsigned char*)file_string.c_str(), file_string.length(), &buffer);
		string xml_data_decoded = csc_decode_aes(buffer, decoded_length);
		document.Parse(xml_data_decoded.c_str(), xml_data_decoded.size());
		XMLElement* root = document.RootElement();
		CC_BREAK_IF(!root);

		// load level storys
		m_mLevelStorys.clear();
		for (XMLElement* surface1 = root->FirstChildElement("Level"); surface1 != NULL; surface1 = surface1->NextSiblingElement("Level"))
		{
			auto level = surface1->IntAttribute("level");
			StoryData story;
			story.condition = surface1->IntAttribute("condition");
			story.end = surface1->IntAttribute("end");
			if (story.end == 1)
				m_nFirstStoryMaxGameLevel = level;
			for (XMLElement* surface2 = surface1->FirstChildElement("Line"); surface2 != NULL; surface2 = surface2->NextSiblingElement("Line"))
			{
				LineData storyline;
				storyline.text = surface2->Attribute("text");
				storyline.size = surface2->IntAttribute("fontsize");
				if (storyline.size == 0)
					storyline.size = 32;
				auto color_r = surface2->IntAttribute("color_r");
				auto color_g = surface2->IntAttribute("color_g");
				auto color_b = surface2->IntAttribute("color_b");
				if (color_r == 0 && color_g == 0 && color_b == 0)
					storyline.color = Color3B::WHITE;
				else
					storyline.color = Color3B(color_r, color_g, color_b);
				storyline.interval = surface2->IntAttribute("interval");

				story.line_data.push_back(storyline);
			}
			m_mLevelStorys.insert(pair<int, StoryData>(level, story));
		}

		// load end storys
		m_mEndStorys.clear();
		for (XMLElement* surface1 = root->FirstChildElement("End"); surface1 != NULL; surface1 = surface1->NextSiblingElement("End"))
		{
			auto id = surface1->IntAttribute("id");
			map<string, vector<LineData>> end_story;
			for (XMLElement* surface2 = surface1->FirstChildElement("Subend"); surface2 != NULL; surface2 = surface2->NextSiblingElement("Subend"))
			{
				auto condition = surface2->Attribute("condition");
				vector<LineData> storylines;
				for (XMLElement* surface3 = surface2->FirstChildElement("Line"); surface3 != NULL; surface3 = surface3->NextSiblingElement("Line"))
				{
					LineData storyline;
					storyline.text = surface3->Attribute("text");
					storyline.size = surface3->IntAttribute("fontsize");
					if (storyline.size == 0)
						storyline.size = 32;
					auto color_r = surface3->IntAttribute("color_r");
					auto color_g = surface3->IntAttribute("color_g");
					auto color_b = surface3->IntAttribute("color_b");
					if (color_r == 0 && color_g == 0 && color_b == 0)
						storyline.color = Color3B::WHITE;
					else
						storyline.color = Color3B(color_r, color_g, color_b);
					storyline.interval = surface3->IntAttribute("interval");

					storylines.push_back(storyline);
				}
				end_story.insert(pair<string, vector<LineData>>(condition, storylines));
			}
			m_mEndStorys.insert(pair<int, map<string, vector<LineData>>>(id, end_story));
		}

		bRet = true;
	} while (false);

	return bRet;
}

void GameMediator::setDeadCount(int deadCount)
{
	int minDeadCount = m_vLevelMinDeadCount.at(m_nCurGameLevel - 1);
	if (minDeadCount < 0 || deadCount < minDeadCount)
	{
		m_vLevelMinDeadCount.at(m_nCurGameLevel - 1) = deadCount;
		this->saveDataForKey(LEVEL_DEAD_START + m_nCurGameLevel - 1, deadCount);
		// update total dead
		m_nTotalDeadCount = 0;
		for (size_t i = 0; i < m_nGameLevelCount; i++)
		{
			auto levelDeadCount = m_vLevelMinDeadCount.at(i);
			if (levelDeadCount > 0)
				m_nTotalDeadCount += levelDeadCount;
		}
		this->saveDataForKey(TOTAL_DEAD, m_nTotalDeadCount);
		if (m_nMaxGameLevel >= m_nFirstStoryMaxGameLevel && m_nCurGameLevel <= m_nFirstStoryMaxGameLevel)
			CSC_IOSHelper::getInstance()->GameCenter_reportScoreForLeaderboard("com.reallycsc.lifeishard.totaldeadcount1", this->getDeadCountAll(m_nFirstStoryMaxGameLevel));
		if (m_nMaxGameLevel == m_nGameLevelCount)
		{
			CSC_IOSHelper::getInstance()->GameCenter_reportScoreForLeaderboard("com.reallycsc.lifeishard.totaldeadcount_all", m_nTotalDeadCount);
			CSC_IOSHelper::getInstance()->GameCenter_checkAndUnlockAchievement(" com.reallycsc.lifeishard.finish");
		}
		else if (deadCount >= 500)
			CSC_IOSHelper::getInstance()->GameCenter_checkAndUnlockAchievement("com.reallycsc.lifeishard.dead500");
	}
}

void GameMediator::setMaxGameLevel()
{
	int nextLevel = m_nCurGameLevel + 1;
	if (nextLevel <= m_nGameLevelCount && nextLevel > m_nMaxGameLevel)
	{
		m_nMaxGameLevel = nextLevel;
		this->saveDataForKey(MAX_LEVEL, m_nMaxGameLevel);
		CSC_IOSHelper::getInstance()->GameCenter_reportScoreForLeaderboard("com.reallycsc.lifeishard.maxlevel", m_nMaxGameLevel);
	}
}

void GameMediator::setCurGameLevel(const int &level)
{
    m_nCurGameLevel = level;
    this->saveDataForKey(CURRENT_LEVEL, m_nCurGameLevel);
}

void GameMediator::saveUserConfig()
{
	auto audio = AudioCtrl::getInstance();
	UserDefault::getInstance()->setBoolForKey("Music", audio->getIsListPlaying() & !audio->getIsMute());
}

void GameMediator::gotoNextGameLevel()
{
	int nextLevel = m_nCurGameLevel + 1;
	if (nextLevel <= m_nGameLevelCount)
	{
		m_nCurGameLevel = nextLevel;
		this->saveDataForKey(CURRENT_LEVEL, m_nCurGameLevel);
		if (m_nCurGameLevel > m_nMaxGameLevel)
		{
			m_nMaxGameLevel = m_nCurGameLevel;
			this->saveDataForKey(MAX_LEVEL, m_nMaxGameLevel);
			CSC_IOSHelper::getInstance()->GameCenter_reportScoreForLeaderboard("com.reallycsc.lifeishard.maxlevel", m_nMaxGameLevel);
		}
	}
	m_nCurGameRoom = 1;
}

void GameMediator::gotoNextGameRoom()
{
	m_nCurGameRoom++;
}

void GameMediator::encodeAllFiles()
{
	auto fileCtrl = FileUtils::getInstance();
	// Encryption Game Level File
	string gameLevel_data_encoded = csc_encode_base64(csc_encode_aes(fileCtrl->getStringFromFile(fileCtrl->fullPathForFilename("config/GameLevels.xml"))));
	fileCtrl->writeStringToFile(gameLevel_data_encoded, fileCtrl->fullPathForFilename("config/GameLevels_Encryption.dat"));
	fileCtrl->writeStringToFile(gameLevel_data_encoded, fileCtrl->fullPathForFilename("../../Resources/config/GameLevels_Encryption.dat"));

	// Encryption Game Story File
	string gameStory_data_encoded = csc_encode_base64(csc_encode_aes(fileCtrl->getStringFromFile(fileCtrl->fullPathForFilename("config/GameStorys.xml"))));
	fileCtrl->writeStringToFile(gameStory_data_encoded, fileCtrl->fullPathForFilename("config/GameStorys_Encryption.dat"));
	fileCtrl->writeStringToFile(gameStory_data_encoded, fileCtrl->fullPathForFilename("../../Resources/config/GameStorys_Encryption.dat"));
}

int GameMediator::getDeadCountAll(int level)
{
	int deadCountAll = 0;
	for (size_t i = 0, length = MIN(level, m_nGameLevelCount); i < length; i++)
	{
		int deadCount = m_vLevelMinDeadCount.at(i);
		if (deadCount > 0)
			deadCountAll += deadCount;
	}
	return deadCountAll;
}

StoryData* GameMediator::getLevelStoryLines(int level)
{
	if (m_mLevelStorys.find(level) == m_mLevelStorys.end())
		return nullptr;
	else
		return &m_mLevelStorys.at(level);
}

map<string, vector<LineData>>* GameMediator::getEndStoryLines(int id)
{
	if (m_mEndStorys.find(id) == m_mEndStorys.end())
		return nullptr;
	else
		return &m_mEndStorys.at(id);
}