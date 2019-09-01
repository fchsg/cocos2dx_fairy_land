//
//  Common.h
//  XJ
//
//  Created by wxf on 14-3-20. 
//
//

#ifndef XJ_CCUTIL_H
#define XJ_CCUTIL_H

#include "cocos2d.h"

cocos2d::extension::CCEditBox* createEditBox(cocos2d::extension::CCEditBoxDelegate *editBoxDelegate, cocos2d::gui::Widget *parent,cocos2d::CCSize size, int maxInputLen,const char *placeHolderText);
cocos2d::gui::Widget* getChildWidgetByName(cocos2d::gui::Widget *root, std::string childName);
bool setWidgetVisible(cocos2d::gui::Widget *root, std::string childName, bool flag);
bool addWidgetTouchEvent(cocos2d::gui::Widget *root, std::string childName, cocos2d::CCObject *target, cocos2d::gui::SEL_TouchEvent selector);

#endif
