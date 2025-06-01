/**
  *@file brief 市面上常见的一个小OLED显示屏的驱动头文件
  *@个人建议：头文件这样来写
  *@1.引入头文件
  *@2.定义用到的变量(一般是结构体，与枚举型的)
  *@3.宏定义
  *@4.声明函数
  *@note 另外我只是一个搬运工，把驱动的固件库的软件模拟。
		修改为HAL库下的用外设处理数据。
  */


#ifndef __USER_OLED_H__
#define __USER_OLED_H__
/*引入头文件*/
#include <stdbool.h>

/*定义用到的变量*/
/*宏定义*/
#define OLED_ADDR_WRITE 0x78
#define OLED_WRITE_COMD 0x00
#define OLED_WRITE_DATA 0x40


/*声明函数*/
void OLED_SetCursor(uint8_t Y,uint8_t X);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
uint32_t OLED_Pow(uint32_t X, uint32_t Y);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

#endif
