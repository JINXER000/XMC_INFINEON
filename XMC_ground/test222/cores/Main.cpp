#include "common.h"
#include "Dps310.h"	
#include "timer.h"
#include "driver.h"
#include "BspUsart.h"

#define F_CPU 144000000U

//send_flag
#define ENABLE_NONE 0x20
#define ENABLE_RADAR 0X21
#define ENABLE_BARO  0X22
#define ENABLE_RADAR_BARO 0x23

//uint8_t button_init;
//uint8_t button_state1;
//uint8_t button_state2;


Dps310 Dps310PressureSensor = Dps310();

int32_t temperature_ground;
int32_t temperature_air;
int32_t pressure_ground;
int32_t pressure_air;

static uint8_t send_flag = ENABLE_RADAR_BARO;
static uint8_t flag_enable_delta_h = 0x00; 
static uint8_t flag_set_ref;

 uint8_t button_state ; 

int16_t ret_T;
int16_t ret_P;

volatile  uint16_t baro_alt_air;          //all unit : dm
volatile  uint16_t baro_alt_ground;
volatile  uint8_t  baro_height;
volatile  uint16_t baro_h_t;
volatile  int8_t  baro_h_offset;
volatile  uint8_t  baro_delta_p;
volatile  uint16_t baro_alt_ref;

//extern int32_t temperature_ground;
//extern int32_t temperature_air;
//extern int32_t pressure_ground;
//extern int32_t pressure_air;
//extern int16_t ret_T;
//extern int16_t ret_P;

	/* tropospheric properties (0-11km) for standard atmosphere */
	//const double T1 = 15.0 + 273.15;	/* temperature at base height in Kelvin */
	const double a  = -6.5 / 1000;	/* temperature gradient in degrees per metre */
	const double g  = 9.80665;	/* gravity constant in m/s/s */
	const double R  = 287.05;	/* ideal gas constant in J/kg/K */

	/* current pressure at MSL in kPa */
	uint32_t _msl_pressure= 101325 ; //100564;//101325;
	double p1 = _msl_pressure / 1000.0;

	/* measured pressure in kPa */
	double p ;
	static double T1 = 15+273.15;

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
    ret_T = Dps310PressureSensor.measureTempOnce(temperature_ground, oversampling);
		uint32_t now_time = tick;
		 while(ret_T != 0 || temperature_ground < 0 ||  temperature_ground > 40)
		 {
			ret_T = Dps310PressureSensor.measureTempOnce(temperature_ground, oversampling);
  	  delay_ms(200);
			if(tick > now_time + baro_timeout)
         return 0;
	   }
		  //T1 = temperature_ground + 273.15;
     //int32_t button = Buttons_Initialize();
		 
		 now_time = tick;
		 ret_P = Dps310PressureSensor.measureTempOnce(pressure_ground, oversampling);
		 while(ret_T != 0 || pressure_ground < 100000 ||  pressure_ground > 110000)
		 {
			ret_T = Dps310PressureSensor.measurePressureOnce(pressure_ground, oversampling);
  	  delay_ms(100);
			if(tick > now_time + baro_timeout)	   
			   return 0;
	   }
	   p = pressure_ground / 1000.0;
		 
		 baro_alt_ref = (uint16_t) ((((pow((p / p1), (-(a * R) / g))) * T1) - T1) / a*10);
		 return 1;
}




float   cntt = 0;
float   cntall = 0;
float     q;
	
int main(void)
{
	
	SystickConfig();
  Timer1Config();
  Timer2Config();
	delay_ms(500);
	UartConfig();
	if(Sls10ere_Test_Init()==1)
	{
			Sls10ere_Run_Test();
	}
	Dps310PressureSensor.begin(Wire);

  delay_ms(500);
	
  	XMC_GPIO_SetMode(P5_8, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);//out put
  	XMC_GPIO_SetMode(P5_9, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);//out put
	
  if ((Buttons_Initialize()==0) &&  baro_test())
	{
		
    XMC_GPIO_SetOutputHigh(P5_9);//LED1 on
	}		
	
	
	
	
	  uint16_t   button_timeout = 500;// Ïû¶¶
	  uint32_t   now_t = tick;
	
	    //button_init = PIN_INTERRUPT_Init(&PIN_INTERRUPT_0); 
    while (1)
    {
			
      button_state=Buttons_GetState();
      
		  ret_P = Dps310PressureSensor.measurePressureOnce(pressure_ground, oversampling);
			p = (double) pressure_ground / 1000.0;
			baro_h_t = (uint16_t) ((((pow((p / p1), (-(a * R) / g))) * T1) - T1) / a*10);
			baro_h_offset = baro_h_t - baro_alt_ref;
			
			if( button_state == 1)
			{
				baro_alt_ref = baro_h_t;
			  XMC_GPIO_SetOutputLow(P5_9);
				flag_set_ref = 1;  
			}
			else if(button_state == 2 && (tick -now_t) > button_timeout)
				{
					 now_t = tick;
			
					flag_enable_delta_h = (!flag_enable_delta_h);
					if(flag_enable_delta_h)
					{
						XMC_GPIO_SetOutputHigh(P5_8);//LED1 on
					}else{
					        XMC_GPIO_SetOutputLow(P5_8);
					     }
				}
			else
				{
					if(ret_P == 0)
					XMC_GPIO_SetOutputHigh(P5_9);
					flag_set_ref = 0;  
				}
			
		
		 
      delay_ms(95);
	
   
			if (ret_P == 0)
				cntt++;
			if (++cntall ==20)
				q = cntt/cntall;
			
			if(ret_P == 0)
			{
				if(flag_set_ref)
			    send2xmc_air(3);
				else
					send2xmc_air(flag_enable_delta_h);

			  //UART1_Report_States();
			//if(send_flag != SEND_NONE)
      }
			else
			{
				XMC_GPIO_SetOutputLow(P5_9);
				send2xmc_air(0);
			}
		}
		
		
		
}
//****************************************************************************
//                                 END OF FILE
//****************************************************************************
