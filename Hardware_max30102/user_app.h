/**
  ******************************************************************************
  * @file   user_app.h
  * @brief  用户应用程序部分的代码
  * 
  ******************************************************************************
  */
#ifndef __USER_APP_H__
#define __USER_APP_H__
//
#include "stdio.h"  //串口实现Printf打印输出

//MAX30102驱动文件
#include "max30102.h"


enum{
	eUPDATE_TIME = 0,					/* 系统时间	*/		
	eUPDATE_FIVEKEY = 1,			/* 五向按键 */
	eUPDATE_CHIPINFO = 2,			/* 系统信息 */
	eUPDATE_SIX_AXIS = 3,			/* 六轴运动	*/
	eUPDATE_WIFI_RSSI = 4,		/* 信号强度	*/	
	eUPDATE_APP_INFO = 5,			/* APP页面信息 */		
	eUPDATE_HEART_RATE = 6,		/* 健康监测 */		
	eUPDATE_NIXIE_SHOW = 7		/* 数码管显示 */		
};
//任务使能标值
typedef struct
{
	uint32_t UPDATE_TIME_EN:1;					//系统时间任务使能
	uint32_t UPDATE_FIVEKEY_EN:1;				//五向按键任务使能				
	uint32_t UPDATE_CHIPINFO_EN:1;			//系统信息任务使能				
	uint32_t UPDATE_SIX_AXIS_EN:1;			//六轴运动任务使能
	uint32_t UPDATE_WIFI_RSSI_EN:1;			//WiFi联网任务使能		
	uint32_t UPDATE_APP_TASK_EN:1;			//APP页面任务使能		
	uint32_t UPDATE_HEART_RATE_EN:1;		//脉搏与血氧饱和度任务使能
	uint32_t UPDATE_NIXIE_SHOW_EN:1;		//数码管显示任务使能
	uint32_t   :24;
}gTask_MarkEN;

//任务状态标值
typedef struct
{
	uint32_t AP3216PS:1;			//AP3216的PS状态标值，用于标识物体是否接近
	uint32_t Sport:1;					//运动状态
	uint32_t ADCC:1;					//ADC转换完成
	uint32_t TouchPress:1;		//1：触摸按下 0：触摸释放
	uint32_t FiveKeyPress:1;	//1：五向键按下 0：触摸释放
	uint32_t ExtITR:1;				//1：光电开关触发 0：释放
	uint32_t ExtFIRE:1;				//1：火焰检测触发	0：释放 
	uint32_t ExtPIR:1;				//1：热释电触发	0：释放 
	uint32_t Max30102:1;			//1：max30102测量完成	0：释放 
	uint32_t   :23;
}gTask_BitDef;

// Retrieve year info
#define OS_YEAR     ((((__DATE__ [7] - '0') * 10 + (__DATE__ [8] - '0')) * 10 \
                                    + (__DATE__ [9] - '0')) * 10 + (__DATE__ [10] - '0'))

// Retrieve month info
#define OS_MONTH    (__DATE__ [2] == 'n' ? (__DATE__ [1] == 'a' ? 1 : 6) \
                                : __DATE__ [2] == 'b' ? 2 \
                                : __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 3 : 4) \
                                : __DATE__ [2] == 'y' ? 5 \
                                : __DATE__ [2] == 'l' ? 7 \
                                : __DATE__ [2] == 'g' ? 8 \
                                : __DATE__ [2] == 'p' ? 9 \
                                : __DATE__ [2] == 't' ? 10 \
                                : __DATE__ [2] == 'v' ? 11 : 12)

// Retrieve day info
#define OS_DAY      ((__DATE__ [4] == ' ' ? 0 : __DATE__ [4] - '0') * 10 \
                                + (__DATE__ [5] - '0'))

// Retrieve hour info
#define OS_HOUR     ((__TIME__ [0] - '0') * 10 + (__TIME__ [1] - '0'))

// Retrieve minute info
#define OS_MINUTE   ((__TIME__ [3] - '0') * 10 + (__TIME__ [4] - '0'))

// Retrieve second info
#define OS_SECOND   ((__TIME__ [6] - '0') * 10 + (__TIME__ [7] - '0'))
//
#define VREFINT_CAL ((uint16_t*) ((uint32_t) 0x0BFA07A5)) //内部参考电压源纠正值
#define TS_CAL1 		((uint16_t*) ((uint32_t) 0x0BFA0710)) //内部温度纠正值
//
#define RTC_BKP0RL_VAULE			0x1A1B
//系统报警
#define BEEP_ENABLE()						HAL_GPIO_WritePin(RUN_BEEP_GPIO_Port, RUN_BEEP_Pin, GPIO_PIN_SET)					/* 使能蜂鸣器鸣叫 */
#define BEEP_DISABLE()					HAL_GPIO_WritePin(RUN_BEEP_GPIO_Port, RUN_BEEP_Pin, GPIO_PIN_RESET)				/* 禁止蜂鸣器鸣叫 */
//系统运行指示灯与扩展指示灯LD1
#define RUN_LED_ENABLE()				HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_SET)		 			/* 使能运行指示灯 */
#define RUN_LED_DISABLE()	  		HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_RESET)				/* 禁止运行指示灯 */
#define RUN_LED_TURN_STATE()	  HAL_GPIO_TogglePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin)											/* 禁止运行指示灯 */
//扩展板指示灯LD2
#define EXT_LED2_ENABLE()				HAL_GPIO_WritePin(EXT_LED2_GPIO_Port, EXT_LED2_Pin, GPIO_PIN_SET)		 			/* 使能外部指示灯2 */
#define EXT_LED2_DISABLE()	  	HAL_GPIO_WritePin(EXT_LED2_GPIO_Port, EXT_LED2_Pin, GPIO_PIN_RESET)				/* 禁止外部指示灯2 */
//扩展板指示灯LD3
#define EXT_LED3_ENABLE()				HAL_GPIO_WritePin(EXT_LED3_GPIO_Port, EXT_LED3_Pin, GPIO_PIN_SET)		 			/* 使能外部指示灯3 */
#define EXT_LED3_DISABLE()	  	HAL_GPIO_WritePin(EXT_LED3_GPIO_Port, EXT_LED3_Pin, GPIO_PIN_RESET)				/* 禁止外部指示灯3 */
//扩展板风扇
#define EXT_FAN_ENABLE()				HAL_GPIO_WritePin(EXT_FAN_GPIO_Port, EXT_FAN_Pin, GPIO_PIN_SET)		 				/* 使能扩展板风扇 */
#define EXT_FAN_DISABLE()	  		HAL_GPIO_WritePin(EXT_FAN_GPIO_Port, EXT_FAN_Pin, GPIO_PIN_RESET)					/* 禁止扩展板风扇 */
//扩展板震动电机
#define EXT_MOTOR_ENABLE()			HAL_GPIO_WritePin(EXT_MOTOR_GPIO_Port, EXT_MOTOR_Pin, GPIO_PIN_SET)		 		/* 使能震动电机 */
#define EXT_MOTOR_DISABLE()	  	HAL_GPIO_WritePin(EXT_MOTOR_GPIO_Port, EXT_MOTOR_Pin, GPIO_PIN_RESET)			/* 禁止震动电机 */
//
void ESP8266_RSSI_Task(void);
void Update_AppPageInfo(void);
void Update_Backlight(uint8_t pDutyRatio);
void Update_ChipInfo(void);
void Update_FiveKey_Value(void);
void Update_System_Time(void);
void System_Time_init(void);
//
void mpu_init_dmp(void);
void Update_EulerAngle(void);
void Update_HeartRateInfo(void);
void Update_NixieDisplay(void);
 void mpu_get_max30102_data(void);
//定义的系统任务数量
#define OS_TASKLISTCNT	8  
extern void (* g_OSTsakList[OS_TASKLISTCNT])(void);
#endif /* __USER_APP_H__ */

