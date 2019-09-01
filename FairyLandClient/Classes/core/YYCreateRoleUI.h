////
////  YYStartLayer.h
////  Fairland
////
////  Created by 章敏 on 11/8/13. 
////
////
//
//#ifndef __Fairland__YYStartLayer__
//#define __Fairland__YYStartLayer__
//
//#include <iostream>
//#include <cocos2d.h>
//#include "cocos-ext.h"
//
//class YYSprite;
//
//
//class CreateRoleUI : public cocos2d::CCLayer
//{
//    
//    CC_SYNTHESIZE(uint8_t, m_u8LoginStatus, LoginStatus);
//    
//private:
//    
//    cocos2d::CCPoint m_TouchPoint;
//    bool m_bTouchBegan;
//    cocos2d::gui::TouchGroup *m_pLoginTG;
//    cocos2d::CCPoint m_UIInitPosition;
//    cocos2d::gui::UITextField* input;
//    //cocos2d::gui::UIImageView *center;
//    cocos2d::extension::CCEditBox * m_pInputName;
//    bool m_bNetLoading;
//    cocos2d::gui::UIButton* enter_button;
//    int m_iProfessionIndex;
//    bool m_bCreatRolePlaying[3];  //是否播放角色动画
//    YYSprite* m_pCreateRoleSprite[3];  //播放动画YYSprite
////    roleAnimation m_pRoleAnimation[3];  //动作效果 0 战士 1 法师 2 弓手
//
//public:
//    
//    CreateRoleUI();
//    ~CreateRoleUI();
//    
//    CREATE_FUNC(CreateRoleUI);
//    void touchBegan(cocos2d::CCSet *pTouches);
//    static cocos2d::CCScene* scene(CreateRoleUI * start);
//    bool init();
//    void menuEnterCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
//    void menuReturnCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
//    void menuStartCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
//    void menuChooseRoleCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
//	void btnGenRoleName(CCObject* pSender, cocos2d::gui::TouchEventType type);
//
//    void update();
//    void updateRole(int roleTag);
//    void createRole();
//    void setNetLoading(bool isNetLoading);
//    void setRoleName(const char *pText);
//    
//    void initRoleAnimation(int idx, cocos2d::gui::UIImageView* imgView);
//    void updateRoleAction(int idx);
//
//};
//
//enum
//{
//    kLoginChooseRole = 0,
//    kLoginCreateRole
//};
//
//#endif /* defined(__Fairland__YYStartLayer__) */
