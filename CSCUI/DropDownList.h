#pragma once
#include "../CommonDefinition.h"

NS_CSC_BEGIN
// set colors
#define DROPDOWNLIST_SHOWBG_COLOR3       Color3B(156, 156, 156)
#define DROPDOWNLIST_NORMAL_COLOR3       Color3B(128, 128, 128)
#define DROPDOWNLIST_SELECTED_COLOR3     Color3B(200, 200, 200)
#define DROPDOWNLIST_HIGHLIGHT_COLOR3    Color3B(0, 0, 255)  

const string EVENT_DROPDOWNLIST_SELECTED = "event_dropdownlist_selected_";

class DropDownList : public Layer
{
public:
	DropDownList();
	virtual ~DropDownList() override;

	static DropDownList* create(Label* label, Size size, const string &suffix);
	bool init(Label* label, Size size, const string &suffix);

	void addLabel(Label* label); // create menu item & add label on it

	void clearAllLabels();

	string getSelectedString() const
	{
		return m_pShowLabel->getString();
	}
	int getSelectedIndex() const
	{
		return m_nLastSelectedIndex;
	}
	void setSelectedIndex(int index);

private:
	virtual void onEnter() override;
	virtual bool onTouchBegan(Touch* touch, Event* event) override;
	void selectedItemEvent(Ref * pSender, ListView::EventType type);
	void onClose(); // close the list  
	
private:
	ListView* m_pListView; // list
	Label* m_pShowLabel;  // show what is be selected  

	Vector<Label*> m_vSelectLabels;  // labels for list  
	Vector<LayerColor*> m_vBgLayers; // background for list  

	string m_sSuffix;
	bool m_bIsShow;  // is list show or not  
	int m_nLastSelectedIndex;  // index of item from list been selected  
};
NS_CSC_END
