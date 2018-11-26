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
HBITMAP bmp_Hero; //主角图像
HBITMAP bmp_HelpButton; //帮助按钮图像
HBITMAP bmp_BlockDirt;  //泥土图像
HBITMAP bmp_BlockGrass;  //草地图像
HBITMAP bmp_BlockThorn;	//尖刺图像
HBITMAP bmp_BlockSave;	//存档点图像
HBITMAP bmp_BlockSaveOn;//触发存档点图像
HBITMAP bmp_BloodBody;	//带血尸体图像

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
	bmp_BlockSaveOn = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_SAVEON));

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
	UpdateHero(hWnd);

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
	block->turnon = false;
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
	switch (stageID)
	{
		case STAGE_STARTMENU:
		{
			break;
		}
		case STAGE_1:
		{
			Block* dirt; Block*thorn; Block*savepoint;
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
			dirt = CreateBlock(1000 + BLOCK_DIRT, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, 500, 360 - BLOCK_SIZE_Y - HERO_SIZE_Y);
			blocks.push_back(dirt);

			dirt = new Block;
			dirt = CreateBlock(1000 + BLOCK_DIRT, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, 200, 300 - 2 * BLOCK_SIZE_Y);
			blocks.push_back(dirt);

			dirt = new Block;
			dirt = CreateBlock(1000 + BLOCK_DIRT, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, 200, 300 - 3 * BLOCK_SIZE_Y);
			blocks.push_back(dirt);

			thorn = new Block;
			thorn = CreateBlock(1000 + BLOCK_THRON, bmp_BlockThorn, BLOCK_SIZE_X, BLOCK_SIZE_Y, 24 * BLOCK_SIZE_X, 350 - BLOCK_SIZE_Y);
			blocks.push_back(thorn);

			savepoint = new Block;
			savepoint = CreateBlock(1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 400, 0);
			blocks.push_back(savepoint);

			CurrentSave = savepoint;//保存初始存档点

			savepoint = new Block;
			savepoint->turnon = true;

			savepoint = CreateBlock(1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 0, 350 - BLOCK_SIZE_Y);
			blocks.push_back(savepoint);

			savepoint = CreateBlock(1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 200, 350 - BLOCK_SIZE_Y);
			blocks.push_back(savepoint);

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



#pragma region 主角相关函数


//陷阱检测
void TrapDetect(HWND hwnd)
{
	int herocenterX = theHero->x + HERO_SIZE_X / 2;
	int herocenterY = theHero->y + HERO_SIZE_Y / 2;
	int blockX = 0, blockY = 0;


	for (int i = 0; i < blocks.size(); i++)
	{
		Block* block = blocks[i];
		blockX = block->x + BLOCK_SIZE_X / 2;
		blockY = block->y + BLOCK_SIZE_Y / 2;
		if (block->visible)
			switch (block->blockID % 100)
			{
				case BLOCK_THRON://尖刺方块判定
				{
					if (abs(herocenterX - blockX) < block->width / 2 + HERO_SIZE_X / 2
						&& abs(herocenterY - blockY) < block->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
					{
						Block*body = NULL;
						body = CreateBlock(1000 + BLOCK_STILLBODY, bmp_BloodBody, HERO_SIZE_X, HERO_SIZE_Y, theHero->x, theHero->y);
						body->visible = true;
						blocks.push_back(body);
						delete theHero;
						theHero = NULL;
					}
					break;
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
				case BLOCK_SAVE://存档点判定
				{
					if (abs(herocenterX - blockX) < block->width / 2 + HERO_SIZE_X / 2
						&& abs(herocenterY - blockY) < block->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
					{
						CurrentSave->img = bmp_BlockSave;
						CurrentSave->turnon = false;
						block->img = bmp_BlockSaveOn;
						block->turnon = true;
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
		
		
		//主角动画
		/*
		int centerX = theHero->x + HERO_SIZE_X / 2;
		int centerY = theHero->y + HERO_SIZE_Y / 2;

		double dirX = mouseX - centerX;
		double dirY = mouseY - centerY;
		theHero->frame = GetHeroFrame(dirX, dirY);
		*/

		//超出地图边界
		if (theHero->y > WINDOW_HEIGHT)
		{
			delete theHero;
			theHero = NULL;
		}
	}
}


// 计算主角的当前帧数
/*int GetHeroFrame(double dirX, double dirY)
{
	double dirLen = sqrt(dirX * dirX + dirY * dirY);
	if (dirLen == 0)return 0;
	double cos = -dirY / dirLen;
	double arc = acos(cos);
	if (dirX < 0)arc = 2 * PI - arc;

	int frame = (int)(arc / (2 * PI / HERO_FRAME_NUMBER));
	if (frame == HERO_FRAME_NUMBER)
		frame = 0;

	if (frame < HERO_FRAME_NUMBER / 2)
		frame = HERO_FRAME_NUMBER / 2 - frame - 1;

	return frame;
}
*/




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
				hdc_loadBmp, 0, 0, block->width, block->height,
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