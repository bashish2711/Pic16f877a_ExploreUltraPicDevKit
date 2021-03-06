/***************************************************************************************************
                                   ExploreEmbedded
****************************************************************************************************
 * File:   main.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: File contains the code to test the ULTRA PIC board.
                Connect the board to system using USB-to-Serial. 
                Reset the board after which the menu will be displayed, select the required option a follow the steps.

This code has been developed and tested on ExploreEmbedded boards.  
We strongly believe that the library works on any of development boards for respective controllers. 
Check this link http://www.exploreembedded.com/wiki for awesome tutorials on 8051,PIC,AVR,ARM,Robotics,RTOS,IOT.
ExploreEmbedded invests substantial time and effort developing open source HW and SW tools, to support consider buying the ExploreEmbedded boards.
 
The ExploreEmbedded libraries and examples are licensed under the terms of the new-bsd license(two-clause bsd license).
See also: http://www.opensource.org/licenses/bsd-license.php

EXPLOREEMBEDDED DISCLAIMS ANY KIND OF HARDWARE FAILURE RESULTING OUT OF USAGE OF LIBRARIES, DIRECTLY OR
INDIRECTLY. FILES MAY BE SUBJECT TO CHANGE WITHOUT PRIOR NOTICE. THE REVISION HISTORY CONTAINS THE INFORMATION 
RELATED TO UPDATES.
 

Permission to use, copy, modify, and distribute this software and its documentation for any purpose
and without fee is hereby granted, provided that this copyright notices appear in all copies 
and that both those copyright notices and this permission notice appear in supporting documentation.
**************************************************************************************************/


#include "uart.h"
#include  "rtc.h" 
#include  "i2c.h"
#include  "lcd.h"
#include "delay.h"
#include "keypad.h"
#include "eeprom.h"
#include "gpio.h"
#include "adc.h"

void gpio_test(void);
void LCD_8bit_test(void);
void LCD_4bit_test(void);
void seg_test(void);
void rtc_test(void);
void adc_test(void);
void keypad_test(void);
void eeprom_test(void);


#define LedOn    0xFF
#define LedOff   0x00




/***************************************************************
               Seven Segment Connection
 **************************************************************/
#define SegmentValue     PORTD //PORTD Segment DataBus
#define SegValueDirnReg  TRISD

#define SegmentSlection  PORTB //PB.0-PB.3  selection lines
#define SegSelectDirnReg TRISB

#define SegOne   0x01
#define SegTwo   0x02
#define SegThree 0x04
#define SegFour  0x08
/*************************************************************/




char mm_option;

/* start the main program */
void main() 
{
   UART_Init(9600);
   UART_TxString("\n\rPIC Ultra baord menu v2.0\r\n 1:GPIO Blink\r\n 2:LCD 8-bit \n\r 3:LCD 4-bit\n\r 4:7-Segment\n\r 5:RTC\n\r 6:EEPROM\n\r 7:ADC\n\r 8:Keypad \n\r Enter option:");
   UART_TxString("\n\rReset the board after test is done");
   mm_option = UART_RxChar();
   while(1)
    {
 	  	switch(mm_option)
		{
		 case '1': gpio_test(); break;
		 case '2': LCD_8bit_test(); break;
		 case '3': LCD_4bit_test(); break;
		 case '4': seg_test(); break;
		 case '5': rtc_test(); break;
		 case '6': eeprom_test(); break; //eeprom
		 case '7': adc_test(); break;
		 case '8': keypad_test();break;
		 default:break;
		}
	}
}


/******************************************************GPIO TEST*************************************
   Turns ON and OFF all the ports of the 8051 micrcontroller. With the help of this you can  
 *****************************************************GPIO TEST**************************************/

void gpio_test()
{ 
   unsigned char i;
   UART_Printf("\n\rConnect any IO Pins to buzzer, relays, leds ");
   UART_Printf("\n\rMake connections and hit 'k' to test ");
   while(UART_RxChar()!='k');
   ADCON0 = 0x00;
   ADCON1 = 0x07;
   TRISA = C_PortOutput_U8;
   TRISB = C_PortOutput_U8;
   TRISC = C_PortOutput_U8;
   TRISD = C_PortOutput_U8;
   TRISE = C_PortOutput_U8;
  while(1)
    {
      for(i=0;i<2;i++)
      {
	 /* Turn On all the leds and wait for one second */ 
	   PORTA = 0xff; 
       PORTB = 0xff;
       PORTC = 0xff;
       PORTD = 0xff;
       PORTE = 0xff;
	   DELAY_ms(500);
       
	 /* Turn off all the leds and wait for one second */
	   PORTA = 0x00; 
       PORTB = 0x00;
       PORTC = 0x00;
       PORTD = 0x00;
       PORTE = 0x00;
	   DELAY_ms(500);
      }
      
      for(i=1;i;i=i<<1)
      {
       PORTA = i; 
       PORTB = i;
       PORTC = i;
       PORTD = i;
       PORTE = i;
	   DELAY_ms(100);
      }
      
      for(i=0x80;i;i=i>>1)
      {
       PORTA = i; 
       PORTB = i;
       PORTC = i;
       PORTD = i;
       PORTE = i;
	   DELAY_ms(100);
      }
    }
}

/******************************************************LCD TEST*************************************
					On board LCD TEST in 8 bit mode.
*****************************************************LCD TEST**************************************/

void LCD_8bit_test()
{     
      UART_TxString("\n\r LCD DataBus: PORTD   RS-PB.0 RW-PB.1 EN-PB.2 ");
	  UART_Printf("\n\r Make connections and hit 'k' to test ");
	  while(UART_RxChar()!='k');
	  LCD_SetUp(PB_0,PB_1,PB_2,PD_0,PD_1,PD_2,PD_3,PD_4,PD_5,PD_6,PD_7);
	  LCD_Init(2,16);
	  DELAY_ms(100);
	  LCD_DisplayString("Explore Embedded");
	  LCD_DisplayString("Lcd 8-bit Mode");
	  while(1);
}



void LCD_4bit_test()
{
     UART_TxString("\n\r LCD DataBus:(PD4-PD7)  RS-PB.0  RW-PB.1  EN-PB.2 ");
      UART_Printf("\n\r Make connections and hit 'k' to test ");
      while(UART_RxChar()!='k');
      LCD_SetUp(PB_0,PB_1,PB_2,P_NC,P_NC,P_NC,P_NC,PD_4,PD_5,PD_6,PD_7);
      LCD_Init(2,16);
      DELAY_ms(100);
      LCD_DisplayString("Explore Embedded");
      LCD_DisplayString("Lcd 4-bit Mode");
      while(1);
}


/*************************************************SEVEN SEGMENT*************************************
 				Displays numbers 1 2 3 4 on the on-board segments 
*********************************************************TEST**************************************/

void seg_test()
{  
 unsigned char seg_code[]={0xC0,0xF9,0xA4,0xB0}; 
 UART_TxString("\n\r Segment DataBus: PORTD    Seg select: S1->PB.0 S2->PB.1 S3->PB.2 S4->PB.3");
 UART_Printf("\n\rMake connections and hit 'k' to test! ");
 while(UART_RxChar()!='k');
 SegValueDirnReg = C_PortOutput_U8;
 SegSelectDirnReg = C_PortOutput_U8;

	while(1)
	{  
	   SegmentSlection=SegOne;
	   SegmentValue = seg_code[0];
	   DELAY_us(10); 
	   SegmentSlection=SegTwo;
	   SegmentValue = seg_code[1];
	   DELAY_us(10);
	   SegmentSlection=SegThree;
	   SegmentValue = seg_code[2];
	   DELAY_us(10);	
	   SegmentSlection=SegFour;  
	   SegmentValue = seg_code[3];
	   DELAY_us(10);
	 }  
}

/***************************************************RTC DS1307*************************************
 				Displays time on UART, reading from the RTC
*********************************************************TEST**************************************/

void rtc_test()
{
    rtc_t rtc;
    UART_Printf("\n\rConnections SCL->PC.3 SDA->PC.4");
    UART_Printf("\n\r Make connections and hit 'k' to test! ");
    while(UART_RxChar()!='k');   

    RTC_Init();
    rtc.hour = 0x10; //  10:40:20 am
    rtc.min =  0x40;
    rtc.sec =  0x00;

    rtc.date = 0x01; //1st Jan 2016
    rtc.month = 0x01;
    rtc.year = 0x16;
    rtc.weekDay = 5; // Friday: 5th day of week considering monday as first day.




    /*##### Set the time and Date only once. Once the Time and Date is set, comment these lines
         and reflash the code. Else the time will be set every time the controller is reset*/
    RTC_SetDateTime(&rtc);  //  10:40:20 am, 1st Jan 2016


    /* Display the Time and Date continuously */
    while(1)
    {
        RTC_GetDateTime(&rtc);
        UART_Printf("\n\rtime:%2x:%2x:%2x  \nDate:%2x/%2x/%2x",(uint16_t)rtc.hour,(uint16_t)rtc.min,(uint16_t)rtc.sec,(uint16_t)rtc.date,(uint16_t)rtc.month,(uint16_t)rtc.year);
    }
}

/***************************************************EEPROM*****************************************
 				Writes and Reads a character to and from EEPROM
*****************************************************TEST*****************************************/
void eeprom_test()
{	 
    unsigned char eeprom_address = 0x00, write_char = 'X', read_char;

    for(write_char='A';write_char<='Z';write_char++) /* Eeprom Write and read A-Z */
    {
        UART_Printf("\n\rEeprom Write: %c    ",write_char); //Print the message on UART
        EEPROM_WriteByte(eeprom_address, write_char); // Write the data at memoryLocation	0x00


        read_char = EEPROM_ReadByte(eeprom_address);  // Read the data from memoryLocation 0x00
        UART_Printf("Eeprom Read: %c",read_char); //Print the message on UART
    }
    
    while (1);
}

/***************************************************ADC *****************************************
 				Reads and displays ADC data from on board sensors.
*****************************************************TEST*****************************************/
void adc_test()
{ 
 uint16_t temp,light,pot;
 ADC_Init();

 while(1)
 {
   temp = ADC_GetAdcValue(0)/2;
   pot = ADC_GetAdcValue(1);
   light = ADC_GetAdcValue(2);
   UART_Printf("\n\rtemp:%3d pot:%4d light:%4d", temp,pot,light);

 }
}


/***************************************************KEYPAD *****************************************
 				Displays keys pressed on 4x4 keypad on the uart
*****************************************************TEST*******************************************/
void keypad_test()
{
uint8_t key;
UART_Printf("\n\rConnections: PORTD");
UART_Printf("\n\rMake connections and hit 'k' to test!");
while(UART_RxChar()!='k');
KEYPAD_Init(PD_0,PD_1,PD_2,PD_3,PD_4,PD_5,PD_6,PD_7);
while(1)
{
  key = KEYPAD_GetKey();
  UART_Printf("\n\r Key:%c", key);
}
}














