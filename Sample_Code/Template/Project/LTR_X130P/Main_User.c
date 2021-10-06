
#include "string.h"

#include "Main_User.h"
#include "I2C_Functions.h"
//#include "MS51_16K.h"
#include "i2c_conf.h"
//Global Variables

unsigned char Slave_Addr = 0xA6 >> 1 ; // 7 bit
int CalibValue = 0;
int AverageCount = 10; 

void HAL_Delay(unsigned int timing)
{
	Timer2_Delay(SYS_CLOCK,16,1,timing*1000);
}

int WriteByte(unsigned char slave_addr , unsigned char register_addr , unsigned char value )
{
	I2Cx_Write_Multi_ToSlave(slave_addr , register_addr , (unsigned char*) &value , 1);
	HAL_Delay(5);	
	return TRUE;
}

unsigned char ReadByte (unsigned char slave_addr , unsigned char register_addr)
{
	unsigned char value = 0;
	I2Cx_Read_Multi_FromSlave(slave_addr , register_addr ,(unsigned char*) &value , 1);
	HAL_Delay(5);
	
	return value;
}


void LTR_X130P_PS_CAN(unsigned int PS_Cancel_Value)
{
	unsigned char Register_Addr = 0;
	unsigned char Command = 0;
	unsigned char Data0 = 0;
	unsigned char Data1 = 0;
//	unsigned char Data2 = 0;	

	unsigned int Upper_Threshold_Value = 0;	
	unsigned int Lower_Threshold_Value = 0;
//	unsigned int PS_Cancel_Value = 0;

	/***** PS_CAN Registers   *****/  
	//The register 0x1F contains PS cancellation lower byte data (PS_CAN_0) 
	//The register 0x20 contains 3 bits of PS cancellation upper byte data (PS_CAN_1) 
 
	//To set PS Cancellation Value (0 to 2047) 
//	PS_Cancel_Value=100;                  		// Example 100 
	Data1 = PS_Cancel_Value >> 8;       			// Shift right to extract the upper byte   
	Data0 = PS_Cancel_Value & 0xFF;      			// Mask to extract lower byte. 
	Register_Addr = PS_CAN_0;        				// PS_CAN_0 Register address 
	WriteByte(Slave_Addr, Register_Addr, Data0); 
	Register_Addr = PS_CAN_1;        				// PS_CAN_1 Register address 
	WriteByte(Slave_Addr, Register_Addr, Data1); 
}

void LTR_X130P_PS_THRES(void)
{
	unsigned char Register_Addr = 0;
	unsigned char Command = 0;
	unsigned char Data0 = 0;
	unsigned char Data1 = 0;
//	unsigned char Data2 = 0;	

	unsigned int Upper_Threshold_Value = 0;	
	unsigned int Lower_Threshold_Value = 0;
	unsigned int PS_Cancel_Value = 0;
	
	/***** PS_THRES Registers   *****/ 
	// The register 0x1B contains PS Interrupt upper threshold lower byte data (PS_THRES_UP_0) 
	// The register 0x1C contains PS Interrupt upper threshold upper byte data (PS_THRES_UP_1) 
	// The register 0x1D contains PS Interrupt lower threshold lower byte data (PS_THRES_LOW_0) 
	// The register 0x1E contains PS Interrupt lower threshold upper byte data (PS_THREH_LOW_1) 
 
	// To set PS Upper threshold for Interrupt 
	Upper_Threshold_Value=1000;                  // Example 1000 
	Data1 = Upper_Threshold_Value >> 8;       	// Shift right to extract the upper byte   
	Data0 = Upper_Threshold_Value & 0xFF;      	// Mask to extract lower byte. 
	Register_Addr = PS_THRES_UP_0;          		// PS_THRES_UP_0 Register address 
	WriteByte(Slave_Addr, Register_Addr, Data0); 
	Register_Addr = PS_THRES_UP_1;          		// PS_THRES_UP_1 Register address 
	WriteByte(Slave_Addr, Register_Addr, Data1); 
	 
	// To set PS Lower threshold for Interrupt 
	Lower_Threshold_Value=100;                	// Example 100   
	Data1 = Lower_Threshold_Value >> 8;       	// Shift right to extract the upper byte   
	Data0 = Lower_Threshold_Value & 0xFF;    		// Mask to extract lower byte. 
	Register_Addr = PS_THRES_LOW_0;          	// PS_THRES_LOW_0 Register address 
	WriteByte(Slave_Addr, Register_Addr, Data0); 
	Register_Addr = PS_THRES_LOW_1;          	// PS_THRES_LOW_1 Register address 
	WriteByte(Slave_Addr, Register_Addr, Data1);
}

void LTR_X130P_INTERRUPT_PERSIST(void)
{
	unsigned char Register_Addr = 0;
	unsigned char Command = 0;

	/***** INTERRUPT_PERSIST Register   *****/   
	// This register sets the PS persist level. 
	// The default setting is 0x00. Interrupt at every PS reading outside set thresholds. 

	Register_Addr = INT_PST;  					// INT_PST register 
	Command = 0x00;              				// Interrupt for every PS value outside threshold 
												// Command =0x10   Subsequent every PS value, outside threshold range 
												// Command =0x11   Subsequent 2 PS values, outside threshold range 
												// Command =0x35    Subsequent 6 PS values, outside threshold range       
	WriteByte(Slave_Addr, Register_Addr, Command);

}

void LTR_X130P_INT_CFG(void)
{
	unsigned char Register_Addr = 0;
	unsigned char Command = 0;

	/***** INT_CFG Register   *****/  
	//This register controls the operation of the interrupt pins and options to trigger interrupt for PS. 
	//The default value for this INT_CFG register is 0x10 (Interrupts inactive for both PS) 
	 
	Register_Addr = INT_CFG;  					// INT_CFG Register address 
	Command = 0x15;       						// Interrupt Enable 
	          									// Command = 0x17     
	          									//        PS Logic Output Mode; PS Interrupt Enable 
												// Command = 0x05       
												//        Normal Trigger Mode; PS Interrupt Enable 
												// Command = 0x14       
												//        Normal Trigger Mode; PS Interrupt Disable 
												// Command = 0x11       
												//        Normal Trigger Mode; PS Interrupt Enable 
 
	WriteByte(Slave_Addr, Register_Addr, Command);
}

void LTR_X130P_CLEAR_DATA(void)
{
	unsigned char Register_Addr = 0;
	unsigned char Command = 0;
	
	unsigned char Data0 = 0;
	unsigned char Data1 = 0;
	unsigned char Data2 = 0;	
	unsigned long CLEAR_Data = 0;

	/***** CLEAR_DATA Registers (Read Only)   *****/  
	//The register 0x0A contains CLEAR_DATA    ADC 0 lower byte data. 
	//The register 0x0B contains CLEAR_DATA    ADC 1 middle byte data. 
	//The register 0x0C contains CLEAR_DATA    ADC 2 upper byte data. 
	//These registers should be read as a group, with the lower address being read first. 
 
	Register_Addr = 0x0A;        					// CLEAR_DATA_0 low byte address 
	Data0 = ReadByte(Slave_Addr, Register_Addr); 
	Register_Addr = 0x0B;        					// CLEAR_DATA_1 middle byte address 
	Data1 = ReadByte(Slave_Addr, Register_Addr); 
	Register_Addr = 0x0C;        					// CLEAR_DATA_2 upper byte address 
	Data2 = ReadByte(Slave_Addr, Register_Addr); 
	CLEAR_Data =(Data2<<16)| (Data1 << 8) | Data0;   
	                							// Shift and combine all register data to get CLEAR ADC Data 

}

unsigned int LTR_X130P_PS_DATA(void)
{
	unsigned char Register_Addr = 0;
//	unsigned char Command = 0;
	unsigned char Value = 0;
	unsigned char Data0 = 0;
	unsigned char Data1 = 0;

	unsigned int PS_ADC_Data = 0;	
	unsigned char Overflow_status = 0;
	
	/***** PS_DATA Registers (Read Only)   *****/ 
	//The register 0x08 contains PS ADC lower byte data. 
	//The register 0x09 contains PS ADC 3 bits of upper byte data and PS Overflow flag 
	//These registers should be read as a group, with the lower address being read first. 
 
	Register_Addr = PS_DATA_0;                	// PS_DATA0 low byte address 
	Data0 = ReadByte(Slave_Addr, Register_Addr); 	// Data= PS ADC lower byte data 

	Register_Addr = PS_DATA_1;              		// PS_DATA1 high byte address 
	Value = ReadByte(Slave_Addr, Register_Addr); 	// Data= PS ADC high byte data 
	//Data1 = Value & 0x03;                   		// Mask with 0x03 to extract data 
	Data1 = Value & 0x07;
	                                   
	PS_ADC_Data = (Data1 << 8) | Data0;   		// Shift and combine lower and upper bytes to give 11-bit PS data 
//	Overflow_status = Value & 0x08;              // If 0x08 PS Data is overflow 
	            								// If 0x00 PS Data is valid 

//	printf("PS_DATA_0 : 0x%2BX : 0x%2BX \r\n" , PS_DATA_0 , Data0);	
//	printf("PS_DATA_0 : 0x%2BX : 0x%2BX \r\n" , PS_DATA_1 , Value);	
//	printf("PS_ADC_Data : 0x%2BX \r\n" , PS_ADC_Data);	
//	printf("Overflow_status : 0x%2BX \r\n" , Overflow_status);	

	printf("DATA_0 : 0x%2BX ,DATA_1 : 0x%2BX , combine : %4d (overflow : 0x%2BX)\r\n" , Data0,Value , PS_ADC_Data , Overflow_status);	

	return PS_ADC_Data;												
}


void LTR_X130P_MAIN_STATUS(void)
{
	unsigned char Register_Addr = 0;
	unsigned char Command = 0;

	unsigned char Value = 0;	
	unsigned char Power_ON_Status = 0;
	unsigned char PS_Logic_Signal_Status = 0;
	unsigned char PS_Interrupt_Status = 0;
	unsigned char PS_Data_Status = 0;	

	/***** MAIN_STATUS Register (Read Only)   *****/ 
	// This Register contains the information on Interrupt, PS data status. 
           
	Register_Addr = MAIN_STATUS;        			// MAIN_STATUS register address 
	Value = ReadByte(Slave_Addr, Register_Addr); 
	 
	Power_ON_Status = Value & 0x20;        		// If 0x20 Part went through power-up event   
	                							// If 0x00 Normal 
	PS_Logic_Signal_Status = Value & 0x04;      	// If 0x04 Object is near 
	                							// If 0x00 Object is far 
	PS_Interrupt_Status = Value & 0x02;        		// If 0x02 Interrupt triggered   
	                							// If 0x00 Interrupt condition not fulfilled 
	PS_Data_Status = Value & 0x01;                // If 0x01 PS data is new 
	                							// If 0x00 Old (previously read) Data 
	                							
//	printf("MAIN_STATUS : 0x%2BX : 0x%2BX \r\n" , Register_Addr , Value);	
//	printf("Power_ON_Status : 0x%2BX \r\n" , Power_ON_Status);	
//	printf("PS_Logic_Signal_Status : 0x%2BX \r\n" , PS_Logic_Signal_Status);
//	printf("PS_Interrupt_Status : 0x%2BX \r\n" , PS_Interrupt_Status);
//	printf("PS_Data_Status : 0x%2BX \r\n" , PS_Data_Status);	

	printf("MAIN_STATUS : 0x%2BX ,Value: 0x%2BX,Power_ON:0x%2BX,Logic_Signal:0x%2BX,Interrupt:0x%2BX,Data:0x%2BX\r\n" , 
			Register_Addr , 
			Value , 
			Power_ON_Status , 
			PS_Logic_Signal_Status , 
			PS_Interrupt_Status ,
			PS_Data_Status);	


}

void LTR_X130P_PS_MEAS_RATE(void)
{
	unsigned char Register_Addr = 0;
	unsigned char Command = 0;
	
	/***** PS_MEAS_RATE Register   *****/
	//This controls the PS Resolution and measurement rate. 
	// Default setting of the register is 0x45 (PS Resolution = 8 bit, Measurement Rate = 100ms) 

	/*
		bit 7:5 : 0x010
		
		target : 11 bit , quick measurement rate
		bit 4:3 : 0x11 , 11bit
		bit 2:0 : 0x001 , 6.25ms
	*/
	Register_Addr = PS_MEAS_RATE;        		// PS_MEAS_RATE register 
	Command = 0x5C;          					//          Resolution = 8 bit, Meas Rate = 100ms 
	                							// Command = 0x47  Resolution = 8 bit Meas Rate = 400ms 
	                							// Command = 0x5D    Resolution = 11 bit Meas Rate = 100ms 
	WriteByte(Slave_Addr, Register_Addr, Command); 
}

void LTR_X130P_PS_PULSES(void)
{
	unsigned char Register_Addr = 0;
	unsigned char Command = 0;

	/***** PS_PULSES Register  *****/
	// This controls the number of PS LED pulses emitted. 
	// Default setting of the register is 0x08 (8 Pulses) 
 
	Register_Addr = PS_PULSES;        			// PS_PULSES register 
	Command = 0x08;          					//        8 pulses 
	                							// Command = 0x00  0 (no light) 
	                							// Command = 0x04  4 pulses 
	                							// Command = 0x20  32 pulses 
	WriteByte(Slave_Addr, Register_Addr, Command); 
}


void LTR_X130P_PS_LED(void)
{
	unsigned char Register_Addr = 0;
	unsigned char Command = 0;

	/***** PS_LED Register  *****/
	// This defines the LED pulse modulation frequency and Peak current. 
	// Default setting is 0x36 (Pulse Freq = 60kHz, peak current = 100mA). 

	/*
		bit 7 : 0x0
		bit 3 : 0x0		
		
		target : LED pulse modulation Frequency  , LED Current 
		bit 4:6 : 0x111 , LED pulse period = 100kHz 
		bit 2:0 : 0x111 , LED pulsed current level = 125mA   		
	*/
	Register_Addr = PS_LED;        				// PS_LED register 
	Command = 0x37;            				//          Pulse Freq = 60kHz, Peak Current =100mA   
	                            				// Command = 0x46  Pulse Freq = 70kHz, Peak Current =100mA   
                								// Command = 0x44  Pulse Freq = 70kHz, Peak Current = 50mA 
                								// Command = 0x76  Pulse Freq = 100kHz, Peak Current = 100mA 
                								// Command = 0x77  Pulse Freq = 100kHz, Peak Current = 125mA 
	WriteByte(Slave_Addr, Register_Addr, Command);

}

void LTR_X130P_MAIN_CTRL(void)
{
	unsigned char Register_Addr = 0;
	unsigned char Command = 0;
	
	/***** MAIN_CTRL Register *****/
	// This defines the operating modes of the PS 
	// Default settings is 0x00 (PS standby) 
	 
	Register_Addr = MAIN_CTRL;        			// MAIN_CTRL register 
	Command = 0x01;          					// PS in Active Mode,   
	                   
	WriteByte(Slave_Addr, Register_Addr, Command); 
}

void LTR_X130P_PART_ID (void)
{
	unsigned char Register_Addr = 0;
	unsigned char Command = 0;

	unsigned char Value = 0;	

	/***** MAIN_STATUS Register (Read Only)   *****/ 
	// This Register contains the information on Interrupt, PS data status. 
           
	Register_Addr = PART_ID ;        			// MAIN_STATUS register address 
	Value = ReadByte(Slave_Addr, Register_Addr); 
		                							
	printf("PART_ID : 0x%2BX : 0x%2BX \r\n" , Register_Addr , Value);	
}

void LTR_X130P_SW_Reset(void)
{
	unsigned char Register_Addr = 0;
	unsigned char Command = 0;
	
	/***** MAIN_CTRL Register *****/
	// This defines the operating modes of the PS 
	// Default settings is 0x00 (PS standby) 
	 
	Register_Addr = MAIN_CTRL;        			// MAIN_CTRL register 
	Command = 0x10;          					// bit 4 , 0 : Software reset is NOT triggered (default)   
												// 1 : Software reset is triggered 
	                   
	WriteByte(Slave_Addr, Register_Addr, Command); 
}

unsigned int LTR_X130P_PS_DATA_Simple(void)
{
	unsigned char Register_Addr = 0;
	unsigned char Value_MAIN_STATUS = 0;
	
	unsigned char Value = 0;
	unsigned char Data0 = 0;
	unsigned char Data1 = 0;
	unsigned int PS_ADC_Data = 0;	

	const unsigned char error_handle = 0xA7;

	static unsigned char zero_cnt = 0;	
	static unsigned char backup_Value = 0;
	static unsigned char backup_Data0 = 0;	
	static unsigned long int avg = 0;
	static unsigned char flag = 1;
	
	Register_Addr = MAIN_STATUS;        			// MAIN_STATUS register address 
	Value_MAIN_STATUS = ReadByte(Slave_Addr, Register_Addr); 
//		HAL_Delay(40);

	Register_Addr = PS_DATA_0;                	// PS_DATA0 low byte address 
	Data0 = ReadByte(Slave_Addr, Register_Addr); 	// Data= PS ADC lower byte data 
//		HAL_Delay(40);

	Register_Addr = PS_DATA_1;              		// PS_DATA1 high byte address 
	Value = ReadByte(Slave_Addr, Register_Addr); 	// Data= PS ADC high byte data 
//		HAL_Delay(40);		

	// issue : sometimes value is 0xA7 
	if (Value == error_handle)
	{
		Value = backup_Value;
		LTR_X130P_CLEAR_DATA();
//		printf("**\r\n");		// debug	
	}
	else
	{
		// issue : sometimes value is 0x00 
		if ( (Data0 == 0x00) && (Value == 0x00) )
		{
			if (zero_cnt >= 2)	// real 0
			{
				zero_cnt = 0;
				Data0 = 0;
//				printf(">>>\r\n");	// debug					
			}	
			else
			{
				Data0 = backup_Data0;
			}

			zero_cnt++;
		}
		else
		{	
			backup_Value = Value;
			backup_Data0 = Data0;		
		}
	}

	Data1 = Value & 0x07;

	PS_ADC_Data = (Data1 << 8) | Data0;   		// Shift and combine lower and upper bytes to give 11-bit PS data 

	// do simple average	
	if (flag)
	{
		flag = 0;
		avg = PS_ADC_Data;
	}
	avg += PS_ADC_Data;
	avg = avg >> 1;
	
	printf("DATA_0 : 0x%2BX ,DATA_1 : 0x%2BX ,Value : 0x%2BX, combine : %5u  \r\n" , Data0,Data1,Value, (unsigned int)PS_ADC_Data);	


	return PS_ADC_Data;												
}

void INIT_LTR_X130P(void)
{
	int i = 0;

	LTR_X130P_SW_Reset();
	HAL_Delay(50);
	
	LTR_X130P_MAIN_CTRL();

	LTR_X130P_PS_LED();

	LTR_X130P_PS_PULSES();

	LTR_X130P_PS_MEAS_RATE();

//	LTR_X130P_MAIN_STATUS();

//	LTR_X130P_PS_DATA();

//	LTR_X130P_CLEAR_DATA();

	LTR_X130P_INT_CFG();

	LTR_X130P_INTERRUPT_PERSIST();

	LTR_X130P_PS_THRES();

	#if 1
	for(i = 0 ; i < AverageCount ; i++)
	{
		CalibValue += LTR_X130P_PS_DATA();
		//Delay of 10 ms needs to be changed depending on the API of the µ-controller of use
//		HAL_Delay(10);
	}

	//Calculate the average of the offset measurement
	CalibValue /= AverageCount;

	//Set Cancellation register to eliminate offset
	LTR_X130P_PS_CAN(CalibValue);

	#endif
	
}

//Print Function
void PRINT_LTR_X130P(void)
{

//	LTR_X130P_MAIN_STATUS();

//	LTR_X130P_PS_DATA();

	LTR_X130P_PS_DATA_Simple();

//	LTR_X130P_PART_ID();

}


