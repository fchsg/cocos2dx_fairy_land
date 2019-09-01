//
//  YYLoginInterface.h
//  Fairyland
//
//  Created by fuchenhao on 14-5-22.
//
//

#ifndef __Fairyland__YYLoginInterface__
#define __Fairyland__YYLoginInterface__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "YYController.h"

class YYSprite;

enum PanelType
{
    kPanelTypeDefault = 0,
	kPanelTypeChooseServer = 1,//选择服务器界面
	kPanelTypeRegister,//注册
	kPanelTypeLogin,//快速登录
	kPanelTypeLoginWithAccount,//登录
    kPanelTypeLostPassword,//忘记密码
	kPanelTypeChooseRole,//选人界面
    kPanelTypeCreateRole//创建角色
};

typedef struct RoleAnimation
{
    int actionIdx[10];  //动作索引
    int curActionIdx;  //当前动作ID
    int count;         //动作总数
    YYSprite* effect[10]; //当前动作对应特效
    
}RoleAnimation;

class LoginUI : public cocos2d::CCLayer,public cocos2d::extension::CCEditBoxDelegate
{
    CC_SYNTHESIZE(PanelType, m_ePanelType, PanelType);
    CC_SYNTHESIZE(cocos2d::gui::TouchGroup *,m_pLoginTouchGroup, LoginTouchGroup);
    
private:
    cocos2d::extension::CCEditBox * m_pAccountTF;
    cocos2d::extension::CCEditBox * m_pPasswordTF;
    cocos2d::extension::CCEditBox * m_pNewAccountTF;
    cocos2d::extension::CCEditBox * m_pNewPasswordTF;
    cocos2d::extension::CCEditBox * m_pNewPassword2TF;
    cocos2d::extension::CCEditBox * m_pNewAccountMailTF;
    cocos2d::extension::CCEditBox * m_pLostPasswordMailTF;
	
    cocos2d::gui::UIPanel *m_pPanelLogin;
	cocos2d::gui::UIPanel *m_pPanelLoginWithAccount;
    cocos2d::gui::UIPanel *m_pPanelChooseServer;
    cocos2d::gui::UIPanel *m_pPanelLostPassword;
    cocos2d::gui::UIPanel *m_pPanelRegister;
    cocos2d::gui::UIPanel *m_pPanelChooseRole;
    cocos2d::gui::UIPanel *m_pPanelCreateRole;
	cocos2d::gui::UIPanel *m_pPanelCurrent;
	cocos2d::gui::UIButton *m_pButtonSwitchAccount;
	cocos2d::gui::UIImageView *m_pImageViewAccount;
    cocos2d::gui::UILabel *m_pDeleteTimeLabel[3];// 删除时间标签
    cocos2d::gui::UIImageView* m_pBorder;
    cocos2d::extension::CCEditBox * m_pInputName;
	cocos2d::gui::UIButton *m_pCSButton[3][4];
    cocos2d::CCPoint m_UIInitPosition;
    bool m_bNetLoading;
    bool *m_bRolePlaying;      //是否播放角色待机动画
    YYSprite *m_pRoleAni[3];   //角色选择页面人物动画
    YYSprite *m_pHaloAni1[3];       //光圈
    YYSprite *m_pHaloAni2[3];       //光环
    cocos2d::gui::UIImageView* m_pRoleShadow[3]; //阴影
    int m_nHaloPlayingIdx;          //选中位置播放光环
    
    int m_iProfessionIndex;
    bool m_bCreatRolePlaying[3];       //是否播放角色动画
    YYSprite* m_pCreateRoleSprite[3];  //播放动画YYSprite
    RoleAnimation m_pRoleAnimation[3]; //动作效果 0 战士 1 法师 2 弓手
    int32_t m_nCurrentTime[3];
    
    cocos2d::gui::UIPanel *m_pTipPanel;// 提示信息面板
    TipType m_eTipType;// 提示信息类型
    
public:
    
    LoginUI();
    ~LoginUI();
    CREATE_FUNC(LoginUI);
    
	virtual bool init();
    virtual void onExit();
	virtual void onEnter();
    
	void onBtnFastLogin(CCObject* pSender, cocos2d::gui::TouchEventType type);
	void onBtnGameStart(CCObject* pSender, cocos2d::gui::TouchEventType type);
	void onBtnLoginWithAccount(CCObject* pSender, cocos2d::gui::TouchEventType type);
	void onBtnRegisterNewAccount(CCObject* pSender, cocos2d::gui::TouchEventType type);
	void onBtnSaveAccount(CCObject* pSender, cocos2d::gui::TouchEventType type);
	void onBtnRegister(CCObject* pSender, cocos2d::gui::TouchEventType type);
	
	void onBtnBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
	void onBtnLostPassword(CCObject* pSender, cocos2d::gui::TouchEventType type);
	void onBtnSendPassword(CCObject* pSender, cocos2d::gui::TouchEventType type);
	void onBtnChooseServer(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void onBtnRecentServerSelected(CCObject* pSender, cocos2d::gui::TouchEventType type);
	void onBtnAllServerSelected(CCObject* pSender, cocos2d::gui::TouchEventType type);
	//
	void onBtnRoleDelete(CCObject* pSender, cocos2d::gui::TouchEventType type);
	void onBtnRoleStartGame(CCObject* pSender, cocos2d::gui::TouchEventType type);
	void onBtnRoleSelect(CCObject* pSender, cocos2d::gui::TouchEventType type);
	void onBtnChooseRoleBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    
    void menuEnterCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuReturnCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuStartCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void btnGenRoleName(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuChooseRoleCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
    
	void showUILayer(PanelType panelType);
	void initPanelLogin();
	void initPanelLoginWithAccount();	//
	void initPanelChooseServer();
	void initPanelLostPassword();
	void initPanelRegister();	//
	void initPanelChooseRole();
    void initPanelCreateRole();
    void setLoginEditBoxEnable(bool isTouchEnable);
    void setRegisterEditBoxEnable(bool isTouchEnable);
    void showServerList();// 更新服务器列表
    void setSelectedServerAndRecentServerList();//设置选中的服务器并修改近期访问过的服务器列表
	void showRolePanel(int panel, void *data);
	void setSelectedRole(int roleIndex);
    void updateRole(int roleTag);
    void initRoleAnimation(int idx,cocos2d::gui::UIImageView* imgView);
    void updateRoleAction(int index);
    void resetRoleAction();
    
    void setStartButtonEnable(bool isRoleCreated);//设置开始游戏选项是否起作用(未创建人物角色时无法开始游戏)
    void setDeleteButtonEnable(bool isRoleCreated);
    void setCreateButtonEnable(bool isEnable);
	void setCSButtonVisible(int row,int col,bool visible);
    void setRoleName(const char *pText);
    
    virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox *editBox);
    virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox *editBox);
    virtual void editBoxTextChanged(cocos2d::extension::CCEditBox *editBox,const std::string &text);
    virtual void editBoxReturn(cocos2d::extension::CCEditBox *editBox);
    
    
    void setInputNameTouchEnabled(bool b);
    
    // 角色删除提示
    void setDeleteInfo(int panelIndex);
    void setDeleteTime(int deleteTime ,int panelIdx);
    void updateDeleteTime();
    
    void update(float f);
};


#endif /* defined(__Fairyland__YYLoginInterface__) */
