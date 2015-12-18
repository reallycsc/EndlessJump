#include "GameLevelData.h"
#include "CommonFunctions.h"

GameLevelData::GameLevelData(void)
{
}

GameLevelData::~GameLevelData(void)
{
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

			m_playerColor = gamedata->getPlayerColor();
			m_fPlayerSpeed = gamedata->getPlayerSpeed();
			m_fPlayerJumpDuration = gamedata->getPlayerJumpDuration();

			for (list<RoomData>::const_iterator iter = gamedata->getRoomsData()->begin();
			iter != gamedata->getRoomsData()->end(); iter++)
			{
				m_lRoomsData.push_back((*iter));
			}
		}
		else
		{
			Size visibleSize = Director::getInstance()->getVisibleSize();

			// level data
			m_nLevel = 1;

			// player data
			m_playerColor = Color4F::BLUE;
			m_fPlayerSpeed = visibleSize.width / 3;
			m_fPlayerJumpDuration = 0.8f;

			// room data
			float topPosY = visibleSize.height;
			float roomHeight = visibleSize.height * 0.2;
			int roomNumber = 5;
			for (int i = 0; i < roomNumber; i++)
			{
				RoomData room;
				room.size = Size(visibleSize.width, roomHeight);
				room.position = Vec2(0, topPosY - roomHeight);
				int c = 25*i + 128;
				room.color = Color3B(c, c, c);

				// enemy
				Size enemySize = generateRandomEnemySize();
				while (enemySize.width <= 0 || enemySize.height <= 0)
				{
					enemySize = generateRandomEnemySize();
				}
				EnemyData enemy;
				enemy.size = enemySize;
				enemy.position = Vec2(visibleSize.width / 2, room.position.y);
				enemy.color = Color3B(c, 0, 0);
				room.enemysData.push_back(enemy);

				topPosY = room.position.y;
				m_lRoomsData.push_back(room);
			}
		}
		
		bRet = true;
	} while (0);

	return bRet;
}

Size GameLevelData::generateRandomEnemySize() const
{
	float halfTime = m_fPlayerJumpDuration / 2;
	float randTime = random(0.0f, halfTime);
	float scaleRandTime = randTime / halfTime * 0.5f;

	float width = m_fPlayerSpeed * (halfTime - randTime) * 2 - 48;
	float height = 48 * 4 * scaleRandTime * (1 - scaleRandTime);

	return Size(width, height);
}

void GameLevelData::generateRandomLevel(int w, int h)
{

}