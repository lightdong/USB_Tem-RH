/**
  ******************************************************************************
  * @file   user_app.c
  * @brief  用户应用程序部分的代码
  * 
  ******************************************************************************
  */
#include "string.h"
#include "math.h"
//
#include "user_app.h"
/***************************心率血氧传感器***************************/
float n_temperature;
#define MAX30102_ENABLE
#define MAX30102_BUFF_LENGTH 200	//Defined in the "algorithm.h",MAX30102_BUFF_LENGTH = BUFFER_SIZE
//
#ifdef MAX30102_ENABLE
#define MAX_BRIGHTNESS 255
//
uint32_t aun_ir_buffer[MAX30102_BUFF_LENGTH]; //IR LED sensor data
int32_t n_ir_buffer_length;    //data length
uint32_t aun_red_buffer[MAX30102_BUFF_LENGTH];    //Red LED sensor data
int32_t n_sp02; //SPO2 value
int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid
int32_t n_heart_rate;   //heart rate value
float n_temperature;	//体表温度
int8_t  ch_hr_valid;    //indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;
//
extern void maxim_heart_rate_and_oxygen_saturation(uint32_t *pun_ir_buffer,  int32_t n_ir_buffer_length, uint32_t *pun_red_buffer, int32_t *pn_spo2, int8_t *pch_spo2_valid, 
																									 int32_t *pn_heart_rate, int8_t  *pch_hr_valid);
#endif

	
#ifdef MAX30102_ENABLE
/*
**********************************************************************
* @fun     :mpu_get_max30102_data 
* @brief   :读取max30102数据进行处理计算，得到心率与血氧数据
* @param   :None
* @return  :None 
**********************************************************************
*/
 void mpu_get_max30102_data(void)
{
  int16_t i;
	uch_dummy = 0x00;  
	//dumping the first 100 sets of samples in the memory and shift the last 400 sets of samples to the top
	for(i=100;i<MAX30102_BUFF_LENGTH;i++)
	{
		aun_red_buffer[i-100]=aun_red_buffer[i];
		aun_ir_buffer[i-100]=aun_ir_buffer[i];  
	}       
	//在计算心率前取100组样本
	for(i=100;i<MAX30102_BUFF_LENGTH;i++)
	{
		while((uch_dummy&0xC0) == 0x00) maxim_max30102_read_reg(REG_INTR_STATUS_1,&uch_dummy); 
		maxim_max30102_read_fifo((aun_red_buffer+i), (aun_ir_buffer+i));
		uch_dummy= 0x00;   
		#ifdef DEBUG_MODE	
			//send samples and calculation result to terminal program through UART
			printf("%i,%i\n\r", aun_red_buffer[i],aun_ir_buffer[i]);	
		#endif // DEBUG	
	}
	maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 	
	// Read the _chip_ temperature in degrees Celsius
  int8_t integer_temperature;
  uint8_t fractional_temperature;
	
  maxim_max30102_read_temperature(&integer_temperature, &fractional_temperature);
  n_temperature = integer_temperature + ((float)fractional_temperature)/16.0;
		
	//send samples and calculation result to terminal program through UART
	if(ch_hr_valid || ch_spo2_valid)
	{
		printf(" HR=%d, ", n_heart_rate/4); //采样率100sps,max30102设置4点求平均
		printf("SpO2=%d\n\r", n_sp02);
	}
}	
/*
**********************************************************************
* @fun     :Update_HeartRateInfo
* @brief   :更新脉搏、血氧饱和度、人体温度
* @remark  :心率传感器的I2C总线与触摸共用同一个总线，读取心率数据的时
*           阻塞了整个main循环，因此不进行连续采集，仅上传一次。获取的
*						心率数据可以通过printf打印至PC端，在PC上进行基线漂移的处理
*						与心率波形显示处理。
**********************************************************************
*/
void Update_HeartRateInfo(void)
{
	uint16_t pVariableX = 0;
	maxim_max30102_init();  //initializes the MAX30102
	
	n_ir_buffer_length=MAX30102_BUFF_LENGTH; //buffer length of 100 stores 2 seconds of samples running at 100sps
			
	//读取前200个样本，并确定信号范围
	for(pVariableX=0;pVariableX<n_ir_buffer_length;pVariableX++)
	{	
		while((uch_dummy&0xC0) == 0x00) maxim_max30102_read_reg(REG_INTR_STATUS_1,&uch_dummy); 
		maxim_max30102_read_fifo((aun_red_buffer+pVariableX), (aun_ir_buffer+pVariableX));  //read from MAX30102 FIFO
		uch_dummy= 0x00;
		
			//printf("%i,%i\n\r", aun_red_buffer[pVariableX],aun_ir_buffer[pVariableX]);
			
	}
	//calculate heart rate and SpO2 after first 200 samples (first 2 seconds of samples)
	maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
	//Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
	mpu_get_max30102_data();
	//Reset the MAX30102
	maxim_max30102_reset();
	//测量完成
}
#endif