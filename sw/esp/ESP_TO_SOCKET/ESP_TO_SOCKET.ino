#include <ESP8266WiFi.h>
#include <stdio.h>
#include <string.h>

// #define     DEBUG1                       // First level of Debug
// #define     DEBUG2                          // Second level of Debug
// #define     DEBUG3                       // Third level of Debug

#define RDY 2                               // GPIO_2
#define SER_BUF_LEN 256                              

char ssid[64] = "<WiFi Name>";
char password[64] = "<WiFi Password>";
char host[16] = "127.0.0.1";
char port[5] = "8080";

char ser_buf[SER_BUF_LEN];

WiFiClient client;

void setup() {
  // UART Setup
  Serial.begin(9600);                     
  Serial.flush();                           // Empty out anything in buffer
  delay(1000);

  // Setup GPIO
  pinMode(0, INPUT);                        // GPIO_0 Input
  pinMode(2, OUTPUT);                       // GPIO_2 Output
  digitalWrite(RDY, LOW);                   // LOW -> GPIO_2 (Not RDY)

  // Setup WiFi
  Setup_WiFi();                             // Read in SSID, Password
  delay(100);                               // Wait before using WiFi
  digitalWrite(RDY, HIGH);
}


void Setup_WiFi(void) {
  static int bufpos = 0;                // starts the buffer back at the first position in the incoming serial.read

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();                        // Disconnect the Wifi before re-enabling it

  // Wait for system to stabilize
  delay(1000);  

  digitalWrite(RDY, HIGH);
  delay(100);                             // Wait before reading

  #ifdef DEBUG1
    Serial.println("Preparing to receive...");
  #endif

  while ((Serial.available() == 0)) {}
  while (Serial.available() > 0)   {
      char inchar = Serial.read();  // assigns one byte (as serial.read()'s only input one byte at a time
      if (inchar != '\n') {         // if the incoming character is not a newline then process byte
        ser_buf[bufpos] = inchar;   // the buffer position in the array get assigned to the current read
        bufpos++;                   // once that has happend the buffer advances, doing this over and over again until the end of package marker is read.
        delay(10);
      }
  }
    
  char pf = 0; //Parse Failed
  if (bufpos  > 0) {
    pf |= get_next_token(ssid,          ser_buf, ",");
    pf |= get_next_token(password,         NULL,    ",");
    pf |= get_next_token(host,         NULL,    ",");
    pf |= get_next_token(port,         NULL,    ",");
 
    for (int i = 0; i < SER_BUF_LEN; i++)  (ser_buf[i]) = 0;
    bufpos = 0;     // Reset buffer pointer
      
    #ifdef DEBUG2
      Serial.println();
      Serial.print(ssid);
      Serial.print(',');
      Serial.print(password);
      Serial.print(',');
      Serial.print(host);
      Serial.print(',');
      Serial.print(port);
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
      Serial.print(".");                    // Feedback that we are still connecting
  }
  
  #ifdef DEBUG1
    Serial.println("\nConnected to the WiFi network");
  #endif
  
  Serial.flush();

  uint16_t port_num = atoi(port);

  if (!client.connect(host, port_num)) {
    Serial.println("Connection failed");
    delay(5000);
    return;
  }

  #ifdef DEBUG2
    if (client.connected()) {
      client.println("Hello from the ESP8266");
    }

    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 40000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        delay(10000);
        return ;
      }
    }

    timeout = millis();
    Serial.println("RCV from remote server");
    while (client.available()) {
      char ch = static_cast<char>(client.read());
      Serial.print(ch);

      if (millis() - timeout > 30000) {
        Serial.flush();
        Serial.println("Stopping debugging remote");
      }
    }
  #endif
  Serial.flush();
  digitalWrite(RDY, LOW);                   // Break out of loop in WiFi Setup
}   

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

String getSerialMessage() {
  String message = "";
  
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c != '\n') {
      message += c;
      delay(10);
    }
  }

  // // ACK to TM4C to unblock
  // Serial.println("ACK");
  // Serial.flush();

  return message;
}

void tm4c2server(String msg) {
  if (client.connected()) {
    client.println(msg);
  }
  else {
    Serial.println("Server communication failed...");
  }

  Serial.flush();
}

String getClientMessage() {
  String message = "";

  while (client.available()) {
      char ch = static_cast<char>(client.read());
      message += ch;
  }

  return message;
}

void server2tm4c(String msg) {
  Serial.println(msg);
  Serial.flush();
}

void loop() {
  if (Serial.available()) {
    // digitalWrite(RDY, LOW);
    String msg = getSerialMessage();
    tm4c2server(msg);
    // digitalWrite(RDY, HIGH);
  }

  if (client.available()) {
    // digitalWrite(RDY, LOW);
    String msg = getClientMessage();
    server2tm4c(msg);
    // digitalWrite(RDY, HIGH);
  }
}