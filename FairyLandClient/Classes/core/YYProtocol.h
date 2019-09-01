#ifndef __FAIRLAND__PROTOCOL__
#define __FAIRLAND__PROTOCOL__
#include "YYConfig.h"

class Protocol{
public:
        
static std::string getRegisterMessage(LoginInfo message);
static std::string getLoginMessage(LoginInfo message);
static std::string getReconnectMessage(LoginInfo message,int roleIndex);

//-----------------------------------------------注册登录相关---------------------------------------------------
#define VERIFY_VERSION                          "version %s"                    //版本验证 
#define ASK_SERVER_LIST                         "server %s"                     //请求服务器列表 [渠道]
#define ASK_REGISTER                            "reg %s_%s_%s_%s_%s_%s_%d"    //reg [账号]_[密码]_[信箱]_[渠道]_[版本]_[机型]_[手机号]_[运营商]_[iemi]_[编码模式（0为gb，非零为utf8)] (账号留空代表快速注册，密码留空代表随机密码，其他字段如果没有都填0) [机型]_[手机号]_[运营商]_[iemi]合并为手机信息
#define ASK_LOGIN                               "login %s_%s_%s_%s_%s_%d"     //login [账号]_[密码]_[渠道]_[版本]_[机型]_[手机号]_[运营商]_[iemi]_[编码模式（0为gb，非零为utf8)]
#define ASK_RECONNECT                           "login %s.%d_%s_%s_%s_%s_%d"  //重连游戏服务器
#define ASK_LOGOUT                              "quit"                          //登出
#define ASK_RANDOM_NAME                         "role N_%d"                     //随机姓名
#define	ASK_ROLE_CREATE                         "role C"                        //创建角色进入游戏 C_[角色index(1,2,3)]_[职业(1,2,3)]_[性别(0,1)]_[名字]
#define	ASK_ROLE_DELETE                         "role D_%d"                     //删除角色 D_[角色index(1,2,3)] 删除角色 返回RD_[index]_[剩余时间]
#define ASK_ROLE_DELETE_SURPLUS                 "role D_F_%d"                   //删除角色剩余时间返回RD_[index]_[剩余时间] -1为已经删除
#define	ASK_ROLE_DELETE_CANCEL                  "role D_C_%d"                   //取消删除 返回RD_[index]_0
#define	ASK_ROLE_SELECT                         "role E"                        //选择角色进入游戏 E_[角色index(1,2,3)] 进入游戏
#define ASK_ROLE_LIST                           "role L"                        //角色列表

#define	RECEIVE_PROTOCOL_VERSION                "PV"                            //协议版本号

//-----------------商城--------------
#define	ASK_SHOP_LIST                           "shop L"                        //请求商城数据
#define ASK_SHOP_BUG                            "shop B_%d_%d"                  //请求购买物品
//-----------------充值--------------
#define ASK_CHARGE_LIST                         "pay L"                         // 请求充值界面数据信息
#define RECEIVE_CHARGE_LIST                     "PL"                            // 充值界面数据信息
#define ASK_CHARGE_RESQUEST                     "pay"                           // RMB充值
#define RECEIVE_CHARGE_SERIAL_NUMBER            "PS"                            // 得到订单串号

//---------------登陆奖励-------------
#define ASK_LOGIN_AWARD                         "sign L_7"                      //请求登录奖励
#define ASK_LOGIN_GET_AWARD                     "sign R_7_%d"                   //请求奖励
#define ASK_SIGNUP_INFO                         "sign L_sign"                   //请求签到信息
#define ASK_SIGNUP                              "sign S_%d"                     //请求签到
#define ASK_SIGNUP_BOX_AWARD                    "sign R_sign_%d"                //请求宝箱奖励

//-----------------活跃度---------------

#define ASK_ACTIVITY_TASK_UPDATE                "activity U"                    //请求任务的更新
#define ASK_ACTIVITY_BOX_AWARD                  "activity R_%d"                 //宝箱的奖励
#define	ASK_ACTIVITY_INFO                       "activity L"                    //请求活跃度数据

//-----------------成就-----------------
#define ASK_ACHIEVEMENT_AWARD                   "achi AF_%d"                    //请求成就奖励
#define ASK_ACHIEVEMENT_BOX_AWARD               "achi GF_%d"                    //请求宝箱奖励
#define	ASK_ACHIEVEMENT_LIST                    "achi L"                        //请求成就数据

//-----------------邮件-----------------
#define ASK_MAIL_LIST                           "mail L"                        // 查看邮件列表
#define SEND_MAIL                               "mail S_%s_%s_%s_%d"            // 发送邮件
#define SEND_READ_MAIL                          "mail R_%d"                     // 读邮件
#define SEND_REMOVE_MAIL                        "mail D_%d"                     // 删邮件
#define SEND_ONE_KEY_REMOVE_MAIL                "mail O"                        // 一键删邮件
#define SEND_GET_ACCESSORY                      "mail A_%d"                     // 提取附件
#define SEND_MAIL_GM                            "mail GM_%s_%s"                 // 发送GM邮件 标题 内容


//-----------------副本-----------------
#define	ASK_RETURN_CITY                         "leave"                         //回城
#define	ASK_DUNGEONS_PORTAL                     "instance L_C"                  //弹出副本选择界面的传送门
#define SEND_DUNGEONS_INDEX                     "instance L_S_%d"               //选择副本,弹出关卡选择界面
#define SEND_STAGE_INDEX                        "instance L_D_%d_%d"            //选择关卡,弹出关卡详细信息
#define SEND_DIFFCULTY                          "instance S_%d_%d_%d"           //确认进入对应难度的副本
#define SEND_WIPE_DUNGEON                       "instance A_%d_%d_%d"           //发送扫荡副本信息 
#define SEND_FIGHT_RESULT                       "instance F_%d"                 //战斗结束确认

#define ASK_EXCLUDE_CARD                        "instance R_%d_%d"              // 排除奖励牌面
#define ASK_TURN_UP_CARD                        "instance D_%d"                 // 翻开奖励牌面
#define RECEIVE_EXCLUDE_CARD                    "IR"
#define RECEIVE_TURN_UP_CARD                    "IS"
//-----------------聊天-------------------
#define	SEND_CHAT_ALL                           "chat W_%s"                     //世界频道
#define	SEND_CHAT_CITY                          "chat R_%s"                     //当前场景
#define	ASK_BUY_                                "shop B%d_%d_%s"                //

#define SEND_CURRENT_POSITION                   "move %d"                       // 发送当前位置
//------------------------------------------星灵--------------------------------------
#define ASK_PROTOSS_LIST_A                      "protoss LC"                    //获取占灵初始信息
#define ASK_PROTOSS_LIST_B                      "protoss LB"                    //获取占灵背包信息
#define ASK_NORMAL_PROTOSS                      "protoss NP"                    //普通占灵
#define ASK_NORMAL_COLLECT                      "protoss NC"                    //普通拾取
#define ASK_NORMAL_SELL                         "protoss NS"                    //普通卖出
#define ASK_NORMAL_COMBO                        "protoss PC"                    //普通合成
#define ASK_ONEKEY_PROTOSS                      "protoss OP"                    //一键占灵
#define ASK_ONEKEY_COLLECT                      "protoss OC"                    //一键拾取
#define ASK_ONEKEY_SELL                         "protoss OS"                    //一键卖出
#define ASK_ONEKEY_COMBO                        "protoss PJ"                    //一键锻造
#define ASK_PROTOSS_EQUIP                       "protoss PE"                    //装备星灵
#define ASK_PROTOSS_REMOVE                      "protoss PR"                    //移除星灵
#define ASK_SCRAP_EXCHANGE                      "protoss SC"                    //碎片兑换
#define ASK_BUTTON_LIGHT                        "protoss BL"                    //点亮按钮

#define ASK_RELIVE                              "revive"                        //复活
#define ASK_RELIVE_JEWEL                        "revive jew"                    //用宝石复活
#define RECEIVE_RELIVE_INFO                     "RE"                            //复活
//-----------------------------------------------场景地图相关---------------------------------------------------

#define	RECEIVE_ERR_MSG                         "FT"                            // 错误提示
#define RECEIVE_LOADING_OVER                    "FN"                            // 加载场景结束

//----------------------------------------背包及物品装备操作相关--------------------------------------------
#define ASK_OPEN_TREASURE                       "chest O_%s"                    //开宝箱
#define ASK_REFRESH_TREASURE                    "chest U_%s"                    //刷新宝箱
#define ASK_GET_ITEM                            "chest E_%s"                    //获得宝箱内物品
#define	ASK_PUT_ON_EQUIP                        "equip W_%s"                    //穿上装备
#define ASK_USE_PROP                            "item U_%s"                     //物品使用
#define ASK_SORT_ITEM                           "item ST_0"                     //整理物品
#define ASK_SALE_PROP                           "item S_0_%s"                   //出售物品
#define ASK_SALE_EQUIP                          "item S_1_%s"                   //出售装备
#define ASK_REINFORCE_INFO                      "equip PR_%s"                   //请求强化信息
#define ASK_REINFORCE_MONEY                     "equip P_%s_money"              //强化(消耗金钱)
#define	ASK_REINFORCE_JEWEL                     "equip P_%s_jew"                //百分百强化(消耗宝石)
#define ASK_MATERIAL_INFO                       "item Q_%s"                     //请求材料信息
#define ASK_MATERIAL_INFO2                      "item Q_%d"                     //请求材料信息
#define	ASK_COMPOSE_INFO                        "equip UR_%s"                   //装备锻造信息
#define	ASK_MATERIAL                            "equip OUR_%s"                  //补全材料提示信息
#define	ASK_COMPOSE_MONEY                       "equip U_%s"                    //确认消耗金钱锻造
#define	ASK_COMPOSE_JEWEL                       "equip OU_%s"                   //确认消耗宝石锻造

//------------------------------------------技能--------------------------------------------
    
#define	ASK_SKILL_LEVEL_UP                      "skill U_%d"                      // 技能升级
#define ASK_SKILL_SHORT_CUT                     "skill P_%d_%d"                   // 技能快捷栏修改 技能id 快捷栏id
    
#define	    ASK_UPDATE_TALENT                   "talent U_%d"                   // 天赋升级 天赋id
#define	    ASK_RESET_TALENT                    "talent R_%d"                   // 天赋重置 技能id
    

//------------------------------------------公告--------------------------------------------
#define SEND_NOTICE                             "notice"

//------------------------------------------兑换码--------------------------------------------
#define SEND_EXCHANGE                           "exchange %s"

//------------------------------------------用户中心--------------------------------------------
#define SEND_USERCENTER                         "custom"
    
//------------------------------------------活动--------------------------------------------

// 一级页面
#define ASK_PLAYTABLE_DATA                       "action"    //获取一级页面活动数据
    
// 转盘
#define ASK_TURNTABLE_INFO                       "turntable L"    //获取转盘信息
#define ASK_TURNTABLE_ROATE                      "turntable T"    //转动转盘
#define ASK_TURNTABLE_RECEIVE                    "turntable R"    //领取转盘奖励
    
// 骰子
#define ASK_DICE_INFO                            "throw L"    //获取骰子页面信息
#define ASK_DICE_THROW                           "throw T"    //摇骰子
#define ASK_DICE_RECEIVE                         "throw R"    //领取骰子奖励
    
// 地下城
#define ASK_DUNGEON_INFO                         "dungeon L"        // 打开地下城
#define ASK_DUNGEON_CHALLENGE                    "dungeon B_%d" // 挑战地下城
#define ASK_DUNGEON_RESET                        "dungeon R_%d"     // 重置地下城
#define ASK_DUNGEON_RESULT                       "dungeon C_%d"    // 地下城结果
    
// 守卫星灵
#define ASK_PROTECT_PROTOSS_INFO                 "tower L"  // 打开守卫星灵
#define ASK_PROTECT_PROTOSS_CHALLENGE            "tower B_%d" // 挑战 (难度等级：0，1，2)
#define ASK_PROTECT_PROTOSS_WIPE                 "tower S_%d" // 扫荡 (难度等级：0，1，2)
#define ASK_PROTECT_PROTOSS_RESULT               "tower C_%d" // 挑战结果 (0失败，1胜利)
    
// boss副本
#define ASK_BOSS_DUNGEON_INFO                    "raidboss L" // 打开boss副本
#define ASK_BOSS_DUNGEON_CHALLENGE               "raidboss B_%d" // 挑战副本
#define ASK_BOSS_DUNGEON_WIPE                    "raidboss S_%d" // 扫荡副本
#define ASK_BOSS_DUNGEON_RESULT                  "raidboss C_%d" // 挑战结果 (0失败，1胜利)
#define ASK_BOSS_DUNGEON_BUY                     "raidboss B" // 购买挑战次数
    
// 世界boss
#define ASK_WORLD_BOSS_INFO                      "worldboss L" // 打开世界boss
#define ASK_WORLD_BOSS_CHALLENGE                 "worldboss B" // 挑战世界boss
#define ASK_WORLD_BOSS_RESULT                    "worldboss R_%d" // 挑战结果 返回造成boss伤害值
#define ASK_WORLD_BOSS_CLEARCD                   "worldboss C" // 清除CD
#define ASK_WORLD_BOSS_PRAY                      "worldboss A" // 增加祝福
#define ASK_WORLD_BOSS_KILL                      "worldboss K" // 该玩家即将打死BOSS
#define ASK_WORLD_BOSS_RETURN                    "leave" // Boss 被打死 玩家回主城
    

//------------------------------------------心跳线程----------------------------------------
#define ASK_HEARTBEAT  "idle"    // 与服务器进行心跳连接 每个30s请求一次
  

//------------------------------------------任务--------------------------------------------
/**
 
 查看npc功能
 指令 npc F_[uid]
 返回 NF （npc身上的功能，未启用）
 返回 NT_[[任务ID];[任务名称]]_[[任务ID];[任务名称]]
 
 通过npc进行任务
 指令 npc T_[uid]_[任务id]
 返回 TA TB
 
 查看任务信息
 task L
 
 任务类型为2,3,4时的数量改变
 task S_[任务id]_[数量]
 
 任务类型为2,3,4时完成当前步骤
 task N_[任务id]
 */
#define ASK_NPC_FUNCTION                        "npc F_%s"                      //查询NPC功能
#define	ASK_TASK_LIST                           "task L"                        //请求任务列表
#define ASK_FUNCTION_FROM_NPC                   "npc F_%s"                      //请求NPC的功能
#define	ASK_TASK_FROM_NPC                       "npc T_%s_%d"                   //向NPC请求任务（包括交、接）
#define	SEND_TASK_RESPONSE                      "task S_%d_%d"                  //杀怪、收集及通关任务完成情况（数量变化）
#define SEND_TASK_COMPLETE                      "task N_%d"

//---------------------------------------------脚本相关--------------------------------------------------

#define	ASK_NEXT_SCRIPT                         "mi P"                          //请求下一段脚本
#define	RECEIVE_RUN_SCRIPT                      "RN"                            // 进入场景，RN后的脚本指令需要在加载场景结束后执行
#define	SCRIPT_OVER                     "NG"                            // SCRIPT_OVER;
#define	SCRIPT_SET_UI_VISIBLE           "setUIVisible"                  // 设置UI界面是否可视
#define SCRIPT_
#define	SCRIPT_LOCK_SPRITE              "lockSprite"                    // 锁定精灵，精灵只受脚本控制
#define	SCRIPT_SET_SPRITE_VISIBLE       "setSpriteVisible"              // 设置精灵可视
#define	SCRIPT_REMOVE_SPRITE            "removeSprite"                  // 移除精灵
#define	SCRIPT_LOCK_CAMERA              "lockCamera"                    // 锁定镜头，镜头只受脚本控制
#define	SCRIPT_SET_CAMERA_POSITION      "setCameraPosition"             // 设置镜头位置
#define	SCRIPT_CAMERA_MOVE              "cameraMove"                    // 镜头移动
#define	SCRIPT_CAMERA_MOVE_TO           "cameraMoveTo"                  // 镜头移动至
#define	SCRIPT_CAMERA_MOVE_BACK         "cameraMoveBack"                // 镜头移回
#define	SCRIPT_SAY                      "say"                           // 对话
#define	SCRIPT_FACE                     "face"                          // 表情 精灵ID 表情ID x,y偏移量
#define	SCRIPT_SKILLUSING               "skillUsing"                    // 使用技能 参数1 精灵ID，参数2 技能编号
#define	SCRIPT_SET_ACTION               "setAction"                     // 设置人物动作 1 精灵ID 2 动作序号 3 持续帧数 4 是否停顿在最后一帧
#define	SCRIPT_MOVE                     "move"                          // 移动
#define	SCRIPT_MOVE_TO                  "moveTo"                        // 移动到
#define	SCRIPT_PAUSE                    "pause"                         // 暂停脚本
#define	SCRIPT_GUIDE                    "guide"                         // 引导动画
#define	SCRIPT_HELP                     "help"                          // 引导动画
};
#endif
