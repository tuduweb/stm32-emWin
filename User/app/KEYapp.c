﻿/**************************************************************************************
* 因为emWin显示只支持UTF-8编码格式的中文，如果希望直接显示在Keil直接输入的中文，      *
*            比如使用：GUI_DispStringHCenterAt("流水灯",110,120);                     *
* 该文件必须以UTF-8编码格式，不然中文无法正常显示。                                   *
*                                                                                     *
* 如果只是个别例程出现中文显示乱码（如果所有例程都无法显示中文可能是字库问题），      *
* 把对应的例程文件(比如LEDapp.c)用电脑的记事本软件打开，然后选择另存为，在弹出对      *
* 话框中“保存(S)"按钮的左边有个"编码(E)"选项，选择"UTF-8",然后同样保存为同名称的      *
* C语言文件(覆盖原来文件)，再编译。                                                   *
*                                                                                     *
* 如果编译工程时出现下面类似错误也是该文件编码格式问题,必须把文件保存为UTF-8格式      *
* 再编译                                                                              *
* ..\..\User\app\LEDapp.c(275): error:  #8: missing closing quote                     *
*        GUI_DispStringHCenterAt("娴?姘?鐏?",110,120);                                *
* ..\..\User\app\LEDapp.c(276): error:  #165: too few arguments in function call      *
*        GUI_DispStringHCenterAt("瑙?鎽?鍋?宸?澶?鎵?闇€瑕?瑙?鎽?鏍?鍑?",110,215);     *
* ..\..\User\app\LEDapp.c(276): error:  #18: expected a ")"                           *
*        GUI_DispStringHCenterAt("瑙?鎽?鍋?宸?澶?鎵?闇€瑕?瑙?鎽?鏍?鍑?",110,215);     *
*                                                                                     *
* 修改文件后编译就出错这是Keil5软件问题(Keil4没这问题)，推荐使用其他程序编辑工具，    *
* 只用Keil5完成编译和下载工作。                                                       *
***************************************************************************************
*                      实验平台: 野火STM32 ISO 开发板                                 *
*                      论    坛: http://www.chuxue123.com                             *
*                      淘    宝: http://firestm32.taobao.com                          *
*                      邮    箱: wildfireteam@163.com                                 *
***************************************************************************************
*/
/**************************************************************************************
*                                                                                     *
*                SEGGER Microcontroller GmbH & Co. KG                                 *
*        Solutions for real time microcontroller applications                         *
*                                                                                     *
***************************************************************************************
*                                                                                     *
* C-file generated by:                                                                *
*                                                                                     *
*        GUI_Builder for emWin version 5.22                                           *
*        Compiled Jul  4 2013, 15:16:01                                               *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG                                *
*                                                                                     *
***************************************************************************************
*                                                                                     *
*        Internet: www.segger.com  Support: support@segger.com                        *
*                                                                                     *
***************************************************************************************
*/
// USER START (Optionally insert additional includes)
#include "includes.h"
#include  "app.h"
// USER END
/**************************************************************************************
*
*       Defines
*
***************************************************************************************
*/


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
// USER START (Optionally insert additional static data)
extern GUI_FONT     XBF_Font;
extern __IO uint8_t key_flag;
extern KEY Key1,Key2;
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateKEY[] = {
  { FRAMEWIN_CreateIndirect, "KEY", 0, 0, 0, 240, 320, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "KEY1", GUI_ID_BUTTON0, 20, 55, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "KEY2", GUI_ID_BUTTON1, 120, 55, 80, 40, 0, 0x0, 0 },
	{ SLIDER_CreateIndirect, "LED_Slider", GUI_ID_SLIDER0, 35, 135, 170, 40, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Calibration", GUI_ID_BUTTON2, 118-60, 240, 120, 40, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialogKEY(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
	uint16_t i,v;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) 
  {
	  case WM_DELETE:
		// USER START (Optionally insert additional code for further widget initialization)
		  key_flag=1;
		  OS_INFO("KEYapp delete\n");
		  Flag_ICON100 = 0;
			UserApp_Flag = 0;		
			LED1_OFF;LED2_OFF;LED3_OFF;
		// USER END
		    break;
	  case WM_INIT_DIALOG:
		//
		// Initialization of 'KEY TEST'
		//
		hItem = pMsg->hWin;
		FRAMEWIN_SetTitleHeight(hItem, 20);
		FRAMEWIN_SetTextColor(hItem,GUI_DARKGRAY);
		FRAMEWIN_SetFont(hItem, GUI_FONT_16B_ASCII);
		FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
		
		//
		// Initialization of 'Calibration'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON2);
		BUTTON_SetText(hItem, "触摸校准");
		BUTTON_SetFont(hItem, &XBF_Font);
		// USER START (Optionally insert additional code for further widget initialization)
		key_flag=0;
		// USER END
		break;
	  case WM_NOTIFY_PARENT:
		Id    = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) {
		case GUI_ID_BUTTON0: // Notifications sent by 'KEY1'
		  switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
			// USER START (Optionally insert code for reacting on notification message)
			LED2_ON;
			// USER END
			break;
		  case WM_NOTIFICATION_RELEASED:
			// USER START (Optionally insert code for reacting on notification message)
		    LED2_OFF;
			// USER END
			break;
		  case WM_NOTIFICATION_MOVED_OUT:
			// USER START (Optionally insert code for reacting on notification message)
			LED2_OFF;
			// USER END
			break;
		  // USER START (Optionally insert additional code for further notification handling)
		  // USER END
		  }
		  break;
		case GUI_ID_BUTTON1: // Notifications sent by 'KEY2'
		  switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
			// USER START (Optionally insert code for reacting on notification message)
			LED3_ON;
			// USER END
			break;
		  case WM_NOTIFICATION_RELEASED:
			// USER START (Optionally insert code for reacting on notification message)
			LED3_OFF;
			// USER END
			break;
		   case WM_NOTIFICATION_MOVED_OUT:
			// USER START (Optionally insert code for reacting on notification message)
			LED3_OFF;
			// USER END
			break;
		  // USER START (Optionally insert additional code for further notification handling)
		  // USER END
		  }
		  break;
		case GUI_ID_BUTTON2: // Notifications sent by 'LED3'
		  switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
			// USER START (Optionally insert code for reacting on notification message)
			// USER END
			break;
		  case WM_NOTIFICATION_RELEASED:
			// USER START (Optionally insert code for reacting on notification message)
			SPI_FLASH_SectorErase(CALADD);
			Soft_Reset();
			//BUTTON_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0), "请按复位键");
			// USER END
			break;
		  case WM_NOTIFICATION_VALUE_CHANGED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END			    
				break;
		  }
		  break;
		case GUI_ID_SLIDER0: // Notifications sent by 'LED_Slider'
		  switch(NCode) 
		  {
			  case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
				break;
			  case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
				hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_SLIDER0);
				v = SLIDER_GetValue(hItem);
				WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0));
				WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON1));
			  WM_DisableWindow(hItem);
			    //printf("delay time=%d\n",v);
				i=10;
				while(i--)
				{
					LED1_ON;
					GUI_Delay(v);
					LED1_OFF;
					LED2_ON;
					GUI_Delay(v);
					LED2_OFF;
					LED3_ON;
					GUI_Delay(v);
					LED3_OFF;
					if(Flag_ICON100 == 0)return;
				}
				WM_EnableWindow(hItem);
				WM_EnableWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0));
				WM_EnableWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON1));
				LED1_OFF;LED2_OFF;LED3_OFF;
				break;
				
			  case WM_NOTIFICATION_VALUE_CHANGED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
				break;
		  }
		  break;
		// USER START (Optionally insert additional code for further Ids)
		// USER END
		}
		break;
	  // USER START (Optionally insert additional message handling)
		case WM_PAINT:	
			GUI_SetBkColor(APPBKCOLOR);
			GUI_SetColor(GUI_MAGENTA);
			GUI_Clear();
			GUI_SetTextMode(GUI_TM_TRANS);
			GUI_DrawGradientV(0, 205, 230, 295, GUI_CYAN, GUI_CYAN);		
			GUI_DispStringHCenterAt("按键控制LED亮灭",110,5);
			GUI_DispStringHCenterAt("(其他界面可用KEY2截屏)",110,30);		
			GUI_DispStringHCenterAt("流水灯",110,110);					
			GUI_DispStringHCenterAt("触摸偏差大才需要触摸校准",110,215);
		break;
	  // USER END
	  default:
		WM_DefaultProc(pMsg);
		break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateKEY TEST
*/
void FUN_ICON100Clicked(void)
{
	GUI_CreateDialogBox(_aDialogCreateKEY, GUI_COUNTOF(_aDialogCreateKEY), _cbDialogKEY, WM_HBKWIN, 0, 0);
	OS_INFO("KEYapp create\n");
	while(Flag_ICON100)
	{					 				
		Key_RefreshState(&Key1);//刷新按键状态
		Key_RefreshState(&Key2);//刷新按键状态
		if(Key_AccessTimes(&Key1,KEY_ACCESS_READ)!=0 && !key_flag)//按键被按下过
		{
			LED2_TOGGLE;
			Key_AccessTimes(&Key1,KEY_ACCESS_WRITE_CLEAR);
		}
		if(Key_AccessTimes(&Key2,KEY_ACCESS_READ)!=0 && !key_flag)//按键被按下过
		{
			LED3_TOGGLE;
			Key_AccessTimes(&Key2,KEY_ACCESS_WRITE_CLEAR);
		}
		GUI_Delay(10); 				
	}
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
