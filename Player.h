#pragma once

namespace Player
{
  int8_t posX;
  int8_t posY;
  int8_t lives;
  int8_t ammo;
  uint16_t score;

  void Reset()
  {
    posX = 3;
    posY = 15;
    lives = 3;
    ammo = 3;
    score = 0;
  }

  void TakeHit()
  {
    if(lives > 0)
      lives--;
  }

  void UseWeapon()
  {
    if(ammo > 0)
      ammo--;
  }

  void GrantLive()
  {
    if(lives < 3)
      lives++;
  }

  void GrantAmmo()
  {
    if(ammo < 3)
      ammo++;
  }
}