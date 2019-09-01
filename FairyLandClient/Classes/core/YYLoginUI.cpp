//
//  YYLoginInterface.cpp
//  Fairyland
//
//  Created by fuchenhao on 14-5-22.
//
//

#include "YYLoginUI.h"
#include "../common/ODSocket.h"
#include "../common/ccUtil.h"
#include "../common/Common.h"
#include "../animation/YYSprite.h"
#include "SimpleAudioEngine.h"
#include "YYConfig.h"
#include "YYScene.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif
USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
using namespace gui;

LoginUI::LoginUI()
:m_bNetLoading(false)
,m_pAccountTF(NULL)
,m_pPasswordTF(NULL)
,m_pNewAccountTF(NULL)
,m_pNewPasswordTF(NULL)
,m_pNewPassword2TF(NULL)
,m_pNewAccountMailTF(NULL)
,m_pLostPasswordMailTF(NULL)
,m_pPanelLogin(NULL)
,m_pPanelChooseServer(NULL)
,m_pPanelLostPassword(NULL)
,m_pPanelRegister(NULL)
,m_pPanelCurrent(NULL)
,m_pButtonSwitchAccount(NULL)
,m_pImageViewAccount(NULL)
,m_ePanelType(kPanelTypeDefault)
,m_nHaloPlayingIdx(-1)
{
    m_bRolePlaying = new bool[3];
    for (int i = 0; i < 3; i++)
    {
        m_bRolePlaying[i] = false;
        m_pRoleAni[i] = NULL;
        m_pHaloAni1[i] = NULL;
        m_pHaloAni2[i] = NULL;
        m_pRoleShadow[i] = NULL;
        m_pDeleteTimeLabel[i] = NULL;
        m_nCurrentTime[i] = 0;
    }
}

LoginUI::~LoginUI()
{
    CC_SAFE_DELETE(m_bRolePlaying);
}

static void showPanel(UIPanel* panel, bool visible)
{
	if(panel)
	{
		panel->setVisible(visible);
		panel->setEnabled(visible);
	}
}
bool LoginUI::init()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //黑背景
    ccColor4B ct0 = {0,0,0,255};
    CCLayerColor *blackBG = CCLayerColor::create(ct0, winSize.width, winSize.height);
    addChild(blackBG);
    m_UIInitPosition = ccp((winSize.width - STANDARD_SCENE_WIDTH) / 2,(winSize.height - STANDARD_SCENE_HEIGHT) / 2);
    std::string filePath = getFilePath("NewProject_7",kDirTypeUI,kFileSuffixJson);
	Layout* task_root = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
	m_pLoginTouchGroup = UILayer::create();
    m_pLoginTouchGroup->setPosition(m_UIInitPosition);
    addChild(m_pLoginTouchGroup);
    m_pLoginTouchGroup->addWidget(task_root);
    
    UIPanel *m_pPanel = static_cast<UIPanel*>(UIHelper::seekWidgetByName(task_root, "Panel0"));
    m_pPanel->setVisible(true);
	m_pPanelLogin = static_cast<UIPanel*>(UIHelper::seekWidgetByName(task_root, "Panel0_0"));
    
    // 版本号
    Label * label = static_cast<Label *>(UIHelper::seekWidgetByName(task_root, "Label_0_0"));
    label->setText(Config::getInstance()->getVersion());
    label->setColor(ccCUSTOM);

	m_pPanelChooseServer = static_cast<UIPanel*>(UIHelper::seekWidgetByName(task_root, "Panel0_1"));
	m_pPanelLoginWithAccount = static_cast<UIPanel*>(UIHelper::seekWidgetByName(task_root, "Panel0_2"));
	m_pPanelRegister = static_cast<UIPanel*>(UIHelper::seekWidgetByName(task_root, "Panel0_3"));
	m_pPanelLostPassword = static_cast<UIPanel*>(UIHelper::seekWidgetByName(task_root, "Panel0_4"));
	m_pPanelChooseRole = static_cast<UIPanel*>(task_root->getChildByName("Panel1"));
    m_pPanelChooseRole->setTouchEnabled(true);
    m_pPanelCreateRole= static_cast<UIPanel*>(task_root->getChildByName("Panel2"));  //创建角色页面屏蔽 CreateRoleUI界面使用
    
	m_pPanelCurrent = m_pPanelChooseServer;
    
    //隐藏维护按钮
    UIPanel* panel1 = static_cast<UIPanel *>(m_pPanelChooseServer->getChildByName("Panel1"));
    UIButton* btn0  = static_cast<UIButton *>(panel1->getChildByName("Button0"));
    UIImageView* img1 = static_cast<UIImageView *>(btn0->getChildByName("ImageView_1"));
    img1->setVisible(false);

	showPanel(m_pPanelLogin,false);
	showPanel(m_pPanelChooseServer, false);
	showPanel(m_pPanelLoginWithAccount,false);
	showPanel(m_pPanelRegister,false);
	showPanel(m_pPanelLostPassword,false);
	showPanel(m_pPanelChooseRole,false);
    showPanel(m_pPanelCreateRole,false);

	//init ui
	initPanelLogin();
	initPanelLoginWithAccount();
	initPanelChooseServer();
	initPanelLostPassword();
	initPanelRegister();
	initPanelChooseRole();
    initPanelCreateRole();
	return true;
}

//
//初始化登陆界面
//
void LoginUI::initPanelLogin()
{
	addWidgetTouchEvent(m_pPanelLogin, "Button_begin", this, toucheventselector(LoginUI::onBtnFastLogin));
	addWidgetTouchEvent(m_pPanelLogin, "Button_login", this, toucheventselector(LoginUI::onBtnLoginWithAccount));
	addWidgetTouchEvent(m_pPanelLogin, "ImageView0", this, toucheventselector(LoginUI::onBtnChooseServer));
	
	showServerList();
    setSelectedServerAndRecentServerList();
}

void LoginUI::showServerList()
{
    UIPanel *m_pPanelAllServer = static_cast<UIPanel*>(UIHelper::seekWidgetByName(m_pPanelChooseServer, "Panel2"));
    ScrollView *scrollView = static_cast<ScrollView *>(UIHelper::seekWidgetByName(m_pPanelAllServer, "ScrollView0"));
    for (int i = 0; i < scrollView->getChildrenCount(); i++)
    {
        UIButton *button = static_cast<UIButton *>(scrollView->getChildByName(CCString::createWithFormat("Button%d",i)->getCString()));
        button->setVisible(false);
        button->setTag(i);
        if (i < Config::getInstance()->getGameServerCount())
        {
            CCString *str = Config::getInstance()->getGameServer(i);
            ServerItem *serverItem = new ServerItem(str->getCString());
            serverItem->autorelease();
            button->setUserObject(serverItem);
            button->setVisible(true);
            button->setTitleText(serverItem->serverName);
            ImageView *imageView_0 = static_cast<ImageView *>(button->getChildByName("ImageView_0"));
            if (imageView_0)
            {
                imageView_0->setVisible(serverItem->isNew);
            }
            button->addTouchEventListener(this, toucheventselector(LoginUI::onBtnAllServerSelected));
        }
    }
}

/**
 * 设置选中的服务器并修改近期访问过的服务器列表
 */
void LoginUI::setSelectedServerAndRecentServerList()
{
    UIPanel *m_pPanelRecentServer = static_cast<UIPanel*>(UIHelper::seekWidgetByName(m_pPanelChooseServer, "Panel1"));
    for (int i = 0; i < RECENT_SERVER_MAX; i++)
    {
        UIButton *button = static_cast<UIButton *>(m_pPanelRecentServer->getChildByName(CCString::createWithFormat("Button%d",i)->getCString()));
        button->setVisible(false);
        button->setTag(i);
        if (i < Config::getInstance()->getRecentServerCount())
        {
            ServerItem *serverItem = new ServerItem(Config::getInstance()->getRecentServer(i).c_str());
            serverItem->autorelease();
            button->setUserObject(serverItem);
            button->setVisible(true);
            button->setTitleText(serverItem->serverName);
            ImageView *imageView_0 = static_cast<ImageView *>(button->getChildByName("ImageView_0"));
            imageView_0->setVisible(serverItem->isNew);
            button->addTouchEventListener(this, toucheventselector(LoginUI::onBtnRecentServerSelected));
            if (i == 0)
            {
                Controller::getInstance()->setSelectedServer(serverItem);
            }
        }
    }
    
    UILabel *label = (UILabel *)getChildWidgetByName(m_pPanelLogin, "Label");
	if(label)
	{
        if (Controller::getInstance()->getSelectedServer())
        {
            label->setText(Controller::getInstance()->getSelectedServer()->serverName);
        }
		else
            label->setText("");
	}
}
//
// 初始化账号登陆界面
//
void LoginUI::initPanelLoginWithAccount()
{
	ImageView *img = static_cast<ImageView*>(UIHelper::seekWidgetByName(m_pPanelLoginWithAccount, "ImageView0_0"));
	if(img)
	{
		CCSize size = CCSizeMake(img->getSize().width - 60, img->getSize().height);
		m_pAccountTF = createEditBox(this,img,size,20,g_sInputAccount);
		m_pAccountTF->setText(Config::getInstance()->getUserID().c_str());
	}
    
	img = static_cast<ImageView*>(UIHelper::seekWidgetByName(m_pPanelLoginWithAccount, "ImageView1_0"));
	if(img)
	{
		// 密码文本显示区域
		m_pPasswordTF = createEditBox(this,img,CCSizeMake(200, 45),36,g_sInputPassword);
		m_pPasswordTF->setInputFlag(kEditBoxInputFlagPassword);
		m_pPasswordTF->setText(Config::getInstance()->getPassword().c_str());
	}
    
	addWidgetTouchEvent(m_pPanelLoginWithAccount, "Button_back", this, toucheventselector(LoginUI::onBtnBack));
	addWidgetTouchEvent(m_pPanelLoginWithAccount, "Button0", this, toucheventselector(LoginUI::onBtnRegisterNewAccount));
	addWidgetTouchEvent(m_pPanelLoginWithAccount, "Button1", this, toucheventselector(LoginUI::onBtnGameStart));
	addWidgetTouchEvent(m_pPanelLoginWithAccount, "ImageView1_3", this, toucheventselector(LoginUI::onBtnLostPassword));
    
    //暂时屏蔽保存账号功能
	//addWidgetTouchEvent(m_pPanelLoginWithAccount, "ImageView1_2", this, toucheventselector(LoginUI::onBtnSaveAccount));
	//addWidgetTouchEvent(m_pPanelLoginWithAccount, "ImageView1_1", this, toucheventselector(LoginUI::onBtnSaveAccount));
    
	img = static_cast<ImageView*>(UIHelper::seekWidgetByName(m_pPanelLoginWithAccount, "ImageView1_2"));
	if(img)
	{
		img->setEnabled(false);
        img->setVisible(false);
	}
    img = static_cast<ImageView*>(UIHelper::seekWidgetByName(m_pPanelLoginWithAccount, "ImageView1_1"));
	if(img)
	{
		img->setEnabled(false);
        img->setVisible(false);
	}

	setWidgetVisible(m_pPanelLoginWithAccount, "Panel2_0.ImageView0.ImageView0_0.TextField_0", false);
	setWidgetVisible(m_pPanelLoginWithAccount, "Panel2_0.ImageView1.ImageView1_0.TextField_0", false);
}

void LoginUI::initPanelChooseServer()
{
	addWidgetTouchEvent(m_pPanelChooseServer, "Button_closed", this, toucheventselector(LoginUI::onBtnBack));
	UIPanel *m_pPanelRecentServer = static_cast<UIPanel*>(UIHelper::seekWidgetByName(m_pPanelChooseServer, "Panel1"));
    for (int i = 0; i < RECENT_SERVER_MAX; i++)
    {
        UIButton *button = static_cast<UIButton *>(m_pPanelRecentServer->getChildByName(CCString::createWithFormat("Button%d",i)->getCString()));
        button->setVisible(false);
        button->setTag(i);
        if (i < Config::getInstance()->getRecentServerCount())
        {
            ServerItem *serverItem = static_cast<ServerItem *>(button->getUserObject());
            if (!serverItem)
            {
                serverItem = new ServerItem(Config::getInstance()->getRecentServer(i).c_str());
                serverItem->autorelease();
                button->setUserObject(serverItem);
            }
            button->setVisible(true);
            button->setTitleText(serverItem->serverName);
            ImageView *imageView_0 = static_cast<ImageView *>(button->getChildByName("ImageView_0"));
            imageView_0->setVisible(serverItem->isNew);
            button->addTouchEventListener(this, toucheventselector(LoginUI::onBtnRecentServerSelected));
        }
    }
	UIPanel *m_pPanelAllServer = static_cast<UIPanel*>(UIHelper::seekWidgetByName(m_pPanelChooseServer, "Panel2"));
    ScrollView *scrollView = static_cast<ScrollView *>(UIHelper::seekWidgetByName(m_pPanelAllServer, "ScrollView0"));
    for (int i = 0; i < scrollView->getChildrenCount(); i++)
    {
        UIButton *button = static_cast<UIButton *>(scrollView->getChildByName(CCString::createWithFormat("Button%d",i)->getCString()));
        button->setVisible(false);
        if (i < Config::getInstance()->getGameServerCount())
        {
            ServerItem *serverItem = new ServerItem(Config::getInstance()->getGameServer(i)->getCString());
            serverItem->autorelease();
            button->setUserObject(serverItem);
            button->setVisible(true);
            button->setTitleText(serverItem->serverName);
            ImageView *imageView_0 = static_cast<ImageView *>(button->getChildByName("ImageView_0"));
            if (imageView_0)
            {
                imageView_0->setVisible(serverItem->isNew);
            }
            button->addTouchEventListener(this, toucheventselector(LoginUI::onBtnAllServerSelected));
        }
    }
}

void LoginUI::setCSButtonVisible(int row,int col,bool visible)
{
	m_pCSButton[row][col]->setVisible(visible);
}

void LoginUI::initPanelLostPassword()
{
	ImageView *img = static_cast<ImageView*>(UIHelper::seekWidgetByName(m_pPanelLostPassword, "ImageView_write"));
	if(img)
	{
		m_pLostPasswordMailTF = createEditBox(this,img,CCSizeMake(320, 45),40,NULL);
	}
	setWidgetVisible(m_pPanelLostPassword, "ImageView_write.TextField_0", false);
    
	addWidgetTouchEvent(m_pPanelLostPassword, "Button_done",this, toucheventselector(LoginUI::onBtnSendPassword));
	addWidgetTouchEvent(m_pPanelLostPassword, "Button_back",this, toucheventselector(LoginUI::onBtnBack));
}

/**
 * 初始化注册界面
 */
void LoginUI::initPanelRegister()
{
	setWidgetVisible(m_pPanelRegister, "Panel1_0.ImageView0.ImageView0_0.TextField_0", false);
	setWidgetVisible(m_pPanelRegister, "Panel1_0.ImageView1.ImageView0_0.TextField_0", false);
	setWidgetVisible(m_pPanelRegister, "Panel1_0.ImageView2.ImageView0_0.TextField_0", false);
	setWidgetVisible(m_pPanelRegister, "Panel1_0.ImageView3.ImageView0_0.TextField_0", false);
    
	addWidgetTouchEvent(m_pPanelRegister, "Button_begin",this, toucheventselector(LoginUI::onBtnRegister));
	addWidgetTouchEvent(m_pPanelRegister, "Button_back",this, toucheventselector(LoginUI::onBtnBack));
    
	ImageView *img = static_cast<ImageView*>(getChildWidgetByName(m_pPanelRegister, "Panel1_0.ImageView0.ImageView0_0"));
	if(img)
	{
		m_pNewAccountTF = createEditBox(this,img,CCSizeMake(200, 45),16,g_sInputAccount);
	}
	img = static_cast<ImageView*>(getChildWidgetByName(m_pPanelRegister, "Panel1_0.ImageView1.ImageView0_0"));
	if(img)
	{
		m_pNewPasswordTF = createEditBox(this,img,CCSizeMake(200, 45),16,g_sInputPassword);
		m_pNewPasswordTF->setInputFlag(kEditBoxInputFlagPassword);
	}
	img = static_cast<ImageView*>(getChildWidgetByName(m_pPanelRegister, "Panel1_0.ImageView2.ImageView0_0"));
	if(img)
	{
		m_pNewPassword2TF = createEditBox(this,img,CCSizeMake(200, 45),16,g_sInputPassword);
		m_pNewPassword2TF->setInputFlag(kEditBoxInputFlagPassword);
	}
	img = static_cast<ImageView*>(getChildWidgetByName(m_pPanelRegister, "Panel1_0.ImageView3.ImageView0_0"));
	if(img)
	{
		m_pNewAccountMailTF = createEditBox(this,img,CCSizeMake(200, 45),64,g_sInputMail);
        m_pNewAccountMailTF->setInputMode(kEditBoxInputModeEmailAddr);
	}
}

/**
 *初始化角色选择界面
 */
void LoginUI::initPanelChooseRole()
{
	addWidgetTouchEvent(m_pPanelChooseRole, "Button_back",this, toucheventselector(LoginUI::onBtnChooseRoleBack));
    addWidgetTouchEvent(m_pPanelChooseRole, "Button_begin",this, toucheventselector(LoginUI::onBtnRoleStartGame));
    addWidgetTouchEvent(m_pPanelChooseRole, "Button_delete",this, toucheventselector(LoginUI::onBtnRoleDelete));
    for(int i = 0;i < ROLE_MAX;i++)
	{
		showRolePanel(i, Controller::getInstance()->getRoleInfo(i));
        setDeleteInfo(i);
	}
    setSelectedRole(0);
}

/**
 *初始化角色创建界面
 */
void LoginUI::initPanelCreateRole()
{
    addWidgetTouchEvent(m_pPanelCreateRole, "Button_back",this, toucheventselector(LoginUI::menuReturnCallback));
    addWidgetTouchEvent(m_pPanelCreateRole, "Button_go",this, toucheventselector(LoginUI::menuStartCallback));
    addWidgetTouchEvent(m_pPanelCreateRole, "Button_dice",this, toucheventselector(LoginUI::btnGenRoleName));
    //    buttonDice->setSize(CCSize(28.0f, 27.0f));
    
    //人物职业选择
    {
        Layout* panel0 = static_cast<Layout*>(m_pPanelCreateRole->getChildByName("Panel0"));
        for (int i =0 ; i < 3; i++)
        {
            UIImageView* imgView = static_cast<UIImageView*>(panel0->getChildByName(CCString::createWithFormat("ImageView_%d",i)->getCString()));
            imgView->setTag(i);
            imgView->setTouchEnabled(true);
            imgView->addTouchEventListener(this, toucheventselector(LoginUI::menuChooseRoleCallback));
            
            //动画
            if (i == 2)
            {
                
            }
            else
            {
                UIImageView* imgView0 = static_cast<UIImageView*>(m_pPanelCreateRole->getChildByName("ImageView_0"));
                initRoleAnimation(i,imgView0);
            }
        }
    }
    
    //输入框 cocostudio 自带输入框不好用 自己添加box
    UIImageView* input_view = static_cast<UIImageView*>(m_pPanelCreateRole->getChildByName("ImageView_1"));
    std::string filePath = getFilePath("223",kDirTypeUI,kFileSuffixPng);
    m_pInputName = CCEditBox::create(CCSizeMake(320.0f, 55.0f),CCScale9Sprite::create(filePath.c_str()));
    m_pInputName->setPosition(input_view->getPosition());
    m_pInputName->setFont("Helvetica-Bold",25);
    m_pInputName->setFontColor(ccRED);
    m_pInputName->setPlaceholderFontColor(ccWHITE);
    m_pInputName->setMaxLength(50);
    m_pInputName->setReturnType(kKeyboardReturnTypeDone);
    m_pInputName->setAnchorPoint(ccp(0.5f,0.5f));
    m_pInputName->setPlaceHolder(Controller::getInstance()->getRandomName().c_str());
    m_pInputName->setText(Controller::getInstance()->getRandomName().c_str());
    m_pInputName->setTouchEnabled(true);
    m_pInputName->setInputMode(kEditBoxInputModeSingleLine);
    m_pInputName->setTouchPriority(0);
    m_pPanelCreateRole->addNode(m_pInputName);
    m_pPanelCreateRole->removeChild(input_view,true);
    m_pPanelCreateRole->setTouchEnabled(true);
    
    updateRole(0);
}

/**
 * 设置选中角色
 */
void LoginUI::setSelectedRole(int roleIndex)
{
    Controller::getInstance()->setRoleIndex(roleIndex);
    
    setStartButtonEnable(Controller::getInstance()->isRoleCreated(roleIndex) && Controller::getInstance()->getRoleInfo(roleIndex)->deleteTime == 0);//是否可以开始游戏
    bool isRoleCreated = Controller::getInstance()->isRoleCreated(roleIndex);
    //setCreateButtonEnable(!isRoleCreated);//是否可以创建角色 创建角色按钮已删除
    setDeleteButtonEnable(isRoleCreated && Controller::getInstance()->getRoleInfo(roleIndex)->deleteTime == 0);//是否可以删除角色
    
    //更新光环
    m_nHaloPlayingIdx = roleIndex;
    for (int i = 0; i < 3; i++)
    {
        if (m_nHaloPlayingIdx == i && m_bRolePlaying[i])
        {
            if(m_pHaloAni1[i] && m_pHaloAni2[i])
            {
                m_pHaloAni1[i]->setVisible(true);
                m_pHaloAni2[i]->setVisible(true);
            }
        }
        else
        {
            if(m_pHaloAni1[i] && m_pHaloAni2[i])
            {
                m_pHaloAni1[i]->setVisible(false);
                m_pHaloAni2[i]->setVisible(false);
            }
        }
    }
}

void LoginUI::showRolePanel(int panelIndex, void *data)
{
	RoleInfo *role = (RoleInfo *)data;
    
    UIPanel *imgRolePanel = static_cast<UIPanel*>(m_pPanelChooseRole->getChildByName(CCString::createWithFormat("Panel%d",panelIndex)->getCString()));
    UIImageView *imgBg  = static_cast<UIImageView*>(imgRolePanel->getChildByName("ImageView_0"));
    UIImageView *imgDel = static_cast<UIImageView*>(imgRolePanel->getChildByName("ImageView_2"));
    UIImageView *imgAdd = static_cast<UIImageView*>(imgRolePanel->getChildByName("ImageView_3"));
    
	imgBg->setUserData(data);
    imgBg->setTag(panelIndex);
    imgBg->setVisible(true);
    imgBg->setTouchEnabled(true);
	imgBg->addTouchEventListener(this,toucheventselector(LoginUI::onBtnRoleSelect));
    
    if(role->professionIndex > 0) //是否需要创建角色
	{
        imgDel->setVisible(false);
        imgDel->setEnabled(false);
        imgAdd->setVisible(false);
        //等级
        UILabel *labelLevel = static_cast<UILabel*>(imgRolePanel->getChildByName("Label_level"));
        if(labelLevel)
        {
            UILabel *label = static_cast<UILabel*>(labelLevel->getChildByName("Label_num"));
            char lvl_buf[32];
            sprintf(lvl_buf,"%d", role->roleLevel);
            label->setText(lvl_buf);
            label->setColor(ccCUSTOM);
        }
        //名字
        UILabel *labelName = static_cast<UILabel*>(imgRolePanel->getChildByName("Label_name"));
        if(labelName)
        {
            labelName->setText(role->roleName);
            labelName->setVisible(true);
        }
        //开启角色待机动画
        SpriteData * spriteData = (SpriteData *)GlobalData::getInstance()->getSpriteDataDict()->objectForKey(role->professionIndex);// 精灵基本数据
        m_pRoleAni[panelIndex] = YYSprite::create();
        m_pRoleAni[panelIndex]->initWithActionSet(spriteData->getResourceArray(), kDirTypeSprite,true);
        m_pRoleAni[panelIndex]->setAction(ACTION_STAND0, true);
        m_pRoleAni[panelIndex]->setVisible(true);
        float fOffSetY = imgBg->getContentSize().height * 0.5f;
        m_pRoleAni[panelIndex]->setPosition(ccp(m_pRoleAni[panelIndex]->getPositionX(),m_pRoleAni[panelIndex]->getPositionY()-fOffSetY+80));
        imgBg->addNode(m_pRoleAni[panelIndex],3);
        m_bRolePlaying[panelIndex] = true;
        
        //阴影
        m_pRoleShadow[panelIndex] = UIImageView::create();
        imgBg->addChild(m_pRoleShadow[panelIndex],0);
        std::string filePath = getFilePath("472",kDirTypeUI,kFileSuffixPng);
        m_pRoleShadow[panelIndex]->loadTexture(filePath.c_str());
        m_pRoleShadow[panelIndex]->setPosition(m_pRoleAni[panelIndex]->getPosition());
        
        //光圈
        m_pHaloAni1[panelIndex] = YYSprite::createWithActionSet("guangquan","guangquan",kDirTypeSprite);
        m_pHaloAni1[panelIndex]->setAction(0,true);
        m_pHaloAni1[panelIndex]->setVisible(false);
        m_pHaloAni1[panelIndex]->setPosition(m_pRoleAni[panelIndex]->getPosition());
        imgBg->addNode(m_pHaloAni1[panelIndex],2);
        
        //光环
        m_pHaloAni2[panelIndex] = YYSprite::createWithActionSet("guanghuan","guanghuan",kDirTypeSprite);
        m_pHaloAni2[panelIndex]->setAction(0,true);
        m_pHaloAni2[panelIndex]->setVisible(false);
        m_pHaloAni2[panelIndex]->setPosition(m_pRoleAni[panelIndex]->getPosition());
        imgBg->addNode(m_pHaloAni2[panelIndex],4);
	}
    else
    {
        //删除角色 隐藏等级 名字
        UILabel *labelLevel = static_cast<UILabel*>(imgRolePanel->getChildByName("Label_level"));
        labelLevel->setVisible(false);
        imgAdd->setVisible(true);
        
        UILabel *labelNameEx = static_cast<UILabel*>(imgRolePanel->getChildByName("Label_name"));
        labelNameEx->setVisible(false);
        
        //删除角色时 清除动画
        if (m_bRolePlaying[panelIndex] && m_pRoleAni[panelIndex])
        {
            m_bRolePlaying[panelIndex] = false;
            imgBg->removeChild(m_pRoleAni[panelIndex]);
            m_pRoleAni[panelIndex] = NULL;
        }
        
        //删除角色时 隐藏光圈 光环 阴影
        if (m_pHaloAni1[panelIndex])
        {
            m_pHaloAni1[panelIndex]->removeFromParent();
        }
        if ( m_pHaloAni2[panelIndex])
        {
            m_pHaloAni2[panelIndex]->removeFromParent();
        }
        if (m_pRoleShadow[panelIndex])
        {
            m_pRoleShadow[panelIndex]->removeFromParent();
        }
    }
    
    //隐藏删除面板
    ImageView *imageView = static_cast<ImageView *>(imgRolePanel->getChildByName("ImageView_2"));
    imageView->setVisible(false);
    
    //删除角色过程中点击, "开始游戏" 按钮置灰
    if(role->deleteTime > 0)
    {
        setStartButtonEnable(false);
    }
}

void LoginUI::setDeleteInfo(int panelIndex)
{
    UIPanel *imgRolePanel = static_cast<UIPanel*>(m_pPanelChooseRole->getChildByName(CCString::createWithFormat("Panel%d",panelIndex)->getCString()));
    ImageView *imageView = static_cast<ImageView *>(imgRolePanel->getChildByName("ImageView_2"));
    RoleInfo *roleInfo = Controller::getInstance()->getRoleInfo(panelIndex);
    if (roleInfo->deleteTime > 0)
    {
        setDeleteButtonEnable(false);
        setStartButtonEnable(false);
        imageView->setVisible(true);
        imageView->setEnabled(true);
        m_nCurrentTime[panelIndex] = Controller::getInstance()->getCurrentSecond();
        Label *label_0 = static_cast<Label *>(imageView->getChildByName("Label_0"));
        m_pDeleteTimeLabel[panelIndex] = static_cast<Label *>(label_0->getChildByName("Label_0"));
        setDeleteTime(roleInfo->deleteTime,panelIndex);
    }
    else
    {
        setDeleteButtonEnable(true);
        setStartButtonEnable(true);
        imageView->setVisible(false);
        imageView->setEnabled(false);
        if(roleInfo->deleteTime == -1)
        {
            //删除角色 隐藏等级 名字
            UILabel *labelLevel = static_cast<UILabel*>(imgRolePanel->getChildByName("Label_level"));
            labelLevel->setVisible(false);
            UIImageView *imgAdd = static_cast<UIImageView*>(imgRolePanel->getChildByName("ImageView_3"));
            imgAdd->setVisible(true);
            UILabel *labelNameEx = static_cast<UILabel*>(imgRolePanel->getChildByName("Label_name"));
            labelNameEx->setVisible(false);
            
            //删除角色时 清除动画
            if (m_bRolePlaying[panelIndex] && m_pRoleAni[panelIndex])
            {
                m_bRolePlaying[panelIndex] = false;
                UIImageView *imgBg  = static_cast<UIImageView*>(imgRolePanel->getChildByName("ImageView_0"));
                imgBg->removeChild(m_pRoleAni[panelIndex]);
            }
            
            //删除角色时 隐藏光圈 光环 阴影
            if (m_pHaloAni1[panelIndex])
            {
                m_pHaloAni1[panelIndex]->removeFromParent();
            }
            if ( m_pHaloAni2[panelIndex])
            {
                m_pHaloAni2[panelIndex]->removeFromParent();
            }
            if (m_pRoleShadow[panelIndex])
            {
                m_pRoleShadow[panelIndex]->removeFromParent();
            }
        }
    }
}

void LoginUI::setDeleteTime(int deleteTime,int panelIdx)
{
    if (m_pDeleteTimeLabel[panelIdx])
    {
        int hour = deleteTime / 3600;
        int minute = deleteTime / 60 % 60;
        int second = deleteTime % 60;
        const char *time = CCString::createWithFormat("%d:%d:%d",hour,minute,second)->getCString();
        if (minute < 10)
        {
            if (second < 10)
            {
                time = CCString::createWithFormat("%d:0%d:0%d",hour,minute,second)->getCString();
            }
            else
            {
                time = CCString::createWithFormat("%d:0%d:%d",hour,minute,second)->getCString();
            }
        }
       // CCLOG("panelidx %d time %s",panelIdx,time);
        m_pDeleteTimeLabel[panelIdx]->setText(time);
    }
}

void LoginUI::updateDeleteTime()
{
    if (m_ePanelType != kPanelTypeChooseRole)
    {
        return;
    }
    for (int i = 0; i < 3; i++)
    {
        if (Controller::getInstance()->getRoleInfo(i)->deleteTime > 0)
        {
            if (Controller::getInstance()->getCurrentSecond() != m_nCurrentTime[i])
            {
                m_nCurrentTime[i] = Controller::getInstance()->getCurrentSecond();
                Controller::getInstance()->getRoleInfo(i)->deleteTime--;
                setDeleteTime(Controller::getInstance()->getRoleInfo(i)->deleteTime,i);
                if (Controller::getInstance()->getRoleInfo(i)->deleteTime == 0)
                {
                    CCString *str = CCString::createWithFormat(ASK_ROLE_DELETE_SURPLUS, i+1);
                    Controller::getInstance()->sendMessage(str->getCString(), true);
                }
            }
        }
    }
}

void LoginUI::onEnter()
{
	CCNode::onEnter();
    
	if(m_ePanelType > 0)
	{
		showUILayer(m_ePanelType);
	}
	else
	{
		if(Config::getInstance()->getUserID().empty())	// 首次运行
		{
			showUILayer(kPanelTypeChooseServer);
		}
		else
		{
			showUILayer(kPanelTypeLogin);
		}
	}
}

void LoginUI::onExit()
{
    CCLayer::onExit();
}

void LoginUI::showUILayer(PanelType panelType)
{
	UIPanel* panel;
	if(panelType == kPanelTypeChooseServer)panel = m_pPanelChooseServer;
	else if(panelType == kPanelTypeLogin)panel = m_pPanelLogin;
	else if(panelType == kPanelTypeLoginWithAccount)panel = m_pPanelLoginWithAccount;
	else if(panelType == kPanelTypeChooseRole)panel = m_pPanelChooseRole;
	else if(panelType == kPanelTypeRegister)panel = m_pPanelRegister;
	else if(panelType == kPanelTypeLostPassword)panel = m_pPanelLostPassword;
    else if(panelType == kPanelTypeCreateRole)panel = m_pPanelCreateRole;
	else return;
	
	if(m_pPanelCurrent)
    {
		showPanel(m_pPanelCurrent, false);
		m_pPanelCurrent->setTouchEnabled(false);
	}
	m_pPanelCurrent = panel;
	showPanel(m_pPanelCurrent, true);
    
	setPanelType(panelType);
}

void LoginUI::onBtnBack(CCObject* pSender, TouchEventType type)
{
    if (m_bNetLoading)
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
		showUILayer(kPanelTypeLogin);
    }
}

void LoginUI::onBtnFastLogin(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    
    if (type == TOUCH_EVENT_ENDED)
    {
            const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
        if (!Controller::getInstance()->getSelectedServer())
        {
            ((YYScene*)getParent())->showTips(g_sChooseServer);
            return;
        }
		std::string userID = Config::getInstance()->getUserID();
		std::string password = Config::getInstance()->getPassword();
        std::string sPhoneInfo = Config::getInstance()->getPhoneInfo();
		if(userID.empty())
		{
            std::string message = Protocol::getRegisterMessage(Config::getInstance()->getLoginInfo());
            if (ODSocket::getInstance()->getNetStatus() == kNetStatusConnected)
            {
                Controller::getInstance()->sendMessage(message.c_str(),true);
            }
            else if (ODSocket::getInstance()->getNetStatus() == kNetStatusDisConnect)
            {
                ODSocket::getInstance()->setNetStatus(kNetStatusConnecting);
                Controller::getInstance()->setConnectMessage(message);
                Controller::getInstance()->connectRegister();
            }
		}
        else
        {
            std::string message = Protocol::getLoginMessage(Config::getInstance()->getLoginInfo());
            if (ODSocket::getInstance()->getNetStatus() == kNetStatusDisConnect)
            {
                ODSocket::getInstance()->setNetStatus(kNetStatusConnecting);
                Controller::getInstance()->setConnectMessage(message);
                Controller::getInstance()->connectRegister();
            }
            else if (ODSocket::getInstance()->getNetStatus() == kNetStatusConnected)
            {
                Controller::getInstance()->sendMessage(message.c_str(),false);
            }
        }
//        Controller::getInstance()->switchGameStatus(kStatusLoading);
	}
}

void LoginUI::setLoginEditBoxEnable(bool isTouchEnable)
{
    m_pAccountTF->setTouchEnabled(isTouchEnable);
    m_pPasswordTF->setTouchEnabled(isTouchEnable);
}

void LoginUI::setRegisterEditBoxEnable(bool isTouchEnable)
{
    m_pNewAccountTF->setTouchEnabled(isTouchEnable);
    m_pNewPasswordTF->setTouchEnabled(isTouchEnable);
    m_pNewPassword2TF->setTouchEnabled(isTouchEnable);
    m_pNewAccountMailTF->setTouchEnabled(isTouchEnable);
}

void LoginUI::onBtnRegister(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED){
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
        std::string userID = m_pNewAccountTF->getText();
		std::string password = m_pNewPasswordTF->getText();
		std::string password2 = m_pNewPassword2TF->getText();
		std::string mail = m_pNewAccountMailTF->getText();
		if(userID.length() < 3 || userID.length() > 9)
		{
			((YYScene*)getParent())->showTips(g_sAccountLenErr);
            setRegisterEditBoxEnable(false);
			return;
		}
		else if(password.length() < 6 || password.length() > 9)
		{
			((YYScene*)getParent())->showTips(g_sPasswordLenErr);
            setRegisterEditBoxEnable(false);
			return;
		}
		else if(password.compare(password2) != 0)
		{
			((YYScene*)getParent())->showTips(g_sPasswordNotMatch);
            setRegisterEditBoxEnable(false);
			return;
		}
        else
        {
            const char* strUsrID = userID.c_str();
            for (int i = 0; i < userID.length(); i++)
            {
                char ch = strUsrID[i];
                if(!(('0'<= ch && ch <= '9') || ('a'<= ch && ch <= 'z') || ('A'<= ch && ch <= 'Z')))
                {
                    ((YYScene*)getParent())->showTips(g_sAccountSpecial);
                    setRegisterEditBoxEnable(false);
                    return;
                }
            }
            const char* strPassword = password.c_str();
            for (int i = 0; i < password.length(); i++)
            {
                char ch = strPassword[i];
                if(!(('0'<= ch && ch <= '9') || ('a'<= ch && ch <= 'z') || ('A'<= ch && ch <= 'Z')))
                {
                    ((YYScene*)getParent())->showTips(g_sPasswordSpecial);
                    setRegisterEditBoxEnable(false);
                    return;
                }
            }
        }
		Config::getInstance()->setUserID(userID);
		Config::getInstance()->setPassword(password);
        std::string sPhoneInfo = Config::getInstance()->getPhoneInfo();
        if (!mail.empty())
        {
            Config::getInstance()->getLoginInfo().mail = m_pNewAccountMailTF->getText();
        }
        std::string message = Protocol::getRegisterMessage(Config::getInstance()->getLoginInfo());

        if(ODSocket::getInstance()->getNetStatus() == kNetStatusConnected)
        {
            Controller::getInstance()->sendMessage(message.c_str(),true);
        }
        else if(ODSocket::getInstance()->getNetStatus() == kNetStatusDisConnect)
        {
            ODSocket::getInstance()->setNetStatus(kNetStatusConnecting);
            Controller::getInstance()->setConnectMessage(message.c_str());
            Controller::getInstance()->connectRegister();
        }
	}
}

void LoginUI::onBtnGameStart(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
        if (!Controller::getInstance()->getSelectedServer())
        {
            ((YYScene*)getParent())->showTips(g_sChooseServer);
            return;
        }
		std::string userID = m_pAccountTF->getText();
		std::string password = m_pPasswordTF->getText();
		Config::getInstance()->setUserID(userID);
		Config::getInstance()->setPassword(password);
        std::string message = Protocol::getLoginMessage(Config::getInstance()->getLoginInfo());
         CCLOG("33333");
        if (ODSocket::getInstance()->getNetStatus() == kNetStatusConnected)
        {
            Controller::getInstance()->sendMessage(message.c_str(),true);
        }
        else if (ODSocket::getInstance()->getNetStatus() == kNetStatusDisConnect)
        {
            ODSocket::getInstance()->setNetStatus(kNetStatusConnecting);
            Controller::getInstance()->setConnectMessage(message.c_str());
            Controller::getInstance()->connectRegister();
        }
	}
}

void LoginUI::onBtnLoginWithAccount(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
        if (!Controller::getInstance()->getSelectedServer())
        {
            ((YYScene*)getParent())->showTips(g_sChooseServer);
            return;
        }
		showUILayer(kPanelTypeLoginWithAccount);
	}
}

void LoginUI::onBtnRegisterNewAccount(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
        showUILayer(kPanelTypeRegister);
	}
}

//
///**
// * 确认提示信息
// */
//void LoginUI::confirmTips(CCObject* pSender, cocos2d::gui::TouchEventType type)
//{
//    if (Controller::getInstance()->getScene()->isNetLoading())
//    {
//        return;
//    }
//    if (type == TOUCH_EVENT_ENDED)
//    {
//        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
//        PLAY_EFFECT(filePath.c_str());
//        m_pTipPanel->setEnabled(false);
//        m_pTipPanel->setVisible(false);
//        m_pTipPanel->setTouchEnabled(false);
//        if (m_eTipType == kTipExit)
//        {// 退出提示
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//            JniMethodInfo minfo;//定义Jni函数信息结构体
//            bool isExist = JniHelper::getStaticMethodInfo(minfo, "com/yourcompany/fairyland/FairyLand", "exitGame", "()V");
//            if (isExist)
//            {
//                minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);     //调用函数
//                minfo.env->DeleteLocalRef(minfo.classID);
//            }
//#endif
//        }
//        else if(m_eTipType == kTipRoleDeleted)
//        {
//            const char * cmd = CCString::createWithFormat(ASK_ROLE_DELETE,Controller::getInstance()->getRoleIndex() + 1)->getCString();
//            Controller::getInstance()->sendMessage(cmd,true);
//        }
//        else if(m_eTipType == kTipRoleDeletedCancel)
//        {
//            const char * cmd = CCString::createWithFormat(ASK_ROLE_DELETE_CANCEL,Controller::getInstance()->getRoleIndex() + 1)->getCString();
//            Controller::getInstance()->sendMessage(cmd,true);
//        }
//    }
//}
//
//void LoginUI::cancelTips(CCObject* pSender, cocos2d::gui::TouchEventType type)
//{
//    if (Controller::getInstance()->getScene()->isNetLoading())
//    {
//        return;
//    }
//    if (type == TOUCH_EVENT_BEGAN)
//    {
//        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
//        PLAY_EFFECT(filePath.c_str());
//        m_pTipPanel->setEnabled(false);
//        m_pTipPanel->setVisible(false);
//        m_pTipPanel->setTouchEnabled(false);
//    }
//}

void LoginUI::onBtnSaveAccount(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
		Widget *widget = getChildWidgetByName(m_pPanelLoginWithAccount, "ImageView1_1_0");
		if(widget)
		{
			widget->setVisible(!widget->isVisible());
		}
    }
}

void LoginUI::onBtnLostPassword(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
		showUILayer(kPanelTypeLostPassword);
    }
}

void LoginUI::onBtnSendPassword(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
		showUILayer(kPanelTypeLogin);
    }
}

void LoginUI::onBtnChooseServer(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
        if (Config::getInstance()->getGameServerCount() > 0)
        {
            showServerList();
            showUILayer(kPanelTypeChooseServer);
        }
        else
        {
            CCString *msg = CCString::createWithFormat(ASK_SERVER_LIST,Config::getInstance()->getChannel().c_str());
            Controller::getInstance()->setConnectMessage(msg->getCString());
            Controller::getInstance()->connectRegister();
        }
	}
}

void LoginUI::onBtnRecentServerSelected(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
		UIButton *button = (UIButton *)pSender;
		ServerItem *item = (ServerItem *)button->getUserObject();
		if(item)
		{
            Controller::getInstance()->setSelectedServer(item);
            int tag = button->getTag();
            if (tag != 0)
            {
                std::string temp = Config::getInstance()->getRecentServer(tag);
                Config::getInstance()->sortRecentServerList(temp.c_str(), tag) ;
                Config::getInstance()->saveConfig();
            }
            setSelectedServerAndRecentServerList();
		}
        showUILayer(kPanelTypeLogin);
	}
}

void LoginUI::onBtnAllServerSelected(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
		UIButton *button = (UIButton *)pSender;
		ServerItem *item = (ServerItem *)button->getUserObject();
		if(item)
		{
            Controller::getInstance()->setSelectedServer(item);
            CCString * str = Config::getInstance()->getGameServer(button->getTag());
            Config::getInstance()->addServerInofToList(str->getCString());
            setSelectedServerAndRecentServerList();
		}
		showUILayer(kPanelTypeLogin);
	}
}

void LoginUI::onBtnRoleDelete(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (m_bNetLoading)
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
	{
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
        ((YYScene*)getParent())->showTips(g_sRoleDeletedTip,kTipRoleDeleted);
    }
}

void LoginUI::onBtnRoleStartGame(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (m_bNetLoading)
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
	{
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
		Controller::getInstance()->chooseRole();
    }
}

void LoginUI::onBtnRoleSelect(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (m_bNetLoading)
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
	{
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
		UIImageView *imgBG = static_cast<UIImageView*>(pSender);
        setSelectedRole(imgBG->getTag());
        bool isRoleCreated = Controller::getInstance()->isRoleCreated(imgBG->getTag());
        if (!isRoleCreated) //创建角色
        {
            CCString *msg = CCString::createWithFormat(ASK_RANDOM_NAME,3);
            Controller::getInstance()->sendMessage(msg->getCString(),true);
            //            Controller::getInstance()->switchGameStatus(kStatusCreateRole);
            showUILayer(kPanelTypeCreateRole);
        }
        else if (Controller::getInstance()->getRoleInfo(imgBG->getTag())->deleteTime > 0) //取消删除角色
        {
            ((YYScene*)getParent())->showTips(g_sRoleDeletedCancelTip,kTipRoleDeletedCancel);
        }
    }
}

/**
 * 设置开始游戏选项是否起作用(未创建人物角色时无法开始游戏)
 */
void LoginUI::setStartButtonEnable(bool isEnable)
{
    Button *startGameButton = static_cast<Button *>(getChildWidgetByName(m_pPanelChooseRole, "Button_begin"));
    startGameButton->setTouchEnabled(isEnable);
    startGameButton->setBright(isEnable);
}
/**
 * 设置删除角色选项是否起作用(未创建人物角色时无效)
 */
void LoginUI::setDeleteButtonEnable(bool isEnable)
{
    Button *deleteButton = static_cast<Button *>(getChildWidgetByName(m_pPanelChooseRole, "Button_delete"));
    deleteButton->setTouchEnabled(isEnable);
    deleteButton->setBright(isEnable);
}

/**
 * 设置创建角色选项是否起作用(已创建角色时无效)
 */
void LoginUI::setCreateButtonEnable(bool isEnable)
{
    Button *createButton = static_cast<Button *>(getChildWidgetByName(m_pPanelChooseRole, "Button_create"));
    createButton->setTouchEnabled(isEnable);
    createButton->setBright(isEnable);
}

void LoginUI::onBtnChooseRoleBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
	{
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
		Controller::getInstance()->askLogout(true);
    }
}

void LoginUI::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    //     CCLog("editBox %p DidBegin !", editBox);
}

void LoginUI::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    //     CCLog("editBox %p DidEnd !", editBox);
}

void LoginUI::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
    //     CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void LoginUI::editBoxReturn(CCEditBox* editBox)
{
    //     CCLog("editBox %p was returned !");
    
}

void LoginUI::update(float f)
{
    if (m_bNetLoading)
    {
        ((YYSprite *)((CCScene *)getParent())->getChildByTag(10)->getChildByTag(10))->updateAnimation();
    }
    for (int i = 0; i < 3; i++)
    {
        if (m_bRolePlaying[i])
        {
            m_pRoleAni[i]->updateAnimation();
            if (m_nHaloPlayingIdx == i)
            {
                m_pHaloAni1[i]->updateAnimation();
                m_pHaloAni2[i]->updateAnimation();
            }
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if (m_bCreatRolePlaying[i])
        {
            updateRoleAction(i);
        }
    }
    updateDeleteTime();
}

extern std::string trim(std::string& str) ;

void LoginUI::setInputNameTouchEnabled(bool b)
{
    std::string roleName = m_pInputName->getText();
    if(roleName.empty())
        roleName = m_pInputName->getPlaceHolder();
    roleName = trim(roleName);
    CCPoint cp = m_pInputName->getPosition();
    
    m_pInputName->removeFromParentAndCleanup(true);
    m_pInputName = NULL;
    
    std::string filePath = getFilePath("223",kDirTypeUI,kFileSuffixPng);
    m_pInputName = CCEditBox::create(CCSizeMake(320.0f, 55.0f),CCScale9Sprite::create(filePath.c_str()));
    m_pInputName->setPosition(cp);
    m_pInputName->setFont("Helvetica-Bold",25);
    m_pInputName->setFontColor(ccRED);
    m_pInputName->setPlaceholderFontColor(ccWHITE);
    m_pInputName->setMaxLength(50);
    m_pInputName->setReturnType(kKeyboardReturnTypeDone);
    m_pInputName->setAnchorPoint(ccp(0.5f,0.5f));
    m_pInputName->setPlaceHolder(roleName.c_str());
    m_pInputName->setText(roleName.c_str());
    m_pInputName->setTouchEnabled(true);
    m_pInputName->setInputMode(kEditBoxInputModeSingleLine);
    m_pInputName->setTouchPriority(0);
    m_pPanelCreateRole->addNode(m_pInputName);
    m_pPanelCreateRole->setTouchEnabled(true);
    
}

void LoginUI::menuStartCallback(CCObject* pSender, TouchEventType type)
{
    if(m_bNetLoading)
        return;
    if (type == TOUCH_EVENT_ENDED)
    {
		std::string roleName = m_pInputName->getText();
		if(roleName.empty())
			roleName = m_pInputName->getPlaceHolder();
		roleName = trim(roleName);
        const char * temp = roleName.c_str();
        unsigned char c = (unsigned char)*temp;
        int bytes;
        UTF8_NUMBER_OF_BYTES(c,bytes);
        if (roleName.length() > 6 * bytes)
        {
            ((YYScene*)getParent())->showTips(g_sNameTooLong);
             setRoleName(roleName.c_str());
            return;
        }
        // 屏蔽敏感词
        CCArray* pArrNickName = GlobalData::getInstance()->getNickNameArray();
        uint32_t pArrNickNameCount = pArrNickName->count();
        for (uint32_t i = 0; i < pArrNickNameCount; i++)
        {
            const char* pElemWord = ((CCString*)pArrNickName->objectAtIndex(i))->getCString();
            if(strstr(roleName.c_str(),pElemWord) != NULL)
            {
                ((YYScene*)getParent())->showTips(g_sNameNickName);
                setRoleName(roleName.c_str());
                return;
            }
        }
        Controller::getInstance()->createRole(m_iProfessionIndex, roleName);	//login();
    }
}

void LoginUI::btnGenRoleName(CCObject* pSender, TouchEventType type)
{
    if(m_bNetLoading)
        return;
    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
        CCString *msg = CCString::createWithFormat(ASK_RANDOM_NAME,m_iProfessionIndex);
        Controller::getInstance()->sendMessage(msg->getCString(),true);
	}
}

void LoginUI::menuReturnCallback(CCObject* pSender, TouchEventType type)
{
    if(m_bNetLoading)
        return;
    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
        showUILayer(kPanelTypeChooseRole);
    }
}

void LoginUI::menuChooseRoleCallback(CCObject* pSender, TouchEventType type)
{
    if(m_bNetLoading)
        return;
    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
        int tag = static_cast<UIWidget*>(pSender)->getTag();
        updateRole(tag);
    }
}

void LoginUI::updateRole(int roleTag)
{
    int professionIndex[] = {3,2,1};	 // 根据策划按钮 设置角色职业 1 弓手 2 法师 3 战士
	m_iProfessionIndex = professionIndex[roleTag];
    //更新选中光圈UI
    {
        UIPanel* panel0 = static_cast<UIPanel*>(m_pPanelCreateRole->getChildByName("Panel0"));
        for (int i =0 ; i < 3; i++)
        {
            UIImageView* imgView = static_cast<UIImageView*>(panel0->getChildByName(CCString::createWithFormat("ImageView_%d",i)->getCString()));
            UIImageView* imgViewSelect = static_cast<UIImageView*>(imgView->getChildByName("ImageView_1"));
            if (i == roleTag)
            {
                imgViewSelect->setVisible(true);
            }
            else
            {
                imgViewSelect->setVisible(false);
            }
        }
    }
    
    //更新角色信息
    {
        UIPanel* panel1 = static_cast<UIPanel*>(m_pPanelCreateRole->getChildByName("Panel1"));
        UIImageView* imgView = static_cast<UIImageView*>(panel1->getChildByName("ImageView_0"));
        for (int i = 0; i < 3; i++)
        {
            UILabel * pText = static_cast<UILabel*>(imgView->getChildByName(CCString::createWithFormat("Label_%d",i)->getCString()));
            if (i == roleTag)
            {
                pText->setVisible(true);
            }
            else
            {
                pText->setVisible(false);
            }
        }
    }
	
    //更新角色图片
    {
        UIPanel* panel_2 = static_cast<UIPanel*>(m_pPanelCreateRole->getChildByName("Panel2"));
        for (int i = 0; i < 3; i++)
        {
            UIImageView * pImg = static_cast<UIImageView*>(panel_2->getChildByName(CCString::createWithFormat("ImageView_%d",i)->getCString()));
            if (i == roleTag)
            {
                pImg->setVisible(true);
            }
            else
            {
                pImg->setVisible(false);
            }
        }
    }
    //重置动画和特效
    resetRoleAction();
    
    //弓手 敬请期待
    UIImageView* imgView0 = static_cast<UIImageView*>(m_pPanelCreateRole->getChildByName("ImageView_0"));
    UIImageView* imgView_0 = static_cast<UIImageView*>(imgView0->getChildByName("ImageView_0"));
    imgView_0->setVisible(false);
    
    UIButton* go_button = static_cast<UIButton*>(m_pPanelCreateRole->getChildByName("Button_go"));
    if (roleTag == 2)
    {
        imgView_0->setVisible(true);
        go_button->setTouchEnabled(false);
        go_button->setBright(false);
    }
    else
    {
        m_bCreatRolePlaying[roleTag] = true;
        m_pCreateRoleSprite[roleTag]->setVisible(true);
        if (m_pRoleAnimation[roleTag].effect[0])
        {
            m_pRoleAnimation[roleTag].effect[0]->setVisible(true);
        }
        go_button->setTouchEnabled(true);
        go_button->setBright(true);
    }
    return;
}

void LoginUI::setRoleName(const char *pText)
{
    m_pInputName->setText(pText);
    m_pInputName->setPlaceHolder(pText);
}

void LoginUI::initRoleAnimation(int idx,cocos2d::gui::UIImageView* imgView)
{
    float fOffSetY  = imgView->getContentSize().height*0.5f*1.3f;
    float fOffSetX = imgView->getContentSize().width*0.3f;
  
    //角色动作
    int roleIdx[3] = {3,2,1};
    SpriteData * spriteData = (SpriteData *)GlobalData::getInstance()->getSpriteDataDict()->objectForKey(roleIdx[idx]);// 精灵基本数据
    m_pCreateRoleSprite[idx] = YYSprite::create();
    m_pCreateRoleSprite[idx]->initWithActionSet(spriteData->getResourceArray(), kDirTypeSprite,true);
    m_pCreateRoleSprite[idx]->setVisible(false);
    m_pCreateRoleSprite[idx]->setPosition(ccp(m_pCreateRoleSprite[idx]->getPositionX()-fOffSetX ,m_pCreateRoleSprite[idx]->getPositionY()-fOffSetY));
    imgView->addNode(m_pCreateRoleSprite[idx]);
    
    if (idx == 0)  //战士 
    {
        //角色动作
        m_pRoleAnimation[idx].count = 4;
        int arrAct[4] = {1,2,1,14};
        for (int i = 0; i < m_pRoleAnimation[idx].count; i++)
        {
            m_pRoleAnimation[idx].actionIdx[i] =arrAct[i];
        }
        m_pRoleAnimation[idx].curActionIdx = 0;
        m_pCreateRoleSprite[idx]->setAction(m_pRoleAnimation[idx].actionIdx[0]);
        
        //特效
        std::string strEffect = "p3-s6-2";
        for (int i = 0; i < m_pRoleAnimation[idx].count; i++)
        {
            if (i == 3)
            {
                m_pRoleAnimation[idx].effect[i] = YYSprite::createWithActionSet(strEffect,strEffect,kDirTypeSprite);
                m_pRoleAnimation[idx].effect[i]->setAction(0,true);
                m_pRoleAnimation[idx].effect[i]->setVisible(false);
                m_pRoleAnimation[idx].effect[i]->setPosition(m_pCreateRoleSprite[idx]->getPosition());
                imgView->addNode(m_pRoleAnimation[idx].effect[i]);
            }
            else
            {
                m_pRoleAnimation[idx].effect[i] = NULL;
            }
        }
    }
    else if (idx == 1)  //法师
    {
        //角色动作
        m_pRoleAnimation[idx].count = 8;
        int arrAct[8] = {1,2,1,1,2,1,8,9};
        for (int i = 0; i < m_pRoleAnimation[idx].count; i++)
        {
            m_pRoleAnimation[idx].actionIdx[i] =arrAct[i];
        }
        m_pRoleAnimation[idx].curActionIdx = 0;
        m_pCreateRoleSprite[idx]->setAction(m_pRoleAnimation[idx].actionIdx[0]);
        
        //特效
        for (int i = 0; i < m_pRoleAnimation[idx].count; i++)
        {
            if (i == 7)
            {
                m_pRoleAnimation[idx].effect[i] = YYSprite::createWithActionSet("p2-s6","p2-s6-1,p2-s6-2,p2-s6-3",kDirTypeSprite);
                m_pRoleAnimation[idx].effect[i]->setAction(0,true);
                m_pRoleAnimation[idx].effect[i]->setVisible(false);
                m_pRoleAnimation[idx].effect[i]->setPosition(m_pCreateRoleSprite[idx]->getPosition());
                imgView->addNode(m_pRoleAnimation[idx].effect[i]);
            }
            else
            {
                m_pRoleAnimation[idx].effect[i] = NULL;
            }
        }
    }
}

void LoginUI::resetRoleAction()
{
    for (int i = 0; i < 2; i++)
    {
        m_bCreatRolePlaying[i] = false;
        //角色动作
        m_pCreateRoleSprite[i]->setVisible(false);
        m_pRoleAnimation[i].curActionIdx = 0;
        m_pCreateRoleSprite[i]->setAction(m_pRoleAnimation[i].actionIdx[0]);
        //特效
        for (int j = 0; j < m_pRoleAnimation[i].count; j++)
        {
            if (m_pRoleAnimation[i].effect[j])
            {
                m_pRoleAnimation[i].effect[j]->setAction(0,true);
                m_pRoleAnimation[i].effect[j]->setVisible(false);
            }
        }
    }
}

void LoginUI::updateRoleAction(int idx)
{
    RoleAnimation* pRoleAnimation = &(m_pRoleAnimation[idx]);
    if (m_pCreateRoleSprite[idx]->isActionOver())
    {
        pRoleAnimation->curActionIdx++;
        
        if (pRoleAnimation->curActionIdx >= pRoleAnimation->count)
        {
           pRoleAnimation->curActionIdx = 0;
        }
        m_pCreateRoleSprite[idx]->setAction(pRoleAnimation->actionIdx[pRoleAnimation->curActionIdx]);
        
        for (int i = 0; i < pRoleAnimation->count; i++)
        {
            if(pRoleAnimation->effect[i])
            {
                pRoleAnimation->effect[i]->setAction(0,true);
                if (i == pRoleAnimation->curActionIdx)
                {
                    pRoleAnimation->effect[i]->setVisible(true);
                }
                else
                {
                    pRoleAnimation->effect[i]->setVisible(false);
                }
            }
        }
    }
    if (pRoleAnimation->effect[pRoleAnimation->curActionIdx])  //特效指针不为空更新
    {
        pRoleAnimation->effect[pRoleAnimation->curActionIdx]->updateAnimation();
    }
    m_pCreateRoleSprite[idx]->updateAnimation();
    
    return;
}
