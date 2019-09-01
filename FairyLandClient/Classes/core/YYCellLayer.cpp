//
//  YYCellLayer.cpp
//  XJ
//
//  Created by fuchenhao on 13-7-24. 
//
//

#include "YYCellLayer.h"
#include "YYScene.h"
#include "../common/Common.h"
#include "YYController.h"

USING_NS_CC;

void YYCellLayer::draw()
{
    int width = ((YYScene *)getParent())->getSceneSize().width;
    ccDrawColor4B(255,255,255,255);
    for (int i = 1; i < width / Controller::getInstance()->getScene()->getTileWidth(); i++) {
        CCPoint point1 = ccp(i * Controller::getInstance()->getScene()->getTileWidth(),0);
        CCPoint point2 = ccp(i * Controller::getInstance()->getScene()->getTileWidth(),Controller::getInstance()->getSceneCellTopY());
        ccDrawLine(point1,point2);
    }
    for (int j = 0; j <= Controller::getInstance()->getSceneCellTopY() / Controller::getInstance()->getScene()->getTileHeight(); j++) {
        CCPoint point1 = ccp(0 , (Controller::getInstance()->getSceneCellTopY() - j * Controller::getInstance()->getScene()->getTileHeight()));
        CCPoint point2 = ccp(width , (Controller::getInstance()->getSceneCellTopY() - j * Controller::getInstance()->getScene()->getTileHeight()));
        ccDrawLine(point1,point2);
    }
}
