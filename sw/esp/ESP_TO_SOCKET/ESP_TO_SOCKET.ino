#include <ESP8266WiFi.h>
#include <stdio.h>
#include <string.h>

// #define     DEBUG1                  // First level of Debug
#define     DEBUG2                  // Second level of Debug
// #define     DEBUG3                  // Third level of Debug

#define RDY 2
#define SER_BUF_LEN 256                              

char ssid[64] = "<WiFi SSID>";
char password[64] = "<WiFi Password>";
char host[16] = "<IP Address>";
char port[5] = "1111";

char  ser_buf[SER_BUF_LEN];

// ----------------------------------------------------------------
//  -------     Define Safer tokenizer     ------------------------
// ----------------------------------------------------------------

//Similar to strcpy(dest, strtok(src, delim)); except without hard faults
int get_next_token(char *dest, char *src, const char *delim){
  static char* last_src;

  //Continue with old token string, if a new one is not provided.
  if(src != NULL) last_src = src;

  //Check for failure conditions
  if((dest == NULL) || (last_src == NULL)) return 1;  //Return failure if either of the source pointers are NULL  
  if(last_src[0] == 0x00) return 1;                 //Return failure if the src string is empty

  //Attempt to copy the string by character
  while(true){
    if( last_src[0] == 0x00 ){
      break;                                        //String was 'split' due to the string ending  
    } else if( strchr(delim, last_src[0]) == NULL ){
      
      dest[0] = last_src[0];                        //If a delimiter character is not in this position, copy the char
      dest++;
      last_src++;
    } else {
      dest[0] = 0x00;
      last_src[0] = 0x00;
      last_src++;                                  //Increment the position so in the next call, we are in valid spot.
      break;
    }
  }
  
  return 0; //Success!
}

// ----------------------------------------------------------------------------
// This routine sets up Wifi. First step is receive the SSID, Password and
// student EID code using CSV format. Second step is to parse it and try
// to connect to the WiFi hotspot. Once the WiFI connection is established
// we then connect to the MQTT broker
//

void Setup_Wifi(void) {

  //Create a buffer to handle commands sent via UART to the ESP during setup.
  static int bufpos = 0;                // starts the buffer back at the first position in the incoming serial.read

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();                    // Disconnect the Wifi before re-enabling it

  // Wait for system to stabilize
  delay(1000);  
  Serial.print("\nSend a newline, or a command string to start\n");
  Serial.flush();                       // Ensure no old data is left in the buffer before signaling RDY 

  //Indicate to the connected device that the ESP is ready to recive a command 
  digitalWrite(RDY, HIGH);              // Set RDY to TM4C
  delay (500);                          // Wait before checking if serial data is being sent

  //Wait until at least one byte of data is recevied via UART, then record data to the 
  //bufffer until a new line character ('\n') is recevied.
  while ((Serial.available() == 0)) {}
  while (Serial.available() > 0)   {
      char inchar = Serial.read();  // assigns one byte (as serial.read()'s only input one byte at a time
      if (inchar != '\n') {         // if the incoming character is not a newline then process byte
        ser_buf[bufpos] = inchar;   // the buffer position in the array get assigned to the current read
        bufpos++;                   // once that has happend the buffer advances, doing this over and over again until the end of package marker is read.
        delay(10);
      }
  }

  // If any non-newline characters were sent, interprete it as a comma seperated
  // set of values to overwrite the default ssid, password, and port
  if (bufpos  > 0) {
    char pf = 0; //Parse Failed
    pf |= get_next_token(ssid,         ser_buf,    ",");
    pf |= get_next_token(password,     NULL,    ",");
    pf |= get_next_token(port,         NULL,    ",");
 
    for (int i = 0; i < SER_BUF_LEN; i++)  (ser_buf[i]) = 0;
    bufpos = 0;     // Reset buffer pointer
      
    #ifdef DEBUG2
      Serial.println();
      Serial.println(ssid);
      Serial.println(password);
      if(pf)
        Serial.print("\nFailed to parse one or more input strings!\n");
    #endif
  }

  // connect to a WiFi network
  WiFi.begin(ssid, password);
  
  #ifdef DEBUG1
    Serial.print("\nConnecting to WiFi..");
  #endif

  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");                 // Feedback that we are still connecting
  }
  
  #ifdef DEBUG1
    Serial.println("\nConnected to the WiFi network");
    Serial.println();
    Serial.print("ESP Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
  #endif
  
  Serial.flush();

}   //  END OF WiFi Setup

void setup() {
  Serial.begin(9600);                       // Set baud rate to 9600;
  Serial.flush();                           // Flush the serial port buffer
  delay(1000);
  Serial.println("Hello world starting up!");

  //Setup GPIO
  pinMode(0, INPUT);                        // Set GPIO_0 to an input
  pinMode(2, OUTPUT);                       // Set GPIO_2 to an output - RDY signal to the TM4C
  digitalWrite(RDY, LOW);                   // Set the RDY pin LOW

  //Setup Wifi & MQTT 
  Setup_Wifi();                             // This routine reads in the  EID, SSID, Password
  delay(100);                               // Wait before using WIFI

  #ifdef DEBUG3
    Serial.print("Connecting to ");
    Serial.print(host);
    Serial.print(":");
    Serial.println(port);
  #endif

  uint16_t port_num = atoi(port);

  WiFiClient client;
  if (client.connect(host, port_num)) {
    Serial.println("Connected!");

    client.println("Hello from the ESP Board!");
  } else {
    Serial.println("Connection failed!");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
