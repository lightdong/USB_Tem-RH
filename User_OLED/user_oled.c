#include <stdio.h>
#include "i2c.h"
#include "user_oled.h"
#include "oled_font.h"
/*********************user_oled.c中自已定义的函数******************/
/******************************************************************/
/**
  *@brief 能I2C外设向OLED发送一个指令
  *@arg	  uint_t comd：一个指令(数据手册)可查
  *@retval none
  */
void OLED_WriteComd(uint8_t comd){
	uint8_t comd_temp[2] = {OLED_WRITE_COMD,comd};
	HAL_I2C_Master_Transmit(&hi2c1 ,OLED_ADDR_WRITE,comd_temp,2,10);
}
/**
  *@brief 能I2C外设向OLED发送一个数据
  *@arg	  uint_t comd：一个数据(自己想发送的)
  *@retval none
  */
void OLED_WriteData(uint8_t data){
	uint8_t comd_temp[2] = {OLED_WRITE_DATA,data};
	HAL_I2C_Master_Transmit(&hi2c1 ,OLED_ADDR_WRITE,comd_temp,2,10);
}
/******************************************************************/
/**
  *@brief 设置光标的位置
  *@arg1  char Y 设置在哪个页位置（0-7）
  *@arg2  char X 设置在哪个列位置（0-127）
  *@retval none
  */
void OLED_SetCursor(uint8_t Y,uint8_t X){
	OLED_WriteComd(0xB0 | Y);					//设置Y位置
	OLED_WriteComd(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteComd(0x00 | (X & 0x0F));	
}
/**
  *@brief 清屏：屏幕上不显示任何东西
  *@arg1  char Y 设置在哪个页位置（0-7）
  *@arg2  char X 设置在哪个列位置（0-127）
  *@retval true(1):清屏成功
  *		   false(0):发送失败
  */
void OLED_Clear(void){
	uint8_t i;
	uint8_t j;
	for(i = 0;i < 8;i++){
		OLED_SetCursor(i,0);
		for(j = 0;j < 128;j++){
			OLED_WriteData(0x00);
		}
	}
}

/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}
/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}
/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_WriteComd(0xAE);	//关闭显示
	
	OLED_WriteComd(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteComd(0x80);
	
	OLED_WriteComd(0xA8);	//设置多路复用率
	OLED_WriteComd(0x3F);
	
	OLED_WriteComd(0xD3);	//设置显示偏移
	OLED_WriteComd(0x00);
	
	OLED_WriteComd(0x40);	//设置显示开始行
	
	OLED_WriteComd(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteComd(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_WriteComd(0xDA);	//设置COM引脚硬件配置
	OLED_WriteComd(0x12);
	
	OLED_WriteComd(0x81);	//设置对比度控制
	OLED_WriteComd(0xCF);

	OLED_WriteComd(0xD9);	//设置预充电周期
	OLED_WriteComd(0xF1);

	OLED_WriteComd(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteComd(0x30);

	OLED_WriteComd(0xA4);	//设置整个显示打开/关闭

	OLED_WriteComd(0xA6);	//设置正常/倒转显示

	OLED_WriteComd(0x8D);	//设置充电泵
	OLED_WriteComd(0x14);

	OLED_WriteComd(0xAF);	//开启显示
		
	OLED_Clear();				//OLED清屏
}
