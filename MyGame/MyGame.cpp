// JumpingGame.cpp : 定义应用程序的入口点。
//

#include "JumpingGame.h"

using namespace std;



#pragma region 全局变量

#define MAX_LOADSTRING			100		

// 全局变量: 
HINSTANCE hInst; // 当前窗体实例
WCHAR szTitle[MAX_LOADSTRING]; // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING]; // 主窗口类名

//图像资源

HBITMAP bmp_Background; //背景图像

HBITMAP bmp_StartButton; //开始按钮图像
HBITMAP bmp_HelpButton; //帮助按钮图像

HBITMAP bmp_Hero; //主角图像

HBITMAP bmp_BlockDirt;  //泥土图像
HBITMAP bmp_BlockGrass; //草地图像
HBITMAP bmp_BlockSave;	//存档点图像

HBITMAP bmp_BlockThorn;		//尖刺图像
HBITMAP bmp_BlockFire;		//火焰图像
HBITMAP bmp_BlockIce;		//冰焰图像

HBITMAP bmp_BloodBody;	//带血尸体图像
HBITMAP bmp_BurnedBody; //燃烧尸体图像
HBITMAP bmp_FreezedBody;//冰冻尸体图像

Stage* currentStage = NULL; //当前场景状态
Block* CurrentSave = NULL;	//当前存档点
Hero* theHero = NULL; //主角状态
vector<Button*> buttons; //按钮
vector<Block*>blocks; //方块         


int mouseX = 0; //鼠标横坐标
int mouseY = 0; //鼠标纵坐标
bool mouseDown = false; //鼠标左键
bool keyUpDown = false; //上键
bool keyDownDown = false; //下键
bool keyLeftDown = false; //左键
bool keyRightDown = false; //右键
bool keyZDown = false; //未开发


double const PI = acos(double(-1));
#pragma endregion



#pragma region Win32程序框架



// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_JUMPINGGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_JUMPINGGAME));

	MSG msg;

	// 主消息循环: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}




//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_JUMPINGGAME);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindow(szWindowClass, szTitle,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, // 设置窗口样式，不可改变大小，不可最大化,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT + WINDOW_TITLEBARHEIGHT,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			// 初始化游戏窗体
			InitGame(hWnd, wParam, lParam);
			break;
		case WM_KEYDOWN:
			// 键盘按下事件
			KeyDown(hWnd, wParam, lParam);
			break;
		case WM_KEYUP:
			// 键盘松开事件
			KeyUp(hWnd, wParam, lParam);
			break;
		case WM_MOUSEMOVE:
			// 鼠标移动事件
			MouseMove(hWnd, wParam, lParam);
			break;
		case WM_LBUTTONDOWN:
			// 鼠标左键按下事件
			LButtonDown(hWnd, wParam, lParam);
			break;
		case WM_LBUTTONUP:
			// 鼠标左键松开事件
			LButtonUp(hWnd, wParam, lParam);
			break;
		case WM_TIMER:
			// 定时器事件
			if (currentStage != NULL && currentStage->timerOn) TimerUpdate(hWnd, wParam, lParam);
			break;
		case WM_PAINT:
			// 绘图
			Paint(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


#pragma endregion



#pragma region 加载图像资源 添加按钮


void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//加载图像资源
	bmp_Background = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BG));
	bmp_StartButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_START));
	bmp_Hero = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_HERO));
	bmp_HelpButton= LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_START));
	bmp_BlockDirt= LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_DIRT));
	bmp_BlockGrass = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_GRASS));
	bmp_BlockThorn = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_THORN));
	bmp_BloodBody = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BLOODBODY));
	bmp_BlockSave = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_SAVE));
	bmp_BlockFire = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_FIRE));
	bmp_BurnedBody = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BURNEDBODY));
	bmp_BlockIce = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_ICE));
	bmp_FreezedBody = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_FREEZED));



	//添加按钮

	Button* startButton = CreateButton(BUTTON_STARTGAME, bmp_StartButton, BUTTON_STARTGAME_WIDTH, BUTTON_STARTGAME_HEIGHT, 300, 300);
	buttons.push_back(startButton);
	Button* helpButton = CreateButton(BUTTON_HELP, bmp_HelpButton, BUTTON_STARTGAME_WIDTH, BUTTON_STARTGAME_HEIGHT, 300, 400);
	buttons.push_back(helpButton);


	//初始化开始场景
	InitStage(hWnd, STAGE_STARTMENU);

	//初始化主计时器
	SetTimer(hWnd, TIMER_GAMETIMER, TIMER_GAMETIMER_ELAPSE, NULL);
}


#pragma endregion



#pragma region 事件处理函数
// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case VK_UP:
			keyUpDown = true;
			break;
		case VK_DOWN:
			keyDownDown = true;
			break;
		case VK_LEFT:
			keyLeftDown = true;
			break;
		case VK_RIGHT:
			keyRightDown = true;
			break;
		default:
			break;
	}
}

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// TODO
	switch (wParam)
	{
		case VK_UP:
			keyUpDown = false;
			break;
		case VK_DOWN:
			keyDownDown = false;
			break;
		case VK_LEFT:
			keyLeftDown = false;
			break;
		case VK_RIGHT:
			keyRightDown = false;
			break;
		default:
			break;
	}
}

// 鼠标移动事件处理函数
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
}

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = true;
	
	//鼠标与按钮交互
	for (int i = 0; i < buttons.size(); i++) 
	{
		Button* button = buttons[i];
		if (button->visible)
		{
			if (button->x <= mouseX
				&& button->x + button->width >= mouseX
				&& button->y <= mouseY
				&& button->y + button->height >= mouseY)
			{
				switch (button->buttonID) {
					case BUTTON_STARTGAME:
					{
						InitStage(hWnd, STAGE_1);
						break;
					}
					case BUTTON_HELP:
					{
						InitStage(hWnd, STAGE_HELP);
						break;
					}
					default:
						break;
				}
			}
		}
	}

}

// 鼠标左键松开事件处理函数
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = false;
}

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UpdateHero(hWnd); //刷新主角
	UpdateSurround(hWnd); //刷新环境
	
	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}


#pragma endregion



#pragma region 新建结构体函数


// 添加主角函数
Hero* CreateHero(HBITMAP img, int x, int y)
{
	Hero* hero = new Hero();
	hero->img = img;
	hero->x = x;
	hero->y = y;
	hero->frame = 0;
	hero->vx = 0;
	hero->vy = 0;
	return hero;
}


// 添加按钮函数
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y)
{
	Button* button = new Button();
	button->buttonID = buttonID;
	button->img = img;
	button->width = width;
	button->height = height;
	button->x = x;
	button->y = y;
	button->visible = false;
	return button;
}


//添加方块函数
Block* CreateBlock(int blockID, HBITMAP img, int width, int height, int x, int y)
{
	Block*block = new Block();
	block->blockID = blockID;
	block->img = img;
	block->width = width;
	block->height = height;
	block->x = x;
	block->y = y;
	block->visible = false;
	block->frame = 0;
	block->vx = 0;
	block->m = 0;
	return block;
}


#pragma endregion



#pragma region 加载数据资源函数


// 初始化按钮函数(stageID+1)*1000+i=第i个buttonID
void InitButton(HWND hWnd, int stageID)
{
	for (int i = 0; i < buttons.size(); i++)
	{
		Button* button = buttons[i];
		if (button->buttonID / 1000  == stageID)
		{
			button->visible = true;
		}
		else
		{
			button->visible = false;
		}
	}
}


// 初始化方块函数(stageID+1)*1000+i=第i个blockID
void InitBlock(HWND hWnd, int stageID)
{
	for (int i = 0; i < blocks.size(); i++)
	{
		Block* block = blocks[i];
		if (block->blockID / 1000  == stageID)
		{
			block->visible = true;
		}
		else
		{
			block->visible = false;
		}
	}
}


// 初始化地图函数
void InitMap(HWND hWnd, int stageID)
{

	//释放缓存
	for (int i = 0; i < blocks.size(); i++)
	{
		delete blocks[i];
		blocks[i] = NULL;
	}
	vector<Block*>().swap(blocks);

	//添加方块
	Block* dirt, *thorn, *savepoint, *fire, *ice;
	switch (stageID)
	{
		case STAGE_STARTMENU:
		{
			break;
		}
		case STAGE_1:
		{
			for (int i = 0; i < 35; i++)
			{
				dirt = new Block;
				dirt = CreateBlock(1000 + BLOCK_DIRT, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, 0 + i * BLOCK_SIZE_X, 350);
				blocks.push_back(dirt);
			}

			dirt = new Block;
			dirt = CreateBlock(1000 + BLOCK_DIRT, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, 100, 350 - BLOCK_SIZE_Y);
			blocks.push_back(dirt);


			dirt = new Block;
			dirt = CreateBlock(1000 + BLOCK_DIRT, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, 200, 300 - 2 * BLOCK_SIZE_Y);
			blocks.push_back(dirt);

			dirt = new Block;
			dirt = CreateBlock(1000 + BLOCK_DIRT, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, 200, 300 - 3 * BLOCK_SIZE_Y);
			blocks.push_back(dirt);

			
			dirt = new Block;
			dirt = CreateBlock(1000 + BLOCK_DIRT, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, 30 * BLOCK_SIZE_X, 350 - BLOCK_SIZE_Y);
			blocks.push_back(dirt);

			savepoint = new Block;
			savepoint = CreateBlock(1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 400, 0);
			blocks.push_back(savepoint);

			CurrentSave = savepoint;//保存初始存档点
			CurrentSave->frame = 1;


			savepoint = new Block;
			savepoint = CreateBlock(1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 0, 350 - BLOCK_SIZE_Y);
			blocks.push_back(savepoint);

			fire = CreateBlock(1000 + BLOCK_FIRE, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, 300, 350 - BLOCK_SIZE_Y);
			blocks.push_back(fire);

			savepoint = CreateBlock(1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 600, 350 - BLOCK_SIZE_Y);
			blocks.push_back(savepoint);

			savepoint = CreateBlock(1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 800, 350 - BLOCK_SIZE_Y);
			blocks.push_back(savepoint);

			ice = CreateBlock(1000 + BLOCK_ICE, bmp_BlockIce, BLOCK_SIZE_X, BLOCK_SIZE_Y, 700, 350 - 3*BLOCK_SIZE_Y);
			blocks.push_back(ice);
			break;
		}
		case STAGE_HELP:
			break;
		default:
			break;
	}

	//初始化方块
	InitBlock(hWnd, stageID);

}



#pragma endregion



#pragma region 初始化游戏场景函数

void InitStage(HWND hWnd, int stageID)
{
	// 初始化场景实例
	if (currentStage != NULL) delete currentStage;
	currentStage = new Stage();
	currentStage->stageID = stageID;

	//初始化计时器
	if (stageID == STAGE_STARTMENU) {
		currentStage->timeCountDown = 0;
		currentStage->timerOn = false;
	}
	else {
		currentStage->timeCountDown = 10000;
		currentStage->timerOn = true;
	}


	//初始化背景
	switch (stageID)
	{
		case STAGE_STARTMENU:
			currentStage->bg = bmp_Background;
			break;
		case STAGE_1:
			currentStage->bg = bmp_Background;
			break;
		case STAGE_HELP:
			currentStage->bg = bmp_Background;
		default:
			currentStage->bg = bmp_Background;
			break;
	}


	//显示游戏地图
	InitMap(hWnd, stageID);


	//显示游戏界面的按钮
	InitButton(hWnd, stageID);


	// 初始化主角
	if (theHero != NULL) delete theHero;
	theHero = CreateHero(bmp_Hero, 400, 0);


	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}


#pragma endregion



#pragma region 主角状态函数


//陷阱检测
void TrapDetect(HWND hwnd)
{
	int herocenterX = theHero->x + HERO_SIZE_X / 2;
	int herocenterY = theHero->y + HERO_SIZE_Y / 2;
	int blockX = 0, blockY = 0;


	for (int i = 0; i < blocks.size(); i++)
	{
		Block* block = blocks[i],*body=NULL;
		blockX = block->x + BLOCK_SIZE_X / 2;
		blockY = block->y + BLOCK_SIZE_Y / 2;
		if (block->visible)
			if (abs(herocenterX - blockX) <= block->width / 2 + HERO_SIZE_X / 2
				&& abs(herocenterY - blockY) <= block->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
				switch (block->blockID % 100)
				{
					case BLOCK_THORN://尖刺方块判定
					{
						body = CreateBlock(1000 + BLOCK_STILLBODY, bmp_BloodBody, HERO_SIZE_X, HERO_SIZE_Y, theHero->x, theHero->y);
						body->visible = true;
						blocks.push_back(body);
						delete theHero;
						theHero = NULL;
						return;
					}
					case BLOCK_FIRE://火焰方块判定
					{
						body = CreateBlock(1000 + BLOCK_BURNEDBODY, bmp_BurnedBody, HERO_SIZE_X, HERO_SIZE_Y, theHero->x, theHero->y);
						body->visible = true;
						blocks.push_back(body);
						delete theHero;
						theHero = NULL;
						return;
					}
					case BLOCK_ICE://冰焰方块判定
					{
						body = CreateBlock(1000 + BLOCK_FREEZE, bmp_FreezedBody, HERO_SIZE_X, HERO_SIZE_Y, theHero->x, theHero->y);
						body->visible = true;
						blocks.push_back(body);
						delete theHero;
						theHero = NULL;
						return;
					}
					default:
						break;
				}
	}
}



// 碰撞检测
bool CollitionDetect(HWND hwnd)
{
	bool onground = false; //是否落地
	int herocenterX = theHero->x + HERO_SIZE_X / 2; 
	int herocenterY = theHero->y + HERO_SIZE_Y / 2; 
	int blockX = 0, blockY = 0;
	

	//方块检测
	for (int i = 0; i < blocks.size(); i++)
	{
		Block* block = blocks[i];
		blockX = block->x + BLOCK_SIZE_X / 2; 
		blockY = block->y + BLOCK_SIZE_Y / 2; 
		if (block->visible)
			switch (block->blockID % 100)
			{
				case BLOCK_THORN:
					break;

				case BLOCK_FIRE:
					break;

				case BLOCK_BURNEDBODY:
					break;

				case BLOCK_SAVE://存档点判定
				{
					if (abs(herocenterX - blockX) < block->width / 2 + HERO_SIZE_X / 2
						&& abs(herocenterY - blockY) < block->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
					{
						CurrentSave->img = bmp_BlockSave;
						CurrentSave->frame = 0;
						block->frame = 1;
						CurrentSave = block;
					}
					break;
				}
				

				default://一般方块判定
				{
					if (abs(herocenterX - blockX) < block->width / 2 + HERO_SIZE_X / 2
						&& abs(herocenterY - blockY) < block->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
					{
						if (block->width / 2 + HERO_SIZE_Y / 2 - abs(herocenterY - blockY)
							< block->height / 2 + HERO_SIZE_X / 2 - abs(herocenterX - blockX)) //判定此次碰撞为纵向碰撞
						{
							if (herocenterY < blockY) //判定从方块上方碰撞
							{
								while (theHero->y + HERO_SIZE_Y > block->y)theHero->y -= 1; 
								theHero->vy = 0;
							}
							else //判定从方块下方碰撞
							{
								while (theHero->y < block->y + BLOCK_SIZE_Y)theHero->y += 1;
								theHero->vy = 0;
							}
						}
					}

					if (theHero->y + HERO_SIZE_Y == block->y
						&&abs(herocenterX - blockX) < block->width / 2 + HERO_SIZE_X / 2) //判定主角落在方块上
						onground = true;

					if (abs(herocenterX - blockX) < block->width / 2 + HERO_SIZE_X / 2
						&& abs(herocenterY - blockY) <= block->height / 2 + HERO_SIZE_Y / 2) //判定碰撞，边界条件改变
					{
						if (block->width / 2 + HERO_SIZE_Y / 2 - abs(herocenterY - blockY) >
							block->height / 2 + HERO_SIZE_X / 2 - abs(herocenterX - blockX)) //判定此次碰撞为横向碰撞
						{
							if (herocenterX > blockX) //判定从方块右面碰撞
							{
								while (theHero->x < block->x + BLOCK_SIZE_X)theHero->x += 1;
								theHero->vx = 0;
							}
							else { //判定从方块左面碰撞
								while (theHero->x + HERO_SIZE_X > block->x)theHero->x -= 1;
								theHero->vx = 0;
							}
							onground = false; //判定撞墙，不算落地
						}
					}
				}
			}
	}
	return onground;
}


//主角状态刷新

void UpdateHero(HWND hWnd)
{
	//判断陷阱
	if (theHero != NULL)
		TrapDetect(hWnd);

	 //主角重生
	if ((theHero == NULL) && keyUpDown)
		theHero = CreateHero(bmp_Hero, CurrentSave->x, CurrentSave->y);


	if (theHero != NULL) {

		//碰撞检测，判定落地
		bool onground = CollitionDetect(hWnd);
	
		//移动判定
		//y方向速度
		if (keyUpDown && onground) //跳跃
			theHero->vy = -10.0;
		else if (!onground) //重力
			theHero->vy += GRAVITION;

		//x方向速度
		if (keyLeftDown && !keyRightDown && theHero->vx > -1 * HERO_MAXSPEED)
			theHero->vx -= HERO_ACCELERATION;
		else if (!keyLeftDown && keyRightDown && theHero->vx < HERO_MAXSPEED)
			theHero->vx += HERO_ACCELERATION;
		else if (theHero->vx > 0)theHero->vx -= HERO_ACCELERATION;
		else if (theHero->vx < 0)theHero->vx += HERO_ACCELERATION;

		//计算位移
		theHero->x += (int)(theHero->vx);
		theHero->y += (int)(theHero->vy);
		
		
		
		//超出地图边界
		if (theHero->y > WINDOW_HEIGHT)
		{
			delete theHero;
			theHero = NULL;
		}
	}
}


#pragma endregion



#pragma region 环境状态函数


//尸体陷阱检测
void BodyTrapDetect(HWND hwnd,Block*body)
{
	int bodycenterX = body->x + HERO_SIZE_X / 2;
	int bodycenterY = body->y + HERO_SIZE_Y / 2;
	int blockX = 0, blockY = 0;

	if (body->blockID % 100 == BLOCK_FREEZE)
	{
		for (int i = 0; i < blocks.size(); i++)
		{
			Block* block = blocks[i];
			blockX = block->x + BLOCK_SIZE_X / 2;
			blockY = block->y + BLOCK_SIZE_Y / 2;
			if (block->visible)
				if (abs(bodycenterX - blockX) <= block->width / 2 + HERO_SIZE_X / 2
					&& abs(bodycenterY - blockY) <= block->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
					switch (block->blockID % 100)
					{
						case BLOCK_FIRE://火焰方块判定
						{
							body->img = bmp_BloodBody;
							body->m ++;
							if (body->m == 15)
							{
								body->blockID = BLOCK_MOVABLEBODY;
								body->m = 0;
							}
						}
						default:
							break;
					}
		}
	}

	if (body->blockID % 100 == BLOCK_MOVABLEBODY)
	{
		for (int i = 0; i < blocks.size(); i++)
		{
			Block* block = blocks[i];
			blockX = block->x + BLOCK_SIZE_X / 2;
			blockY = block->y + BLOCK_SIZE_Y / 2;
			if (block->visible)
				if (abs(bodycenterX - blockX) <= block->width / 2 + HERO_SIZE_X / 2
					&& abs(bodycenterY - blockY) <= block->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
					switch (block->blockID % 100)
					{
						case BLOCK_FIRE://火焰方块判定
						{
							body->blockID = BLOCK_BURNEDBODY;
							body->img = bmp_BurnedBody;
							return;
						}
						default:
							break;
					}
		}
	}
}

//尸体碰撞检测
bool BodyCollitionDetect(HWND hwnd, Block*body)
{
	bool onground = false; //是否落地
	int bodycenterX = body->x + HERO_SIZE_X / 2;
	int bodycenterY = body->y + HERO_SIZE_Y / 2;
	int blockX = 0, blockY = 0;

	//尸体与主角交互
	if (theHero != NULL)
	{
		int herocenterX = theHero->x + HERO_SIZE_X / 2;
		int herocenterY = theHero->y + HERO_SIZE_Y / 2;
		if (abs(herocenterX - bodycenterX) <= body->width / 2 + HERO_SIZE_X / 2
			&& abs(herocenterY - bodycenterY) <= body->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
		{
			if (body->width / 2 + HERO_SIZE_Y / 2 - abs(herocenterY - bodycenterY) >
				body->height / 2 + HERO_SIZE_X / 2 - abs(herocenterX - bodycenterX))//判定此次碰撞为横向碰撞
			{
				if (herocenterX > bodycenterX) //判定从方块右面碰撞
				{
					while (theHero->x <= body->x + BLOCK_SIZE_X)body->x -= 1;
				}
				else { //判定从方块左面碰撞
					while (theHero->x + HERO_SIZE_X >= body->x)body->x += 1;
				}
				body->vx = theHero->vx;
			}
		}
	}

	//方块检测
	for (int i = 0; i < blocks.size(); i++)
	{
		Block* block = blocks[i];
		if ((block->x == body->x)
			&& (block->y == body->y))
			if (i == blocks.size())
				break;
			else
				continue;
			
		blockX = block->x + BLOCK_SIZE_X / 2;
		blockY = block->y + BLOCK_SIZE_Y / 2;



		if (block->visible)
			switch (block->blockID % 100)
			{
				case BLOCK_THORN:
					break;

				case BLOCK_FIRE:
					break;

				case BLOCK_BURNEDBODY:
					break;

				case BLOCK_SAVE:
					break;

				case BLOCK_ICE:
					break;


				default://一般方块判定
				{
					if (abs(bodycenterX - blockX) < block->width / 2 + HERO_SIZE_X / 2
						&& abs(bodycenterY - blockY) < block->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
					{
						if (block->width / 2 + HERO_SIZE_Y / 2 - abs(bodycenterY - blockY)
							< block->height / 2 + HERO_SIZE_X / 2 - abs(bodycenterX - blockX)) //判定此次碰撞为纵向碰撞
						{
							if (bodycenterY < blockY) //判定从方块上方碰撞
							{
								while (body->y + HERO_SIZE_Y > block->y)body->y -= 1;
								body->vy = 0;
							}
						}
					}

					if (body->y + HERO_SIZE_Y == block->y
						&&abs(bodycenterX - blockX) < block->width / 2 + HERO_SIZE_X / 2) //判定尸体落在方块上
						onground = true;

					if (abs(bodycenterX - blockX) <= block->width / 2 + HERO_SIZE_X / 2
						&& abs(bodycenterY - blockY) <= block->height / 2 + HERO_SIZE_Y / 2) //判定碰撞，边界条件改变
					{
						if (block->width / 2 + HERO_SIZE_Y / 2 - abs(bodycenterY - blockY) >
							block->height / 2 + HERO_SIZE_X / 2 - abs(bodycenterX - blockX)) //判定此次碰撞为横向碰撞
						{
							if (bodycenterX > blockX) //判定方块右面碰撞尸体左面
							{
								while (body->x < block->x + BLOCK_SIZE_X)body->x += 1;
								if (abs(body->vx) > abs(block->vx))
									body->vx = block->vx;
								else
									block->vx = body->vx;
							}
							else { //判定方块左面碰撞尸体右面
								while (body->x + HERO_SIZE_X > block->x)body->x -= 1;
								if (abs(body->vx) > abs(block->vx))
									body->vx = block->vx;
								else
									block->vx = body->vx;
							}
						}
					}
				}
			}	
	}
	return onground;
}

//刷新尸体
void UpdateBody(HWND hWnd, Block*body)
{
	//判断陷阱
	if ((body->blockID % 100 == BLOCK_MOVABLEBODY)
		|| (body->blockID % 100 == BLOCK_FREEZE))
		BodyTrapDetect(hWnd, body);

	if ((body->blockID % 100 == BLOCK_MOVABLEBODY)
		|| (body->blockID % 100 == BLOCK_FREEZE))
	{

		bool onground = BodyCollitionDetect(hWnd,body);

		//移动判定
		//y方向速度
		 if (!onground) //重力
			body->vy += GRAVITION;

		//x方向速度
		
		//计算位移
		body->x += (int)(body->vx);
		body->y += (int)(body->vy);


		//超出地图边界
		if (body->y > WINDOW_HEIGHT)
		{
			body->blockID = BLOCK_DIRT;
			body->visible = false;
			body->vy = 0;
		}
	}
}


//刷新环境
int n = 0;
void UpdateSurround(HWND hWnd)
{
	for (int i = 0; i < blocks.size(); i++)
	{
		Block*block = blocks[i];
		if (block->visible)
			switch (block->blockID % 100)
			{
				case BLOCK_BURNEDBODY:
				{
					n++;
					if (n == 5)
					{
						block->frame++;
						n = 0;
					}
					if (block->frame > 10)
						block->visible = false;
					break;
				}

				case BLOCK_FIRE:
				{
					block->frame++;
					if (block->frame > 31)
						block->frame = 0;
					break;
				}

				case BLOCK_ICE:
				{
					block->frame++;
					if (block->frame > 31)
						block->frame = 0;
					break;
				}

				case BLOCK_MOVABLEBODY:
				{
					UpdateBody(hWnd,block);
					break;
				}
				case BLOCK_FREEZE:
				{
					UpdateBody(hWnd, block);
					break;
				}
				default:
					break;
			}
	}
}


#pragma endregion



#pragma region 绘图函数


// 绘图函数
void Paint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	//初始化缓存
	HBITMAP	blankBmp = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, blankBmp);

	// 绘制背景到缓存
	SelectObject(hdc_loadBmp, currentStage->bg);
	BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);



	//绘制方块到缓存
	for (int i = 0; i < blocks.size(); i++)
	{
		Block* block = blocks[i];
		if (block->visible)
		{
			SelectObject(hdc_loadBmp, block->img);
			TransparentBlt(
				hdc_memBuffer, block->x, block->y,
				block->width, block->height,
				hdc_loadBmp, 0, block->frame*BLOCK_SIZE_Y, block->width, block->height,
				RGB(255, 255, 255)
			);
		}
	}


	// 绘制按钮到缓存
	for (int i = 0; i < buttons.size(); i++)
	{
		Button* button = buttons[i];
		if (button->visible)
		{
			SelectObject(hdc_loadBmp, button->img);
			TransparentBlt(
				hdc_memBuffer, button->x, button->y,
				button->width, button->height,
				hdc_loadBmp, 0, 0, button->width, button->height,
				RGB(255, 255, 255)
			);
		}
	}


	// 按场景分类绘制主角到缓存
	if (theHero != NULL)
		switch (currentStage->stageID) //TODO：添加多个游戏场景
		{
			// 绘制主角到缓存
			case STAGE_1:
			{

				SelectObject(hdc_loadBmp, theHero->img);
				TransparentBlt(
					hdc_memBuffer, theHero->x, theHero->y,
					HERO_SIZE_X, HERO_SIZE_Y,
					hdc_loadBmp, 0, 0, HERO_SIZE_X, HERO_SIZE_Y,
					RGB(255, 255, 255)
				);
			}
			default:
				break;
		}


	// 最后将所有的信息绘制到屏幕上
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	// 回收资源所占的内存（非常重要）
	DeleteObject(blankBmp);
	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);

	// 结束绘制
	EndPaint(hWnd, &ps);
}
#pragma endregion