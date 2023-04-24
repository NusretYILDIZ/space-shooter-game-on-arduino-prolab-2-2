#include "Menu.h"

void setup() 
{
  Audio::Init();
  Input::Init();
  Display::Init();

  if(!Graphics::Init())
    Display::InitializationError();
}

void loop() 
{ 
  Menu::Loop();
}