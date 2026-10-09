#include <Arduino.h>
// Libraries
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//"" are used because BLE_UART.h is in this project file,
// while <> are used for installed libraries
#include "BLE_UART.h"

// Global variables
// OLED Global varaibles
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_DC 16
#define OLED_RESET 17
#define OLED_CS 5
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);

// BLE Global varaibles
bool wasConnected = false;
int messageCount = 0;

// Functions
void showOnOLED(String title, String message)
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Displaying the title
  // The size of the text is different from the lab's because of
  // an issue with displaying certain characters)
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(title);
  display.drawLine(0,16,127,16,SSD1306_WHITE);

  // Displaying the message
  // The posistion of the message is different from the lab's to
  // accommodate for the issue with the title
  display.setTextSize(2);
  display.setCursor(0,24);
  display.println(message);

  display.display();
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Program Started");

  // OLED code
  display.begin(SSD1306_SWITCHCAPVCC);
  Serial.println("OLED Started");
  showOnOLED("Lab 8: BLE","Jacob");

  // BLE code
  bleBegin("ESP32_Jacob");
  Serial.println("BLE Started");
  showOnOLED("BLE Ready","Waiting");
}

void loop()
{
  // Connection status
  if (bleIsConnected() == true && wasConnected == false)
  {
    wasConnected = true;
    Serial.println("Phone Connected");
    showOnOLED("BLE Status", "Connected");
  }

  if (bleIsConnected() == false && wasConnected == true)
  {
    wasConnected = false;
    Serial.println("Phone Disconnected");
    showOnOLED("BLE Status", "Disconnected");
  }

  // Messages
  if(bleMessageAvailable() == true)
  {
    String message = bleReadMessage();

    messageCount = messageCount + 1;
    
    Serial.print("Received: ");
    Serial.println(message);

    String title = "Message " + String(messageCount);
    showOnOLED(title, message);

    bleSend("Shown on OLED");
  }
}