//
//  YYCover.h
//  XJ
//
//  Created by fuchenhao on 13-5-9. 
//
//

#ifndef __XJ__YYCover__
#define __XJ__YYCover__

#include "cocos2d.h"
#include "AssetsManager/AssetsManager.h"

class YYSprite;

enum CoverStatus{
    kCoverStatusDefault = -1,
    kCoverStatusNormal,
    kCoverStatusDownload,
    kCoverStatusUncompress,
};

class Cover : public cocos2d::CCLayer,public cocos2d::extension::AssetsManagerDelegateProtocol
{
    CC_SYNTHESIZE(CoverStatus, m_CoverStatus, CoverStatus);
    CC_SYNTHESIZE_READONLY(YYSprite *, m_pTipSprite, TipSprite);
    CC_SYNTHESIZE(cocos2d::CCLabelTTF *,m_pVersionLabel, VersionLabel);
    CC_SYNTHESIZE(bool , m_bShowConnectFailed, ShowConnectFailed);
    
private:
    
    cocos2d::CCSprite *m_pCoverSprite;
    YYSprite *m_pRoleSprite;
    YYSprite *m_pLightningSprite;
    YYSprite *m_pSmokeSprite1,*m_pSmokeSprite2;
    
    cocos2d::CCLabelTTF *m_pProgressLabel;
    
public:
    
    Cover();
    ~Cover();
    
    CREATE_FUNC(Cover);
    
    virtual void onEnter();
    //cocos2d::extension::AssetsManagerDelegateProtocol
    virtual void onError(cocos2d::extension::AssetsManager::ErrorCode errorCode);
    virtual void onProgress(int percent);
    virtual void onSuccess();
    
    virtual bool init();
    void update();
    void touchBegan(cocos2d::CCTouch * touch);
};

#endif /* defined(__XJ__YYCover__) */
