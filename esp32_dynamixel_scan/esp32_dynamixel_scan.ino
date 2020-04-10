/*******************************************************************************
 * 
 * Derived by https://github.com/Maehem from ROBOTIS Dynamixel2Arduino demo code 
 * 
* Copyright 2016 ROBOTIS CO., LTD.
* 
* REQUIRES: Dynamixel2Arduino library in your Arduino IDE
* https://github.com/ROBOTIS-GIT/Dynamixel2Arduino
* 
* 
* Getting the Robotis Dynamixel to work on a ESP32 required some changes in
* the stock demo code.
* 
* -- A custom serial port handler was written and mostly based on Robitis own example code.
*     * begin() was overridden.
*     * Track whether port is open already to prevent esp32 lockup when changing baud rate.
*     * First time begin() calls port.begin() with pin settings and baud.
*     * Subsequent calls to begin only affect baud rate by calling esp32 updateBaudRate() method.
* 
* 
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include <Dynamixel2Arduino.h>

// Debug messages will appear on USB/serial monitor connection.
#define DEBUG_SERIAL Serial

// This is the other tab in the Arduino IDE
#include "ESP32SerialPortHandler.cpp"

// Port and pins specific to your ESP32 configuration.
#define DXL_SERIAL   Serial1
const uint8_t DXL_DIR_PIN = 4; //  DIR pin
const uint8_t DXL_RX_PIN = 39; //  RX PIN
const uint8_t DXL_TX_PIN = 32; //  TX PIN

#define MAX_BAUD  5
const int32_t baud[MAX_BAUD] = {57600, 115200, 1000000, 2000000, 3000000};

// The old way of creating dxl
//Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

// New way of creating dxl
Dynamixel2Arduino dxl;

// Our custom handler with RX and TX pins specified.
ESP32SerialPortHandler esp_dxl_port(DXL_SERIAL, DXL_RX_PIN, DXL_TX_PIN, DXL_DIR_PIN);


void setup() {

  // Set custom port handler
  dxl.setPort(esp_dxl_port);
  
  int8_t index = 0;
  int8_t found_dynamixel = 0;

  // Use UART port of DYNAMIXEL Shield to debug.
  DEBUG_SERIAL.begin(115200);   //set debugging port baudrate to 115200bps
  while(!DEBUG_SERIAL);         //Wait until the serial port is opened
    
  for(int8_t protocol = 1; protocol < 3; protocol++) {
    // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
    dxl.setPortProtocolVersion((float)protocol);
    DEBUG_SERIAL.print("SCAN PROTOCOL ");
    DEBUG_SERIAL.println(protocol);
    
    for(index = 0; index < MAX_BAUD; index++) {
      // Set Port baudrate.
      DEBUG_SERIAL.print("SCAN BAUDRATE ");
      DEBUG_SERIAL.println(baud[index]);
      dxl.begin(baud[index]);
      
      for(int id = 0; id < DXL_BROADCAST_ID; id++) {
        //iterate until all ID in each buadrate is scanned.
        if(dxl.ping(id)) {
          DEBUG_SERIAL.print("ID : ");
          DEBUG_SERIAL.print(id);
          DEBUG_SERIAL.print(", Model Number: ");
          DEBUG_SERIAL.print(dxl.getModelNumber(id));
          found_dynamixel++;
          DEBUG_SERIAL.println();
        }
      }      
    }
  }
  
  DEBUG_SERIAL.print("Total ");
  DEBUG_SERIAL.print(found_dynamixel);
  DEBUG_SERIAL.println(" DYNAMIXEL(s) found!");
}

void loop() {
  // put your main code here, to run repeatedly:
}
