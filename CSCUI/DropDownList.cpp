#include "DropDownList.h"

NS_CSC_BEGIN
DropDownList::DropDownList()
	: m_pListView(nullptr)
	, m_pShowLabel(nullptr)
	, m_sSuffix("")
	, m_bIsShow(false)
	, m_nLastSelectedIndex(0)
{
}

DropDownList::~DropDownList()
{
	m_pListView->release();
}

DropDownList* DropDownList::create(Label* label, Size size, const string &suffix)
{
	DropDownList* pRet = new DropDownList();
	if (pRet && pRet->init(label, size, suffix))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool DropDownList::init(Label* label, Size size, const string &suffix)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());

		m_sSuffix = suffix;
		this->setContentSize(size);

		// create show bg
		LayerColor* bg = LayerColor::create(Color4B(DROPDOWNLIST_SHOWBG_COLOR3), size.width, size.height);
		this->addChild(bg);
		// create show label
		m_pShowLabel = label;
		m_pShowLabel->setPosition(size / 2);
		this->addChild(m_pShowLabel);
		// create menu with scroll but not addchild
		m_pListView = ListView::create();
		m_pListView->setDirection(ScrollView::Direction::VERTICAL);
		m_pListView->setBounceEnabled(true);
		m_pListView->setBackGroundColorType(ListView::BackGroundColorType::SOLID);
		m_pListView->setBackGroundColor(DROPDOWNLIST_SELECTED_COLOR3);
		m_pListView->setContentSize(size);
		m_pListView->setAnchorPoint(Vec2(0.5, 1));
		m_pListView->setPosition(Vec2(size.width / 2, 0));
		m_pListView->addEventListener(static_cast<ListView::ccListViewCallback>(CC_CALLBACK_2(DropDownList::selectedItemEvent, this)));
		m_pListView->setScrollBarPositionFromCorner(Vec2(2, 2));
		m_pListView->retain();

		// create first item
		this->addLabel(
			Label::createWithSystemFont(label->getString(), 
				label->getSystemFontName(), 
				label->getSystemFontSize())
			);

		bRet = true;
	} while (false);
	return bRet;
}

void DropDownList::setSelectedIndex(int index)
{
	m_nLastSelectedIndex = index;

	for (int i = 0, j = static_cast<int>(m_vSelectLabels.size()); i < j; ++i)
	{
		if (i == m_nLastSelectedIndex)
		{
			m_vBgLayers.at(i)->setColor(DROPDOWNLIST_HIGHLIGHT_COLOR3);
			m_pShowLabel->setString(m_vSelectLabels.at(i)->getString());
		}
		else
		{
			m_vBgLayers.at(i)->setColor(DROPDOWNLIST_NORMAL_COLOR3);
		}
	}
}

void DropDownList::onEnter()
{
	Layer::onEnter();

	// bind touch event
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(DropDownList::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool DropDownList::onTouchBegan(Touch* touch, Event* event)
{
	if (m_vSelectLabels.size() > 0)
	{
		Rect rect(Vec2(0, 0), this->getContentSize());
		Size listSize = m_pListView->getContentSize();
		Rect rect2(Vec2(0, -listSize.height), Size(listSize.width, 0));
		Point position = this->convertToNodeSpace(touch->getLocation());

		if (!m_bIsShow && rect.containsPoint(position))
		{
			m_bIsShow = true;
			// show list menus  
			this->addChild(m_pListView);
			Size old_size = m_pListView->getContentSize();
			m_pListView->setContentSize(Size(old_size.width, 
				MIN(old_size.height, this->convertToWorldSpace(Vec2(0, 0)).y - 10)));

			for (int i = 0, j = static_cast<int>(m_vSelectLabels.size()); i < j; ++i)
				if (i == m_nLastSelectedIndex)
					m_vBgLayers.at(i)->setColor(DROPDOWNLIST_HIGHLIGHT_COLOR3);
				else
					m_vBgLayers.at(i)->setColor(DROPDOWNLIST_NORMAL_COLOR3);

			return true;
		}
		else if (m_bIsShow && !rect2.containsPoint(position))
		{
			this->onClose(); // close the list
		}
	}
	return false;
}

void DropDownList::selectedItemEvent(Ref *pSender, ListView::EventType type)
{
	switch (type)
	{
	case  ListView::EventType::ON_SELECTED_ITEM_END:
	{
		ListView* listView = static_cast<ListView*>(pSender);

		auto index = listView->getCurSelectedIndex();
		m_nLastSelectedIndex = (int)index;
		m_pShowLabel->setString(m_vSelectLabels.at(index)->getString());

		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_DROPDOWNLIST_SELECTED + m_sSuffix); // dispatch event

		this->onClose(); // close the list
		break;
	}
	default:
		break;
	}
}

void DropDownList::addLabel(Label* label)
{
	Size size = this->getContentSize();

	LayerColor* normal = LayerColor::create(Color4B(DROPDOWNLIST_NORMAL_COLOR3), size.width, size.height);

	m_vBgLayers.pushBack(normal);
	m_vSelectLabels.pushBack(label);

	Layout *layout = Layout::create();
	layout->setTouchEnabled(true);
	layout->setContentSize(size);
	layout->addChild(normal);
	label->setPosition(size / 2);
	layout->addChild(label);

	m_pListView->setContentSize(Size(size.width, static_cast<int>(m_vSelectLabels.size()) * size.height));
	m_pListView->addChild(layout);
}

void DropDownList::clearAllLabels()
{
	m_vBgLayers.clear();
	m_vSelectLabels.clear();
	m_pListView->removeAllChildren();
	m_pShowLabel->setString("None");
}

void DropDownList::onClose()
{
	this->removeChild(m_pListView);  // delete the list menus 
	m_bIsShow = false;
}

NS_CSC_END
