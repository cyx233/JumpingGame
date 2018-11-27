#pragma region 头文件引用

// Windows 头文件: 
#include <windows.h>


// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// 资源头文件
#include "resource.h"
#pragma comment(lib, "Msimg32.lib")


//其他头文件
#include <vector>
#include <math.h>


#pragma endregion


#pragma region 宏定义

#define WINDOW_WIDTH			1280	//游戏窗口宽度
#define WINDOW_HEIGHT			720		//游戏窗口高度
#define WINDOW_TITLEBARHEIGHT	32		//标题栏高度


#define STAGE_STARTMENU			0		//开始画面的ID
#define STAGE_1					1		//第一个游戏场景的ID
#define STAGE_HELP				2		//帮助场景ID



#define BLOCK_DIRT			1		//泥土ID
#define BLOCK_GRASS			2		//草地ID
#define BLOCK_THORN			3		//尖刺ID
#define BLOCK_FIRE			4		//火焰ID

#define BLOCK_SAVE			5		//存档点ID
#define BLOCK_STILLBODY		6		//静止尸体ID
#define BLOCK_BURNEDBODY	7		//烧毁尸体ID

#define BLOCK_SIZE_X			32		//方块的宽度
#define BLOCK_SIZE_Y			32		//方块的高度



#define HERO_SIZE_X				32		//主角的宽度
#define HERO_SIZE_Y				32		//主角的高度
#define HERO_FRAME_NUMBER		8		//主角的动画帧数量
#define HERO_ACCELERATION		0.3		//主角加速度
#define GRAVITION				0.6		//重力加速度
#define HERO_MAXSPEED			6.0		//主角最大速度


#define BUTTON_STARTGAME			0001	//开始游戏按钮ID
#define BUTTON_HELP					0002	//游戏帮助按钮ID
#define BUTTON_STARTGAME_WIDTH		147		//开始游戏按钮宽度
#define BUTTON_STARTGAME_HEIGHT		60		//开始游戏按钮高度


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

// 主角结构体
struct Hero
{
	HBITMAP img;	//图片
	int frame;		//当前显示的帧数
	int x;			//坐标x
	int y;			//坐标y
	double vx;		//速度x
	double vy;		//速度y
};

//方块结构体
struct Block
{
	int blockID;	//方块ID  (stageID)*1000+按钮种类序号
	bool visible;	//方块是否可见
	int	frame;		//方块帧数
	HBITMAP img;	//图片
	int x;			//坐标x
	int y;			//坐标y
	int width;		//宽度
	int height;		//高度
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

//刷新环境状态函数
void UpdateSurround(HWND hWnd);

// 刷新主角状态函数
void UpdateHero(HWND hWnd);

// 计算主角的当前帧数
int GetHeroFrame(double dirX, double dirY);

// 碰撞检测函数
bool CollitionDetect(HWND hWnd);

//陷阱检测函数
void TrapDetect(HWND hWnd);

#pragma endregion


#pragma region 绘图函数声明

// 绘图函数
void Paint(HWND hWnd);

#pragma endregion
