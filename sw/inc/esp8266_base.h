//***********************  ESP8266.h  ***********************
// Program written by:
// - Steven Prickett  steven.prickett@gmail.com
//
// Driver for ESP8266 module to act as a WiFi client or server
//
//*********************************************************
/* Modified by Jonathan Valvano, Sept 19, 2015
   Modified by Andreas Gerstlauer, Apr 13, 2020 
 */

#ifndef ESP8266_H
#define ESP8266_H

#include <stdint.h>

#define ESP8266_ENCRYPT_MODE_OPEN            0
#define ESP8266_ENCRYPT_MODE_WEP             1
#define ESP8266_ENCRYPT_MODE_WPA_PSK         2
#define ESP8266_ENCRYPT_MODE_WPA2_PSK        3
#define ESP8266_ENCRYPT_MODE_WPA_WPA2_PSK    4

#define ESP8266_WIFI_MODE_CLIENT            1
#define ESP8266_WIFI_MODE_AP                2
#define ESP8266_WIFI_MODE_AP_AND_CLIENT     3
#define BUFFER_SIZE 												40

typedef struct Message{
	char msg[BUFFER_SIZE];
	uint8_t owner;				// 0 is recv (current board from current POV) 1 is sender (other board from current POV)
} Message;


//------------------- ParseMsg --------------
// Turns UART message stored in ser_buf into message struct
// Inputs: None.
// Outputs: UART and ILI
Message ParseMsg(void);

//------------------- Reset_8266 --------------
// Commands the ESP to reset, determines completion by waiting
// Inputs: None.
// Outputs: PE1 and PE3 are used
void Reset_8266(void);

//------------------- SetupWiFi --------------
// Sends data over UART to change the ESP wifi setup
// Inputs: Hard coded globals
// Outputs: Can use UART, UART5, and ILI
void SetupWiFi(void);


//------------------- ESP2TM4C --------------
// Receives data over UART and will output to device
// Inputs: None.
// Outputs: Can use UART, UART5, and ILI
void ESP2TM4C(void);

void setSSID(char new_ssid[], uint8_t str_len);
	
void setPass(char new_pass[], uint8_t str_len);

void setHost(char new_host[], uint8_t str_len);

void setPort(char new_port[], uint8_t str_len);

void fullReset(void);

#endif