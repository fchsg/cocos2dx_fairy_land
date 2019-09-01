//
//  Common.h
//  XJ
//
//  Created by wxf on 14-3-20. 
//
//

#include "Common.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;


CCEditBox *createEditBox(CCEditBoxDelegate *editBoxDelegate, Widget* parent,CCSize size, int maxInputLen,const char*placeHolderText)
{
    std::string filePath = getFilePath("223",kDirTypeUI,kFileSuffixPng);
	CCEditBox*editBox = CCEditBox::create(size,CCScale9Sprite::create(filePath.c_str()));
	editBox->setPosition(ccp(0,0));
	editBox->setFont("Helvetica-Bold",25);
	editBox->setFontColor(ccRED);
	editBox->setPlaceHolder(placeHolderText);
	editBox->setPlaceholderFontColor(ccWHITE);
	editBox->setMaxLength(maxInputLen);
	editBox->setReturnType(kKeyboardReturnTypeDone);
	editBox->setDelegate(editBoxDelegate);
	editBox->setOpacity(0); // 设置透明度实际是后面为输入框 因为此背景并不从最左边出来 不能设置为输入框
	editBox->setInputMode(kEditBoxInputModeSingleLine);
	editBox->setTouchPriority(-1);
    editBox->setTouchEnabled(true);
	parent->addNode(editBox,100);
	return editBox;
}

Widget *getChildWidgetByName(Widget *root, std::string childName)
{
	char nodeName[128];
	const char *nv = childName.c_str();
	Widget * widget = NULL;

	if(strchr(childName.c_str(), '.') == NULL)
	{
		widget = static_cast<Widget*>(UIHelper::seekWidgetByName(root, childName.c_str()));
	}
	else
	{
		widget = root;
		while(widget && nv)
		{
			nv = get_str_field(nv, '.', nodeName, sizeof(nodeName));
			widget = widget->getChildByName(nodeName);
		}
	}
	return widget;
}

bool setWidgetVisible(Widget *root, std::string childName, bool flag)
{
	Widget * widget = getChildWidgetByName(root, childName.c_str());

	if(widget)
	{
		widget->setVisible(flag);
		return true;
	}
	return false;
}

bool addWidgetTouchEvent(Widget *root, std::string childName, CCObject *target, SEL_TouchEvent selector)
{
	Widget * widget = getChildWidgetByName(root, childName.c_str());

	if(widget)
	{
		widget->setTouchEnabled(true);
		widget->addTouchEventListener(target, selector);
		return true;
	}
	return false;
}
