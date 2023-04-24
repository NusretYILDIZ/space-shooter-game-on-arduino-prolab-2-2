#pragma once

#include "Arduino.h"
#include "Display.h"
#include "src/OLED_I2C/OLED_I2C.h"
#include "Sprites.h"
#include "Player.h"

#define PIN_LDR A0

#define LIGHTLEVEL_DIMM          1
#define LIGHTLEVEL_BRIGHT        2

#define GAMEOBJECT_TYPE_NONE              0
#define GAMEOBJECT_TYPE_METEROID_NORMAL   1
#define GAMEOBJECT_TYPE_METEROID_DAMAGED  2
#define GAMEOBJECT_TYPE_SPACEJUNK         3
#define GAMEOBJECT_TYPE_MEDICBAG          4
#define GAMEOBJECT_TYPE_AMMOBAG           5
#define GAMEOBJECT_TYPE_PLAYER            6
#define GAMEOBJECT_TYPE_BULLET            7
#define GAMEOBJECT_TYPE_INVINCIBILITY     8

namespace Graphics
{
  OLED oled(SDA, SCL);

  uint8_t vram[16][8] = { 0 };
  uint8_t lightLevel = LIGHTLEVEL_DIMM;

  boolean Init()
  {
    if(!oled.begin(SSD1306_128X64))
      return 0;
    
    oled.rotateDisplay(true);
    return 1;
  }

  void UpdateLightLevel()
  {
    int currentLightLevel = analogRead(PIN_LDR);
    
    if(currentLightLevel >= 512)
    {
      lightLevel = LIGHTLEVEL_DIMM;
      oled.invert(false);
    }
    else
    {
      lightLevel = LIGHTLEVEL_BRIGHT;
      oled.invert(true);
    }
  }

  void RenderSprite(uint8_t x, uint8_t y, const uint8_t sprite[], uint8_t sx, uint8_t sy)
  {
    oled.drawBitmap(y, x, sprite, sy, sx);
  }

  void RenderScene(int8_t* objects, boolean renderPlayer)
  {
    for(int8_t y = 0; y < 16; y++)
      for(int8_t x = 0; x < 8; x++)
      {
        if(objects[y*8 + x] == GAMEOBJECT_TYPE_BULLET)
          oled.drawBitmap(y * 8, x * 8, Sprite::bullet, 7, 7);

        else if(objects[y*8 + x] == GAMEOBJECT_TYPE_METEROID_NORMAL)
          oled.drawBitmap(y * 8, x * 8, Sprite::meteroid_normal, 7, 7);

        else if(objects[y*8 + x] == GAMEOBJECT_TYPE_METEROID_DAMAGED)
          oled.drawBitmap(y * 8, x * 8, Sprite::meteroid_damaged, 7, 7);
        
        else if(objects[y*8 + x] == GAMEOBJECT_TYPE_SPACEJUNK)
          oled.drawBitmap(y * 8, x * 8, Sprite::spaceJunk, 7, 7);
        
        else if(objects[y*8 + x] == GAMEOBJECT_TYPE_MEDICBAG)
          oled.drawBitmap(y * 8, x * 8, Sprite::medicBag, 7, 7);
        
        else if(objects[y*8 + x] == GAMEOBJECT_TYPE_AMMOBAG)
          oled.drawBitmap(y * 8, x * 8, Sprite::ammoBag, 7, 7);
        
        else if(objects[y*8 + x] == GAMEOBJECT_TYPE_INVINCIBILITY)
          oled.drawBitmap(y * 8, x * 8, Sprite::invincibility, 7, 7);
      }
    
    if(renderPlayer)
      oled.drawBitmap(Player::posY * 8, Player::posX * 8, Sprite::spaceShip, 7, 7);
  }

  void Clear()
  {
    oled.clrScr();
  }

  void Update()
  {
    oled.update();
  }
}