/*       7-Segment Display
          Pin Connections
       _____________________
       \________ 1 ________/
     |\                     /|
     | |                   | |
     | |                   | |
     |6|                   |2|
     | |                   | |
     | |                   | |
     |/ ___________________ \|
      < ________ 7 ________ >
     |\                     /|
     | |                   | |
     | |                   | |
     |5|                   |3|
     | |                   | |
     | |                   | |
     |/ ___________________ \|
       /________ 4 ________\
*/

#pragma once

#include "Arduino.h"
#include "Player.h"

#define DISPLAY1_SEGMENT1 22
#define DISPLAY1_SEGMENT2 23
#define DISPLAY1_SEGMENT3 24
#define DISPLAY1_SEGMENT4 25
#define DISPLAY1_SEGMENT5 26
#define DISPLAY1_SEGMENT6 27
#define DISPLAY1_SEGMENT7 28

#define DISPLAY2_SEGMENT1 31
#define DISPLAY2_SEGMENT2 32
#define DISPLAY2_SEGMENT3 33
#define DISPLAY2_SEGMENT4 34
#define DISPLAY2_SEGMENT5 35
#define DISPLAY2_SEGMENT6 36
#define DISPLAY2_SEGMENT7 37

#define DISPLAY3_SEGMENT1 43
#define DISPLAY3_SEGMENT2 44
#define DISPLAY3_SEGMENT3 45
#define DISPLAY3_SEGMENT4 46
#define DISPLAY3_SEGMENT5 47
#define DISPLAY3_SEGMENT6 48
#define DISPLAY3_SEGMENT7 49

#define DISPLAY_1 0
#define DISPLAY_2 1
#define DISPLAY_3 2

#define LED_LIVES_1 2
#define LED_LIVES_2 3
#define LED_LIVES_3 4

#define LED_WEAPON_1 5
#define LED_WEAPON_2 6
#define LED_WEAPON_3 7

namespace Display
{
  const uint8_t segmentData[] = {
    0b11111100, // 0
    0b01100000, // 1
    0b11011010, // 2
    0b11110010, // 3
    0b01100110, // 4
    0b10110110, // 5
    0b10111110, // 6
    0b11100000, // 7
    0b11111110, // 8
    0b11110110  // 9
  };

  const uint8_t segmentErrorData[] = {
    0b10011110, // E
    0b00001010  // r
  };

  void DisplayLives(int8_t lives)
  {
    switch(lives)
    {
    case 1:
      digitalWrite(LED_LIVES_1, HIGH);
      digitalWrite(LED_LIVES_2, LOW);
      digitalWrite(LED_LIVES_3, LOW);
      break;
    
    case 2:
      digitalWrite(LED_LIVES_1, HIGH);
      digitalWrite(LED_LIVES_2, HIGH);
      digitalWrite(LED_LIVES_3, LOW);
      break;
    
    case 3:
      digitalWrite(LED_LIVES_1, HIGH);
      digitalWrite(LED_LIVES_2, HIGH);
      digitalWrite(LED_LIVES_3, HIGH);
      break;
    
    default:
      digitalWrite(LED_LIVES_1, LOW);
      digitalWrite(LED_LIVES_2, LOW);
      digitalWrite(LED_LIVES_3, LOW);
      break;
    }
  }

  void DisplayAmmo(int8_t ammo)
  {
    switch(ammo)
    {
    case 1:
      digitalWrite(LED_WEAPON_1, HIGH);
      digitalWrite(LED_WEAPON_2, LOW);
      digitalWrite(LED_WEAPON_3, LOW);
      break;
    
    case 2:
      digitalWrite(LED_WEAPON_1, HIGH);
      digitalWrite(LED_WEAPON_2, HIGH);
      digitalWrite(LED_WEAPON_3, LOW);
      break;
    
    case 3:
      digitalWrite(LED_WEAPON_1, HIGH);
      digitalWrite(LED_WEAPON_2, HIGH);
      digitalWrite(LED_WEAPON_3, HIGH);
      break;
    
    default:
      digitalWrite(LED_WEAPON_1, LOW);
      digitalWrite(LED_WEAPON_2, LOW);
      digitalWrite(LED_WEAPON_3, LOW);
      break;
    }
  }

  void Idle()
  {
    // 7-segmentlerde --- gösteriyor
    for(int8_t i = DISPLAY1_SEGMENT1; i <= DISPLAY1_SEGMENT7; i++)
      digitalWrite(i, i == DISPLAY1_SEGMENT7 ? LOW : HIGH);

    for(int8_t i = DISPLAY2_SEGMENT1; i <= DISPLAY2_SEGMENT7; i++)
      digitalWrite(i, i == DISPLAY2_SEGMENT7 ? LOW : HIGH);

    for(int8_t i = DISPLAY3_SEGMENT1; i <= DISPLAY3_SEGMENT7; i++)
      digitalWrite(i, i == DISPLAY3_SEGMENT7 ? LOW : HIGH);

    DisplayAmmo(0);
    DisplayLives(0);
  }

  void Init()
  {
    for(int8_t i = DISPLAY1_SEGMENT1; i <= DISPLAY1_SEGMENT7; i++)
    {
      pinMode(i, OUTPUT);
      digitalWrite(i, HIGH);
    }
    for(int8_t i = DISPLAY2_SEGMENT1; i <= DISPLAY2_SEGMENT7; i++)
    {
      pinMode(i, OUTPUT);
      digitalWrite(i, HIGH);
    }
    for(int8_t i = DISPLAY3_SEGMENT1; i <= DISPLAY3_SEGMENT7; i++)
    {
      pinMode(i, OUTPUT);
      digitalWrite(i, HIGH);
    }

    for(int8_t i = LED_LIVES_1; i <= LED_WEAPON_3; i++)
    {
      pinMode(i, OUTPUT);
      digitalWrite(i, LOW);
    }

    Idle();
  }

  void OutputToDisplay(uint8_t number, int8_t display)
  {
    if(display == DISPLAY_1)
      for(int8_t i = 0; i < 7; i++)
        digitalWrite(i + DISPLAY1_SEGMENT1, (bitRead(segmentData[number], 7 - i)) ? LOW : HIGH);

    else if(display == DISPLAY_2)
      for(int8_t i = 0; i < 7; i++)
        digitalWrite(i + DISPLAY2_SEGMENT1, (bitRead(segmentData[number], 7 - i)) ? LOW : HIGH);

    else if(display == DISPLAY_3)
      for(int8_t i = 0; i < 7; i++)
        digitalWrite(i + DISPLAY3_SEGMENT1, (bitRead(segmentData[number], 7 - i)) ? LOW : HIGH);
  }

  void PrintNumber(uint16_t number)
  {
    for(int8_t i = DISPLAY_3; i >= DISPLAY_1; i--)
    {
      OutputToDisplay(number % 10, i);
      number /= 10;
    }
  }

  void InitializationError()
  {
    for(int8_t i = 0; i < 7; i++)
      digitalWrite(i + DISPLAY1_SEGMENT1, bitRead(segmentErrorData[0], 7 - i) ? LOW : HIGH);

    for(int8_t i = 0; i < 7; i++)
      digitalWrite(i + DISPLAY2_SEGMENT1, bitRead(segmentErrorData[1], 7 - i) ? LOW : HIGH);
      
    for(int8_t i = 0; i < 7; i++)
      digitalWrite(i + DISPLAY3_SEGMENT1, bitRead(segmentErrorData[1], 7 - i) ? LOW : HIGH);

    while(1)
    {
      DisplayLives(3);
      DisplayAmmo(3);
      delay(200);

      DisplayLives(0);
      DisplayAmmo(0);
      delay(200);
    }
  }

  void Update()
  {
    PrintNumber(Player::score);
    DisplayLives(Player::lives);
    DisplayAmmo(Player::ammo);
  }
}