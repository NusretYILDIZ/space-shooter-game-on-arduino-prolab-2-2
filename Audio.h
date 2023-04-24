#pragma once

#include "Arduino.h"

#define BUZZER_PIN 8

namespace Audio
{
  void Init()
  {
    pinMode(BUZZER_PIN, OUTPUT);
  }

  void PlayDamageSound()
  {
    tone(BUZZER_PIN, 440, 200);
  }

  void PlayGameOverSound()
  {
    tone(BUZZER_PIN, 330, 150);
    delay(50);
    tone(BUZZER_PIN, 220, 150);
  }
}