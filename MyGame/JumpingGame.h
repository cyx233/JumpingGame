#pragma region ͷ�ļ�����

// Windows ͷ�ļ�: 
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// ��Դͷ�ļ�
#include "resource.h"
#pragma comment(lib, "Msimg32.lib")


//����ͷ�ļ�
#include <vector>
#include <math.h>


#pragma endregion


#pragma region �궨��

#define WINDOW_WIDTH			1280	//��Ϸ���ڿ��
#define WINDOW_HEIGHT			720		//��Ϸ���ڸ߶�
#define WINDOW_TITLEBARHEIGHT	32		//�������߶�

#define STAGE_STARTMENU			0		//��ʼ�����ID
#define STAGE_1					1		//��һ����Ϸ������ID
#define STAGE_HELP				2		//��������ID

#define BLOCK_DIRT				1		//����
#define BLOCK_GRASS				2		//�ݵ�
#define BLOCK_SIZE_X			32		//ש��Ŀ��
#define BLOCK_SIZE_Y			32		//ש��ĸ߶�

#define HERO_SIZE_X				36		//���ǵĿ��
#define HERO_SIZE_Y				38		//���ǵĸ߶�
#define HERO_FRAME_NUMBER		8		//���ǵĶ���֡����
#define HERO_ACCELERATION		0.3		//���Ǽ��ٶ�
#define GRAVITION				0.6		//�������ٶ�
#define HERO_MAXSPEED			6.0		//��������ٶ�

#define BUTTON_STARTGAME			1001	//��ʼ��Ϸ��ťID
#define BUTTON_HELP					1002	//��Ϸ������ťID
#define BUTTON_STARTGAME_WIDTH		147		//��ʼ��Ϸ��ť���
#define BUTTON_STARTGAME_HEIGHT		60		//��ʼ��Ϸ��ť�߶�

#define TIMER_GAMETIMER				1	//��Ϸ��Ĭ�ϼ�ʱ��ID
#define TIMER_GAMETIMER_ELAPSE		10	//Ĭ�ϼ�ʱ��ˢ�¼���ĺ�����


#pragma endregion


#pragma region �ṹ������

// �����ṹ��
struct Stage
{
	int stageID;		//�������
	HBITMAP bg;			//����ͼƬ
	int timeCountDown;	//��Ϸʱ�䵹��ʱ
	bool timerOn;		//��ʱ���Ƿ����У���Ϸ�Ƿ���ͣ��

};


// ��ť�ṹ��
struct Button
{
	int buttonID;	//��ťID (stageID)*1000+��ť�������
	bool visible;	//��ť�Ƿ�ɼ�
	HBITMAP img;	//ͼƬ
	int x;			//����x
	int y;			//����y
	int width;		//���
	int height;		//�߶�
};

// ���ǽṹ��
struct Hero
{
	HBITMAP img;	//ͼƬ
	int frame;		//��ǰ��ʾ��֡��
	int x;			//����x
	int y;			//����y
	double vx;		//�ٶ�x
	double vy;		//�ٶ�y
};

//����ṹ��
struct Block
{
	int blockID;	//����ID  (stageID)*1000+��ť�������
	bool visible;	//�����Ƿ�ɼ�
	HBITMAP img;	//ͼƬ
	int x;			//����x
	int y;			//����y
	int width;		//���
	int height;		//�߶�
};


#pragma endregion


#pragma region �¼�����������


// ��ʼ����Ϸ���庯��
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam);

// ���̰����¼�������
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// �����ɿ��¼�������
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// ����ƶ��¼�������
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);

// �����������¼�������
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// �������ɿ��¼�������
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// ��ʱ���¼�������
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);

#pragma endregion


#pragma region ������Ϸ״̬����������

// ��Ӱ�ť����
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y);

//��ӷ��麯��
Block* CreateBlock(int blockID, HBITMAP img, int width, int height, int x, int y);

// ������Ǻ���
Hero* CreateHero(HBITMAP img, int x, int y);

// ��ʼ����������
void InitStage(HWND hWnd, int stageID);

// ��ʼ����ť����
void InitButton(HWND hWnd, int stageID);

// ��ʼ�����麯��
void InitBlock(HWND hWnd, int stageID);

// ��ʼ����ͼ����
void InitMap(HWND hWnd, int stageID);

// ˢ������״̬����
void UpdateHero(HWND hWnd);

// �������ǵĵ�ǰ֡��
int GetHeroFrame(double dirX, double dirY);

// ��ײ��⺯��
bool CollitionDetect(HWND hWnd);

#pragma endregion


#pragma region ��ͼ��������

// ��ͼ����
void Paint(HWND hWnd);

#pragma endregion