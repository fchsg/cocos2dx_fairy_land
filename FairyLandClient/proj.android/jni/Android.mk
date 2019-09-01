LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libfairland

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/animation/YYActionSet.cpp \
                   ../../Classes/animation/YYFrameData.cpp \
                   ../../Classes/animation/YYRect.cpp \
                   ../../Classes/animation/YYSprite.cpp \
                   ../../Classes/common/ccUtil.cpp \
                   ../../Classes/common/FileReadManager.cpp \
                   ../../Classes/common/GeometricRecognizer.cpp \
                   ../../Classes/common/Common.cpp \
                   ../../Classes/common/ODSocket.cpp \
                   ../../Classes/common/StringRes.cpp \
                   ../../Classes/core/YYBagUI.cpp \
                   ../../Classes/core/YYTaskUI.cpp \
                   ../../Classes/core/YYTalentUI.cpp \
                   ../../Classes/core/YYSkillUI.cpp \
                   ../../Classes/core/YYCatapultEffect.cpp \
                   ../../Classes/core/YYCellLayer.cpp \
                   ../../Classes/core/YYConfig.cpp \
                   ../../Classes/core/YYController.cpp \
                   ../../Classes/core/YYControlUILayer.cpp \
                   ../../Classes/core/YYCover.cpp \
                   ../../Classes/core/YYDungeonsUI.cpp \
                   ../../Classes/core/YYFightSprite.cpp \
                   ../../Classes/core/YYGlobalData.cpp \
                   ../../Classes/core/YYLayer.cpp \
                   ../../Classes/core/YYLinkEffect.cpp \
                   ../../Classes/core/YYLoading.cpp \
                   ../../Classes/core/YYLogo.cpp \
                   ../../Classes/core/YYMonster.cpp \
                   ../../Classes/core/YYNPC.cpp \
                   ../../Classes/core/YYPlayer.cpp \
                   ../../Classes/core/YYPopUpUI.cpp \
                   ../../Classes/core/YYPortal.cpp \
                   ../../Classes/core/YYProtocol.cpp \
                   ../../Classes/core/YYProtossInterface.cpp \
                   ../../Classes/core/YYScene.cpp \
                   ../../Classes/core/YYSkill.cpp \
                   ../../Classes/core/YYSkillAction.cpp \
                   ../../Classes/core/YYSkillEffect.cpp \
                   ../../Classes/core/YYSpriteLayer.cpp \
                   ../../Classes/core/YYLoginUI.cpp \
                   ../../Classes/core/YYTrap.cpp \
                   ../../Classes/core/YYChargeUI.cpp \
                   ../../Classes/core/YYMailUI.cpp \
                   ../../Classes/core/YYAchievementUI.cpp \
                   ../../Classes/core/YYTrialPlayer.cpp \
                   ../../Classes/core/YYEntertainmentUI.cpp \
                   ../../Classes/core/YYVipUI.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)

