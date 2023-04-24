#pragma once

#include "Graphics.h"
#include "Game.h"
#include "Input.h"

namespace Menu
{
  boolean hardMode = false;
  uint8_t selection = 0;

  void Render_Page1()
  {
    Graphics::RenderSprite(0, 10, Sprite::menu_title, 64, 23);
    Graphics::RenderSprite(3, 60, Sprite::menu_play_selected, 56, 9);
    Graphics::RenderSprite(0, 80, Sprite::menu_difficulty_option_idle, 64, 17);
    Graphics::RenderSprite(11, 80 + 8, hardMode ? Sprite::menu_difficulty_hard : Sprite::menu_difficulty_easy, 42, 9);
    Graphics::Update();
  }

  void Render_Page2()
  {
    Graphics::RenderSprite(0, 10, Sprite::menu_title, 64, 23);
    Graphics::RenderSprite(3, 60, Sprite::menu_play_idle, 56, 9);
    Graphics::RenderSprite(0, 80, Sprite::menu_difficulty_option_selected, 64, 17);
    Graphics::RenderSprite(11, 80 + 8, hardMode ? Sprite::menu_difficulty_hard : Sprite::menu_difficulty_easy, 42, 9);
    Graphics::Update();
  }

  void Loop()
  {
    Input::UpdateInputs();
    Graphics::UpdateLightLevel();
    Display::Idle();

    if(selection == 0)
    {
      Render_Page1();

      if(Input::IsButtonDown(Input::buttonMenuDown))
      {
        selection = 1;
        return;
      }

      if(Input::IsButtonDown(Input::buttonMenuSelect))
      {
        Game::Init(hardMode);
        return;
      }
    }
    else if(selection == 1)
    {
      Render_Page2();

      if(Input::IsButtonDown(Input::buttonMenuUp))
      {
        selection = 0;
        return;
      }

      if(Input::IsButtonDown(Input::buttonMenuSelect))
      {
        hardMode = !hardMode;
        return;
      }
    }
  }
}