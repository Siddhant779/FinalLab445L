// ----------------------------------------------------------------
// File name:     esp8266.c
// Description:   This code is used to bridge the TM4C123 board 
//                and the MQTT Web Application via the ESP8266. 
//                It does not use the preferred FIFO interface.
//                You will need to convert it to the FIFO interface.
//
// Author:        Mark McDermott
// Date:          June 21, 2023
// Updated:       July 19, 2023
//
// -----------------------------------------------------------------

#include <stdio.h>
#include <stdint.h>
#include <string.h>			
#include "inc/tm4c123gh6pm.h"

#include "esp8266_base.h"
#include "UART.h"
#include "UART5.h"
#include "esp8266.h"
#include "./inc/Timer5A.h"

#define DEBUG1                // First level of Debug
#define DEBUG2                // Second level of Debug
#define DEBUG3                // Third level of Debug

//#undef DEBUG1               // Comment out to enable DEBUG1
#undef DEBUG2               // Comment out to enable DEBUG2
#undef DEBUG3               // Comment out to enable DEBUG3


#define UART5_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART5_FR_RXFE            0x00000010  // UART Receive FIFO Empty
#define UART5_LCRH_WLEN_8        0x00000060  // 8 bit word length
#define UART5_LCRH_FEN           0x00000010  // UART Enable FIFOs
#define UART5_CTL_UARTEN         0x00000001  // UART Enable

#define PE0   (*((volatile uint32_t *)0x40024004))    // RDY from ESP 8266
#define PE1   (*((volatile uint32_t *)0x40024008))    // RST_B to ESP8266
#define PE2   (*((volatile uint32_t *)0x40024010))    // ACK to ESP8266
#define PE3   (*((volatile uint32_t *)0x40024020))    // LED to indicate that the 8266 connected

#define PF0   (*((volatile uint32_t *) 0x40025004))

#define RDY   PE0
#define RDY1  0x1


#define LOW   0x0

#define BIT0  0x1
#define BIT1  0x2
#define BIT2  0x4
#define BIT3  0x8   

//#define SKIP_SETUP 0

char    ssid[32]          = "prithvi";   // Your WiFi Access Point name goes here
char    pass[32]          = "ece445ltpod";			 			// Your WiFi Access Point password goes here
char		host[32]					= "172.20.10.2";					// Your remote server host IP goes here
char		port[16]					= "8080";								// Your remote server host port goes here

char    inchar;   

static uint32_t         bufpos          = 0;
static uint32_t         parse           = 0;

char                    input_char;
char                    serial_buf[BUFFER_SIZE];
char										ESP2B[BUFFER_SIZE];

void DelayWait1ms(uint32_t n) {
    uint32_t volatile time;
    while (n){
        time = 72724 * 2 / 91;  // ~1msec
        while (time){
            --time;
        }
        --n;
    }
}

void Reset_8266(void) 
{
  PE1 = LOW;              // Reset the 8266
  DelayWait1ms(5000);     // Wait for 8266 to reset
  PE1 = BIT1;             // Enable the 8266
  DelayWait1ms(5000);     // Wait before setting up 8266
}

// ----------------------------------------------------------------------
// This routine sets up the Wifi connection between the TM4C and the
// hotspot. Enable the DEBUG flags if you want to watch the transactions.
// 

void SetupWiFi(void) 
{   
  UART_OutString("Waiting for ESP RDY.");
  while (!RDY) {      // Wait for ESP8266 indicate it is ready for programming data
    UART_OutChar('.');
		DelayWait1ms(5000);
  }
	
  UART5_OutString(ssid);          // Send WiFi SSID to ESP8266
  UART5_OutChar(',');
  
  UART5_OutString(pass);          // Send WiFi Password to ESP8266
  UART5_OutChar(',');      
	
	UART5_OutString(host);					// Send remote server host IP
	UART5_OutChar(',');
	
	UART5_OutString(port);					// Send remot server host port
  UART5_OutChar(',');
	
  UART5_OutChar('\n');            // Send NL to indicate EOT   
  
    // #ifdef DEBUG2
    
    // UART_OutChar('\n');  
    // UART_OutString(ssid);
    // UART_OutChar(',');
    // UART_OutString(pass);
    // UART_OutChar(',');
    // UART_OutChar('\n');      
    // #endif
    
//	 while ((RDY) | ((UART5_FR_R & UART5_FR_RXFE) ==	0)) { 
//		 if ((UART5_FR_R & UART5_FR_RXFE) ==0 ){
//				 inchar =(UART5_DR_R & 0xFF);      
//			#ifdef DEBUG1
//				 UART_OutChar(inchar);                        // Output received character
//			#endif
//		 } else {
//			//ST7735_DrawString(0,8,"waiting", ST7735_Color565(200, 0, 0 ));
//			continue;
//		}
//	}
  
  #ifdef DEBUG1
    UART_OutString("Exiting WiFI_Setup routine\r\n");
  #endif
	Timer5A_Init(&ESP2TM4C, 400000, 2);
}



Message ParseMsg(void) {
	Message msg;
	uint8_t str_len = strlen(serial_buf);
	strncpy(msg.msg, ESP2B, BUFFER_SIZE - 1);
	msg.msg[str_len] = '\0';
	msg.owner = 1;
	return msg;
}

void ESP2TM4C(void) {
	parse = 0x0;
    
  if ((UART5_FR_R & UART5_FR_RXFE) ==0 ) {  // Check to see if a there is data in the RXD buffer
    input_char =(UART5_DR_R & 0xFF);        // Read the data from the UART
        
      if (input_char != '\n')               // If the incoming character is not a newline then process byte
      {                                     
        serial_buf[bufpos] = input_char;    // The buffer position in the array get assigned to the current read
        bufpos++;                           // Once that has happend the buffer advances,doing this over and over
                                            // again until the end of package marker is read.
        UART_OutChar(input_char);        		// Debug only
      }
      else if (input_char == '\n')
      {
        if (bufpos  > 0) {
          strcpy(ESP2B, serial_buf);
          parse = 0x1;                      // Parse incoming data
        }
        bufpos = 0;                         // Reset for next string
      } 
      
    else if ((UART5_FR_R & UART5_FR_RXFE) !=0 ) {     // No new data in the RXD buffer -> Exit routine
    }  
  } 
  
  if (parse == 0x1) ParseMsg();                         // Call the parser if new Command received
}

void sendMessage(Message m) {
	UART5_OutString(m.msg);
	UART5_OutChar('\n');
}

void setSSID(char new_ssid[], uint8_t str_len) {
	strncpy(ssid, new_ssid, 31);
	ssid[str_len] = '\0';									// Ensure null termination
}

void setPass(char new_pass[], uint8_t str_len) {
	strncpy(pass, new_pass, 31);
	pass[str_len] = '\0';									// Ensure null termination
}

void setHost(char new_host[], uint8_t str_len) {
	strncpy(ssid, new_host, 31);
	ssid[str_len] = '\0';									// Ensure null termination
}

void setPort(char new_port[], uint8_t str_len) {
	strncpy(ssid, new_port, 15);
	port[str_len] = '\0';									// Ensure null termination
}

void fullReset(void) {
	UART_OutString("Stopping timer...\n");
	Timer5A_Init(&ESP2TM4C, 0, 4);
	
	UART_OutString("Reinitializing UART5...\n");
	UART5_Init();
	
	UART_OutString("Resetting ESP8266...\n");
	Reset_8266();
	
	UART_OutString("Setting up WiFi...\n");
	SetupWiFi();
	
	UART_OutString("Timer reinitializing...");
	Timer5A_Init(&ESP2TM4C, 400000, 4);
}