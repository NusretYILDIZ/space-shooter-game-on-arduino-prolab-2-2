#pragma once

#include "Arduino.h"

#define PIN_BUTTON_MENU_UP      50
#define PIN_BUTTON_MENU_DOWN    51
#define PIN_BUTTON_MENU_SELECT  52
#define PIN_BUTTON_GAME_FIRE    53
#define PIN_ANALOG_PLAYER_X     A1

namespace Input
{
  uint8_t potPos;
  uint8_t currentButtonStates;
  uint8_t previousButtonStates;

  // Bitler
  uint8_t buttonMenuUp = 1;
  uint8_t buttonMenuDown = 2;
  uint8_t buttonMenuSelect = 4;
  uint8_t buttonGameFire = 8;

  void Init()
  {
    potPos = 3;
    currentButtonStates = 0;
    previousButtonStates = 0;

    for(uint8_t i = PIN_BUTTON_MENU_UP; i <= PIN_BUTTON_GAME_FIRE; i++)
      pinMode(i, INPUT_PULLUP);
  }

  void UpdateInputs()
  {
    previousButtonStates = currentButtonStates;

    // Her buton için:
    // Eğer butonun bağlı olduğu pinin değeri 1 ise ilgili biti temizle,
    // Eğer 0 ise ilgili biti set et.
    // Bitler yukarıda yer alıyor.
    // Butonların ters mantıkta çalışmasının nedeni dahili pull-up dirençlerinin kullanılması.
    // Butona basılmadığında otomatik olarak 1 değeri elde ediliyor.
    for(uint8_t i = 0; i <= 3; i++)
      digitalRead(PIN_BUTTON_MENU_UP + i) ? ( currentButtonStates &= ~(uint8_t)(1 << i) ) : ( currentButtonStates |= (uint8_t)(1 << i) );

    potPos = analogRead(PIN_ANALOG_PLAYER_X) / 128;
  }

  boolean IsButtonDown(uint8_t button)
  {
    return !(previousButtonStates & button) && (currentButtonStates & button);
  }

  boolean IsButtonUp(uint8_t button)
  {
    return (previousButtonStates & button) && !(currentButtonStates & button);
  }

  boolean IsButtonPressed(uint8_t button)
  {
    return currentButtonStates & button;
  }
}