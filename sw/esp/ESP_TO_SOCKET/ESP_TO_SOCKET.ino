#include <ESP8266WiFi.h>
#include <stdio.h>
#include <string.h>

#define     DEBUG1                       // First level of Debug
#define     DEBUG2                          // Second level of Debug
// #define     DEBUG3                       // Third level of Debug

#define RDY 2                               // GPIO_2
#define SER_BUF_LEN 256                              

char ssid[64] = "<WiFi Name>";
char password[64] = "<WiFi Password>";
char host[16] = "127.0.0.1";
char port[5] = "8080";

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
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();                        // Disconnect the Wifi before re-enabling it

  // Wait for system to stabilize           ** Not sure if we actually need this**
  delay(1000);  

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
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        delay(60000);
        return ;
      }
    }

    Serial.println("RCV from remote server");
    while (client.available()) {
      char ch = static_cast<char>(client.read());
      Serial.print(ch);
    }
  #endif
  Serial.flush();
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