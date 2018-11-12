#include "common.h"
#include "Dps310.h"	
#include "timer.h"
#include "driver.h"
#include "BspUsart.h"

#define F_CPU 144000000U

//send_flag
//#define ENABLE_NONE 0x20
//#define ENABLE_RADAR 0X21
//#define ENABLE_BARO  0X22
//#define ENABLE_RADAR_BARO 0x23

uint8_t button_init;
uint8_t button_state1;
uint8_t button_state2;


Dps310 Dps310PressureSensor = Dps310();

int32_t temperature_ground;
int32_t temperature_air;
int32_t pressure_ground;
int32_t pressure_air;

volatile uint8_t send_flag = 0;


int16_t ret_T;
int16_t ret_P;

volatile  int16_t baro_alt_air;          //all unit : dm
volatile  int16_t baro_alt_ground;
volatile  int8_t  baro_height;

volatile  int16_t baro_alt_air_ref;


volatile  int8_t  baro_h_offset;   //decided by ground
volatile  int8_t  baro_delta_h;
volatile  uint8_t  baro_delta_p;

//extern int32_t temperature_ground;
//extern int32_t temperature_air;
//extern int32_t pressure_ground;
//extern int32_t pressure_air;
//extern int16_t ret_T;
//extern int16_t ret_P;

	/* tropospheric properties (0-11km) for standard atmosphere */
	//const double T1 = 35.0 + 273.15;	/* temperature at base height in Kelvin */
	const double a  = -6.5 / 1000;	/* temperature gradient in degrees per metre */
	const double g  = 9.80665;	/* gravity constant in m/s/s */
	const double R  = 287.05;	/* ideal gas constant in J/kg/K */

	/* current pressure at MSL in kPa */
	uint32_t _msl_pressure= 100564;//101325;
	double p1 = _msl_pressure / 1000.0;

	/* measured pressure in kPa */
	double p ;
	static double T1;

	/*
	 * Solve:
	 *
	 *     /        -(aR / g)     \
	 *    | (p / p1)          . T1 | - T1
	 *     \                      /
	 * h = -------------------------------  + h1
	 *                   a
	 */
	

 int16_t oversampling = 6;

uint8_t baro_test(void)
{
	 uint16_t baro_timeout = 5000; 
    ret_T = Dps310PressureSensor.measureTempOnce(temperature_air, oversampling);
		uint32_t now_time = tick;
		 while(ret_T != 0 || temperature_air < 0 ||  temperature_air > 40)
		 {
			ret_T = Dps310PressureSensor.measureTempOnce(temperature_air, oversampling);
  	  delay_ms(200);
			if(tick > now_time + baro_timeout)
         return 0;
	   }
		  T1 = temperature_air + 273.15;
     //int32_t button = Buttons_Initialize();
		 
		 now_time = tick;
		 ret_P = Dps310PressureSensor.measureTempOnce(pressure_air, oversampling);
		 while(ret_T != 0 || pressure_air < 100000 ||  pressure_air > 110000)
		 {
			ret_T = Dps310PressureSensor.measurePressureOnce(pressure_air, oversampling);
  	  delay_ms(100);
			if(tick > now_time + baro_timeout)	   
			   return 0;
	   }
	   p1 = pressure_air / 1000.0;
		 return 1;
}



int main(void)
{
	
	
	SystickConfig();
  Timer1Config();
  Timer2Config();
	delay_ms(1000);    //wait for stm32-N3 complete
	UartConfig();
	delay_ms(1000);
	if(Sls10ere_Test_Init()==1)
	{
			if(Sls10ere_Run_Test() == 0)
			   send_flag = 3;
	}
	else 
		  send_flag = 3;
  
	
	//安全认证不通过——
	if (send_flag == 3)
	{ 
		 
		 XMC_GPIO_SetMode(P5_8, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);//out put
		 while(1)
		 {
			  XMC_GPIO_SetOutputHigh(P5_8);//LED on 
			  delay_ms(500);
			  XMC_GPIO_SetOutputLow(P5_8);
			  delay_ms(500);
			 send2stm(send_flag);
		 }
	}
	
	
	
	Dps310PressureSensor.begin(Wire);
  
  delay_ms(500);
	
  if (baro_test())
	{
		XMC_GPIO_SetMode(P5_9, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);//out put
    XMC_GPIO_SetOutputHigh(P5_9);//LED1 on
	}		
	 
	    //button_init = PIN_INTERRUPT_Init(&PIN_INTERRUPT_0); 
    while (1)
    {
			if(counter_110ms >= 110 )
			{      
				counter_110ms = 0;
				ret_P = Dps310PressureSensor.measurePressureOnce(pressure_air, oversampling);
			  if(ret_P == 0)
			  {
				  //UART1_Report_States();
          p = (double) pressure_air / 1000.0;
		      baro_alt_air = (int16_t) ((((pow((p / p1), (-(a * R) / g))) * T1) - T1) / a*10);			     
				}
			   baro_delta_h = baro_alt_air_ref + baro_h_offset - baro_alt_air;
		  }
			 
			if(counter_20ms >= 20)
			{
				counter_20ms = 0;
				send2stm(send_flag);
			
			}
			//if(send_flag != SEND_NONE)
			
    }
		

		
}
//****************************************************************************
//                                 END OF FILE
//****************************************************************************
