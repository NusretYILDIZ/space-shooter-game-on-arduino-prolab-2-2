#include <stdint.h>
#pragma once

#include "Audio.h"
#include "Display.h"
#include "Graphics.h"
#include "Input.h"
#include "Player.h"

namespace Game
{
  int8_t objects[16 * 8] = { 0 };

  uint32_t generatedObjectCount;

  unsigned long gameTick;
  unsigned long rowGenerationDelay;
  unsigned long generateRowTick;
  unsigned long difficultyTweakDelay;
  unsigned long difficultyTweakTick;
  unsigned long deltaTime;
  long invincibilityTick;

  uint8_t gameObjectCount = 0;
  uint8_t enemyCount = 0;

  boolean hardMode;
  boolean currentFrameProcessed = false;
  boolean renderPlayer = true;
  boolean shouldRun = true;

  void ShiftDown()
  {
    for(uint8_t i = 16*8; i >= 8; i--)
      objects[i] = objects[i - 8];
  }

  void GenerateRow()
  {
    int8_t selection;

    for(int8_t i = 0; i < 8; i++)
    {
      selection = random(100);

      // 14 nesnede bir can ve silah koyacağız ancak ilk satırda koymaması için değişik bir kontrol kullanıldı.
      if(generatedObjectCount % 14 != 13)
      {
        if(selection >= 0 && selection < 50)
          objects[i] = GAMEOBJECT_TYPE_NONE;
        
        else if(selection >= 50 && selection < 74)
        {
          objects[i] = GAMEOBJECT_TYPE_METEROID_NORMAL;
          generatedObjectCount++;
        }
        else if(selection >= 74 && selection < 98)
        {
          objects[i] = GAMEOBJECT_TYPE_SPACEJUNK;
          generatedObjectCount++;
        }
        else if(selection >= 98 && selection < 100)
        {
          objects[i] = GAMEOBJECT_TYPE_INVINCIBILITY;
          generatedObjectCount++;
        }
        else
          continue;
      }
      else
      {
        if(selection % 2)
        {
          objects[i] = GAMEOBJECT_TYPE_MEDICBAG;
          generatedObjectCount++;
        }
        else
        {
          objects[i] = GAMEOBJECT_TYPE_AMMOBAG;
          generatedObjectCount++;
        }
      }
    }

    currentFrameProcessed = false;
  }

  void GameLoop()
  {
    gameTick = millis();

    while(shouldRun)
    {
      if(millis() > gameTick + 99)
      {
        deltaTime = 100;
        gameTick = millis();

        generateRowTick += deltaTime;
        difficultyTweakTick += deltaTime;

        if(invincibilityTick > 0)
        {
          if(invincibilityTick < deltaTime)
          {
            invincibilityTick = 0;
            renderPlayer = true;
          }
          else
          {
            invincibilityTick -= deltaTime;
            renderPlayer = !renderPlayer;
          }

          if(invincibilityTick == 0)
            renderPlayer = true;
        }

        if(hardMode && difficultyTweakTick >= difficultyTweakDelay && rowGenerationDelay > 100)
        {
          rowGenerationDelay -= rowGenerationDelay / 5;
          difficultyTweakTick = 0;
        }

        if(generateRowTick >= rowGenerationDelay)
        {
          generateRowTick = 0;
          ShiftDown();
          GenerateRow();

          Player::score++;
          Display::PrintNumber(Player::score);
        }
        
        Input::UpdateInputs();
        Player::posX = Input::potPos;
        Player::posY = 15;

        for(uint8_t i = 8; i < 16*8; i++)
        {
          if(objects[i] == GAMEOBJECT_TYPE_BULLET)
          {
            if(objects[i - 8] == GAMEOBJECT_TYPE_METEROID_NORMAL)
              objects[i - 8] = GAMEOBJECT_TYPE_METEROID_DAMAGED;

            else if(objects[i - 8] != GAMEOBJECT_TYPE_NONE)
              objects[i - 8] = GAMEOBJECT_TYPE_NONE;
            
            else
              objects[i - 8] = GAMEOBJECT_TYPE_BULLET;
            
            objects[i] = GAMEOBJECT_TYPE_NONE;
          }
        }

        if(Input::IsButtonDown(Input::buttonGameFire) && Player::ammo > 0)
        {
          if(objects[(Player::posY - 1) * 8 + Player::posX] == GAMEOBJECT_TYPE_METEROID_NORMAL)
            objects[(Player::posY - 1) * 8 + Player::posX] = GAMEOBJECT_TYPE_METEROID_DAMAGED;
          
          else if(objects[(Player::posY - 1) * 8 + Player::posX] != GAMEOBJECT_TYPE_NONE)
            objects[(Player::posY - 1) * 8 + Player::posX] = GAMEOBJECT_TYPE_NONE;
          
          else
            objects[(Player::posY - 1) * 8 + Player::posX] = GAMEOBJECT_TYPE_BULLET;
          
          Player::UseWeapon();
        }

        if((objects[Player::posY * 8 + Player::posX] == GAMEOBJECT_TYPE_METEROID_NORMAL || 
            objects[Player::posY * 8 + Player::posX] == GAMEOBJECT_TYPE_METEROID_DAMAGED || 
            objects[Player::posY * 8 + Player::posX] == GAMEOBJECT_TYPE_SPACEJUNK) && !currentFrameProcessed && invincibilityTick == 0)
        {
          Player::TakeHit();
          Audio::PlayDamageSound();
          invincibilityTick = 3000;
          currentFrameProcessed = true;
        }
        else if(objects[Player::posY * 8 + Player::posX] == GAMEOBJECT_TYPE_MEDICBAG && Player::lives < 3)
        {
          objects[Player::posY * 8 + Player::posX] = GAMEOBJECT_TYPE_NONE;
          Player::GrantLive();
        }
        else if(objects[Player::posY * 8 + Player::posX] == GAMEOBJECT_TYPE_AMMOBAG && Player::ammo < 3)
        {
          objects[Player::posY * 8 + Player::posX] = GAMEOBJECT_TYPE_NONE;
          Player::GrantAmmo();
        }
        else if(objects[Player::posY * 8 + Player::posX] == GAMEOBJECT_TYPE_INVINCIBILITY)
        {
          objects[Player::posY * 8 + Player::posX] = GAMEOBJECT_TYPE_NONE;
          invincibilityTick = 3000;
        }

        if(Player::lives == 0)
        {
          Audio::PlayGameOverSound();

          do {
            Input::UpdateInputs();
            Graphics::UpdateLightLevel();
            Graphics::Clear();
            Graphics::RenderSprite(16, 20, Sprite::game_over, 32, 20);
            Graphics::RenderSprite(4, 70, Sprite::main_menu_option, 56, 20);
            Graphics::Update();
          } while (!Input::IsButtonDown(Input::buttonMenuSelect));

          shouldRun = false;
        }

        Graphics::UpdateLightLevel();
        Graphics::Clear();

        if(shouldRun)
          Graphics::RenderScene(objects, renderPlayer);

        Graphics::Update();
        Display::Update();
      }
    }
  }

  void ResetGame()
  {
    for(uint8_t i = 0; i < 16*8; i++)
      objects[i] = GAMEOBJECT_TYPE_NONE;
    
    Player::Reset();

    generatedObjectCount = 0;
    generateRowTick = 0;
    invincibilityTick = 0;

    shouldRun = true;

    Display::Update();
  }

  void Init(boolean isHardMode)
  {
    randomSeed(Player::score);

    rowGenerationDelay = 1000;
    difficultyTweakDelay = 10000;
    hardMode = isHardMode;

    ResetGame();
    GameLoop();
  }
}