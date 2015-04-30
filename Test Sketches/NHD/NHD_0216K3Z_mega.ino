
//---------------------------------------------------------
/*
NHD_0216K3Z_mega.ino
Program for writing to Newhaven Display 2 x 16 (K3Z) Character LCD with PIC16F690 controller

(c)2014 Mike LaVine - Newhaven Display International, LLC. 

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.
*/
//---------------------------------------------------------

#include <Wire.h>

int ASDA = 20;      // SDA signal connected to pin 20 (SDA) of Arduino Mega 2560
int ASCL = 21;      // SCL signal connected to pin 21 (SCL) of Arduino Mega 2560

unsigned char text1[]={"Newhaven Display"};
unsigned char text2[]={" International  "};

unsigned char tx_packet[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);

const char slave2w = 0x28;   //0x50 shifted over 1 bit

void send_packet(unsigned char x)
{
  unsigned char ix;
  Wire.beginTransmission(slave2w);
  for(ix=0;ix<x;ix++)
  {
    Wire.write(tx_packet[ix]);
  }
  Wire.endTransmission();
}

void lcd_cursor(unsigned int x)
{
  tx_packet[0] = 0xFE;
  tx_packet[1] = 0x45;
  tx_packet[2] = x;
  send_packet(3);
}

void lcd_clear(void)
{
  tx_packet[0] = 0xFE;
  tx_packet[1] = 0x51;
  send_packet(2);
}

void setup()       //runs first when powered on
{
  pinMode(ASCL, OUTPUT);      //set Arduino I2C lines as outputs
  pinMode(ASDA, OUTPUT);      //
  digitalWrite(ASCL, LOW);    //
  digitalWrite(ASDA, LOW);    //
  delay(100);    //100ms delay
}

void loop() 
{
  Wire.begin();             //initialize Arduino I2C
  TWBR = 152;               //change Arduino Mega 2560 I2C clock frequency from 100kHz to 50kHz
  lcd_clear();
  
  tx_packet[0] = 0xFE;
  tx_packet[1] = 0x52;
  tx_packet[2] = 40;        //contrast 1 - 50
  send_packet(3);
  
  tx_packet[1] = 0x53;      
  tx_packet[2] = 15;        //backlight 1 - 15
  send_packet(3);
  
  tx_packet[1] = 0x48;      //underline cursor off
  send_packet(2);
  
  tx_packet[1] = 0x46;      //cursor home
  send_packet(2);
  
  delay(250);
  
  while(1)
  {
  for(unsigned char i=0;i<16;i++)
  {
    tx_packet[i] = text1[i];
  }
  send_packet(16);
  
  lcd_cursor(0x40);
  delay(100);  //100ms delay
  
  for(unsigned char i=0;i<16;i++)
  {
    tx_packet[i] = text2[i];
  }
  send_packet(16);
  delay(1000);
  }
}

