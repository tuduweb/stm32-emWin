#include "includes.h"
#include  "app.h"
#include "pysearch.h"
/*
*********************************************************************************************************
*                                         宏定义 
*********************************************************************************************************
*/ 

#define COLOR_BORDER           0xBD814F
#define COLOR_KEYPAD0          0xA8D403
#define COLOR_KEYPAD1          0xBF5C00
#define ID_BUTTON              (GUI_ID_USER + 0)

#define BUTTON_WHITH   77
#define BUTTON_HIGHT   27
#define BUTTON_SpaceX  (2+BUTTON_WHITH) 
#define BUTTON_SpaceY  (3+BUTTON_HIGHT)

/*
*********************************************************************************************************
*                                      变量
*********************************************************************************************************
*/ 
static TEXT_Handle ahText[2];
static WM_HWIN hEdit;
WM_HWIN KEYBOARD;
extern GUI_FONT     XBF_Font;


static uint8_t s_Key;

static char s_Textbuf[10]={0};

uint8_t  keyflag=0;//键盘是否在使用：1：使用中，0:无效中
static  uint8_t  editflag=0;//判断是edit还是multiedit：1:multiedit，0：edit
typedef struct {
  int          xPos;
  int          yPos;
  int          xSize;
  int          ySize;
  const char * acLabel;   /* 按钮对应的小写字符 */
  const char * acLabel1;  /* 按钮对应的大写字符 */
  char         Control;   /* 按钮对应的控制位，1表示功能按钮，0表示普通按钮 */
} BUTTON_DATA;

static const BUTTON_DATA _aButtonData[] = 
{

	/* 第1排按钮 */
	{ 2,                   5+BUTTON_SpaceY*1, 				 	BUTTON_WHITH, 		BUTTON_HIGHT, "1" ,   	"1",  	0},//0
	{ 2+BUTTON_SpaceX,     5+BUTTON_SpaceY*1,  					BUTTON_WHITH, 		BUTTON_HIGHT, "2",    	"2",  	0},
	{ 2+BUTTON_SpaceX*2,   5+BUTTON_SpaceY*1,  					BUTTON_WHITH, 		BUTTON_HIGHT, "3",    	"3",  	0},

	{ 2+BUTTON_SpaceX*0,   5+BUTTON_SpaceY*2,  					BUTTON_WHITH, 		BUTTON_HIGHT, "4",    	"4",  	0},
	{ 2+BUTTON_SpaceX*1,   5+BUTTON_SpaceY*2,  					BUTTON_WHITH, 		BUTTON_HIGHT, "5",   	"5",  	0},
	{ 2+BUTTON_SpaceX*2,   5+BUTTON_SpaceY*2,  					BUTTON_WHITH, 		BUTTON_HIGHT, "6",   	"6",  	0},

	{ 2+BUTTON_SpaceX*0,   5+BUTTON_SpaceY*3,  					BUTTON_WHITH, 		BUTTON_HIGHT, "7",    	"7",  	0},
	{ 2+BUTTON_SpaceX*1,   5+BUTTON_SpaceY*3,  					BUTTON_WHITH, 		BUTTON_HIGHT, "8",    	"8",  	0},
	{ 2+BUTTON_SpaceX*2,   5+BUTTON_SpaceY*3,  					BUTTON_WHITH, 		BUTTON_HIGHT, "9",    	"9",  	0},

	{ 2,                   5+BUTTON_SpaceY*4,  BUTTON_WHITH, 		BUTTON_HIGHT, "",   "", 	1},//38
	{ 2+BUTTON_SpaceX*1,   5+BUTTON_SpaceY*4,  					BUTTON_WHITH, 	BUTTON_HIGHT, "0",    	"0",  	0},
	{ 2+BUTTON_SpaceX*2,   5+BUTTON_SpaceY*4,  BUTTON_WHITH, 		BUTTON_HIGHT, "Ok",    	"Ok",  	1},//43

};
/*
*********************************************************************************************************
*	函 数 名: _cbKeyPad
*	功能说明: 回调函数
*	形    参：pMsg  指针参数
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbKeyPad(WM_MESSAGE * pMsg) 
{
	WM_HWIN    hWin;
	WM_HWIN    hButton;
	int        Id;
	int        NCode;
	int        xSize;
	int        ySize;
	unsigned char  i;
	
	hWin = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		/* 创建所需的按钮 */
		case WM_CREATE:
			//遍历创建数组中所有按键
			for (i = 0; i < GUI_COUNTOF(_aButtonData); i++) 
			{
				hButton = BUTTON_CreateEx(_aButtonData[i].xPos, _aButtonData[i].yPos, _aButtonData[i].xSize, _aButtonData[i].ySize, 
				hWin, WM_CF_SHOW, 0, ID_BUTTON + i);
				BUTTON_SetFont(hButton, &GUI_Font16B_ASCII);
				BUTTON_SetText(hButton, _aButtonData[i].acLabel);
				BUTTON_SetTextAlign(hButton,GUI_TA_HCENTER|GUI_TA_VCENTER);
				BUTTON_SetFocussable(hButton, 0);
			}
			break;
		case WM_DELETE:
			keyflag=0;//使能键盘
			editflag=0;//默认是edit控件书写
			WM_DeleteWindow(ahText[0]);
			WM_DeleteWindow(ahText[1]);
		break;
		/* 绘制背景 */	
		case WM_PAINT:	
			xSize = WM_GetWindowSizeX(hWin);
			ySize = WM_GetWindowSizeY(hWin);
			GUI_SetColor(COLOR_BORDER);
			GUI_DrawRect(0, 0, xSize - 1, ySize - 1);
			GUI_DrawGradientV(1, 1, xSize - 2, ySize - 2, COLOR_KEYPAD0, COLOR_KEYPAD1);
			break;
		
		/* 用于处理按钮的消息 */
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch (NCode) 
			{
				case WM_NOTIFICATION_RELEASED:
					if (_aButtonData[Id - ID_BUTTON].acLabel) 
					{
							s_Key = _aButtonData[Id - ID_BUTTON].acLabel[0];


							if(Id==(ID_BUTTON+11) || Id==(ID_BUTTON+9))//if(strncmp((const char *)_aButtonData[Id - ID_BUTTON].acLabel, "Ok", sizeof(_aButtonData[Id - ID_BUTTON].acLabel)) == 0)
							{
								WM_DeleteWindow(hWin);
								keyflag=0;
								break;
							}

							/* 其它按键，但不能是功能键 */
							if(_aButtonData[Id - ID_BUTTON].Control == 0)
							{
								s_Textbuf[0] = s_Key;
								s_Textbuf[1] = '\0';
								if(editflag) MULTIEDIT_AddText(hEdit, s_Textbuf);
								else EDIT_AddKey(hEdit,s_Textbuf[0]);
							}
					}
				break;
			}//endSwitch
	}
}
/*
*********************************************************************************************************
*	函 数 名: MainAPP
*	功能说明: GUI主函数 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void keypad(uint16_t *message)
{
	//printf("pend size=%d,hedit[0]=%d,hedit[1]=%d\n",GUI_COUNTOF(message),message[0],message[1]);
	keyflag=1;//使能键盘
	editflag=0;//默认是edit控件书写
	/***************************设置默认字体**********************************/
	GUI_UC_SetEncodeUTF8();
    //GUI_SetDefaultFont(&XBF_Font);
	/*************************************************************************/
	
	/* 创建键盘窗口 */
	KEYBOARD=WM_CreateWindowAsChild(0, 320-155, 240, 155, WM_HBKWIN, WM_CF_SHOW | WM_CF_STAYONTOP, _cbKeyPad, 0);

	/* 两个文本控件用于汉字输入时，拼音和汉字的显示 */
	ahText[0] = TEXT_Create( 0, 320-155, 240, 30, GUI_ID_TEXT0, WM_CF_SHOW | WM_CF_STAYONTOP,"",TEXT_CF_HCENTER | TEXT_CF_VCENTER);
	TEXT_SetFont(ahText[0],&XBF_Font);
	TEXT_SetText(ahText[0],"大喵爱吃小鱼");

	ahText[1] = TEXT_Create( 0, 145, 240, 20, GUI_ID_TEXT1, WM_CF_SHOW | WM_CF_STAYONTOP,"",TEXT_CF_LEFT | TEXT_CF_BOTTOM);
	TEXT_SetFont(ahText[1],&XBF_Font);	
	TEXT_SetTextColor(ahText[1],GUI_RED);
	//控件句柄
	hEdit = message[0];
	//当前编辑的是multiedit控件
	if(message[1]>=0x190 && message[1]<=0x193)
		editflag=1;
	
	/* 设置聚焦 */
	WM_SetFocus(hEdit);
	while(keyflag)
	{
		GUI_Delay(10);
	}
}
