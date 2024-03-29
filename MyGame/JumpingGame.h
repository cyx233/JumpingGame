﻿#pragma region 头文件引用

// Windows 头文件: 
#include <windows.h>


// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>

// 资源头文件
#include "resource.h"
#pragma comment(lib, "Msimg32.lib")
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

//其他头文件
#include <vector>
#include <math.h>


#pragma endregion


#pragma region 宏定义

#define WINDOW_WIDTH			1296	//游戏窗口宽度
#define WINDOW_HEIGHT			768		//游戏窗口高度
#define WINDOW_TITLEBARHEIGHT	32		//标题栏高度


#define STAGE_STARTMENU			0		//开始画面的ID
#define STAGE_STARTSTORY		1		//开始故事界面
#define STAGE_SELECT			2		//选关界面ID
#define STAGE_1					3		//第一个游戏场景的ID  黄铁矿之苹果
#define STAGE_2					4		//锌铁铜之苹果
#define STAGE_3					5		//只是黄色的普通苹果
#define STAGE_4					6		//镀金之苹果
#define STAGE_5					7		//金金金金金苹果
 
#define STAGE_HELP_1			8		//帮助场景ID 同伴不会白白死去，他们会为我们铺出道路 方向键操作 空格键复活
#define STAGE_HELP_2			9		//让火焰净化一切
#define STAGE_HELP_3			10		//短道速滑，，要快！
#define STAGE_HELP_4			11		//水晶控制元素，末影之眼控制传送

#define STAGE_ENDSTORY			12		//结局故事
#define STAGE_ENDSTORY2			13	
#define STAGE_ENDSTORY3			14



#define BLOCK_NORMAL		1		//泥土ID
#define BLOCK_THORN			3		//尖刺ID
#define BLOCK_FIRE			4		//火焰ID
#define BLOCK_ICE			5		//冰焰ID
#define BLOCK_MOVETHORN		6		//移动尖刺ID
#define BLOCK_PEDAL			7		//踏板ID
#define BLOCK_ONOFF			8		//开关ID
#define BLOCK_APPLE			9		//终点ID

#define BLOCK_SAVE			10		//存档点ID
#define BLOCK_MOVABLEBODY	11		//可移动尸体ID
#define BLOCK_FREEZE		12		//冰冻状态ID
#define BLOCK_STILLBODY		13		//静止尸体ID
#define BLOCK_BURNEDBODY	14		//烧毁尸体ID
#define BLOCK_STICKBODY		15		//插在移动尖刺上的尸体


#define BLOCK_SIZE_X			32		//方块的宽度
#define BLOCK_SIZE_Y			32		//方块的高度

#define NAME_WIDTH				210		//姓名栏宽度
#define NAME_HEIGHT				27		//姓名栏高度


#define HERO_SIZE_X				32		//主角的宽度
#define HERO_SIZE_Y				32		//主角的高度
#define HERO_FRAME_NUMBER		8		//主角的动画帧数量
#define HERO_ACCELERATION		0.3		//主角加速度
#define GRAVITION				0.6		//重力加速度
#define HERO_MAXSPEED			6.0		//主角最大速度


#define BUTTON_STARTGAME			1	//开始游戏按钮ID
#define BUTTON_HELP					2	//游戏帮助按钮ID
#define BUTTON_STARTGAME_WIDTH		147	//开始游戏按钮宽度
#define BUTTON_STARTGAME_HEIGHT		60	//开始游戏按钮高度
#define BUTTON_LABEL_WIDTH			200	//选关标签宽度
#define BUTTON_LABEL_HEIGHT			81	//选关标签高度
#define BUTTON_MENU					3	//菜单按钮ID
#define BUTTON_BACK					4	//返回按钮ID
#define BUTTON_CONTINUE				5	//继续按钮ID
#define BUTTON_RETRY				6	//重来按钮ID
#define BUTTON_LABEL				7	//标签ID
#define BUTTON_SELECT				8	//选关ID
#define BUTTON_PAUSE				9	//暂停ID
#define BUTTON_NEXT					10	//下一场景ID
#define BUTTON_SOUND				11	//bgmID
#define BUTTON_SILENT				12	//静音ID



#define TIMER_GAMETIMER				1	//游戏的默认计时器ID
#define TIMER_GAMETIMER_ELAPSE		10	//默认计时器刷新间隔的毫秒数


#pragma endregion


#pragma region 结构体声明

// 场景结构体
struct Stage
{
	int stageID;		//场景编号
	HBITMAP bg;			//背景图片
	int timeCountDown;	//游戏时间倒计时
	bool timerOn;		//计时器是否运行（游戏是否被暂停）

};


// 按钮结构体
struct Button
{
	int buttonID;	//按钮ID (stageID)*1000+按钮种类序号
	bool visible;	//按钮是否可见
	HBITMAP img;	//图片
	int x;			//坐标x
	int y;			//坐标y
	int width;		//宽度
	int height;		//高度
};

//姓名栏结构体
struct Name
{
	bool visible;	//姓名栏是否可见
	HBITMAP img;	//图片
	int x;			//坐标x
	int y;			//坐标y
	int width;		//宽度
	int height;		//高度
	int frame;		//帧数
};

// 主角结构体
struct Hero
{
	HBITMAP img;	//图片
	int frame;		//当前显示的帧数
	int x;			//坐标x
	int y;			//坐标y
	double vx;		//速度x
	double vy;		//速度y
	double basevx;	//地面速度x
};

//方块结构体
struct Block
{
	int blockID;	//方块ID  (stageID)*1000+按钮种类序号
	bool visible;	//方块是否可见
	bool turnon;	//方块是否触发
	int	frame;		//方块帧数
	HBITMAP img;	//图片
	int x;			//坐标x
	int y;			//坐标y
	int width;		//宽度
	int height;		//高度
	double vx;		//横坐标速度
	double vy;		//纵坐标速度
	double prevx;	//暂停前横坐标速度
	double prevy;	//暂停前纵坐标速度
	int m;			//辅助变量m
	int n;			//辅助变量n
	int link;		//关联变量
};


#pragma endregion


#pragma region 事件处理函数声明


// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标移动事件处理函数
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标左键松开事件处理函数
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);

#pragma endregion


#pragma region 添加结构体函数声明

// 添加按钮函数
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y);

//添加方块函数
Block* CreateBlock(int blockID, HBITMAP img, int width, int height, int x, int y);

// 添加主角函数
Hero* CreateHero(HBITMAP img, int x, int y);

Name* CreateName(HBITMAP img, int x, int y);


#pragma endregion


#pragma region 初始化函数声明

// 初始化场景函数
void InitStage(HWND hWnd, int stageID);

// 初始化按钮函数
void InitButton(HWND hWnd, int stageID);

// 初始化方块函数
void InitBlock(HWND hWnd, int stageID);

// 初始化地图函数
void InitMap(HWND hWnd, int stageID);

#pragma endregion


#pragma region 主角函数声明

// 刷新主角状态函数
void UpdateHero(HWND hWnd);

//刷新姓名栏函数
void UpdateName();

// 碰撞检测函数
bool CollitionDetect(HWND hWnd);

//陷阱检测函数
void TrapDetect(HWND hWnd);

#pragma endregion


#pragma region 环境状态函数

void UpdateBody(HWND hWnd, Block*block);	//刷新尸体状态函数

void UpdateSurround(HWND hWnd);				//刷新环境状态函数

void BodyTrapDetect(HWND hWnd,Block*body);	//尸体陷阱检测

bool BodyCollitionDetect(HWND hWnd, Block*body);//尸体碰撞检测

void TriggerP(HWND hWnd, Block*pedal);	//踏板触发检测

void TriggerOnOff(HWND hWnd, Block*onoff);

#pragma endregion


#pragma region 绘图函数声明

// 绘图函数
void Paint(HWND hWnd);

#pragma endregion
