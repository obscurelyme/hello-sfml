// clang-format off
#ifdef _WINDOWS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
// clang-format on

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <chrono>
#include <filesystem>
#include <iostream>

#include "Color.hpp"
#include "Cursor.hpp"
#include "FPS.hpp"
#include "FontManager.hpp"
#include "Game/Collider.hpp"
#include "Game/PlayerEvents.hpp"
#include "Game/Scene.hpp"
#include "Game/Scenes/MainScene.hpp"
#include "Game/Scenes/TitleScene.hpp"
#include "InputManager.hpp"
#include "Logger.hpp"
#include "Renderer.hpp"
#include "Timer.hpp"
#include "Utilities.hpp"
#include "Widgets/Button.hpp"
#include "Widgets/Text.hpp"
#include "Window.hpp"

bool quit = false;
SDL_Event event;

// SDL_AssertState appHandler(const SDL_AssertData* data, void*) {
//   std::cout << "Error executing function: " << data->function << std::endl;
//   return SDL_ASSERTION_IGNORE;
// };

int main(int, char**) {
  // Start clock
  auto start = std::chrono::steady_clock::now();
  CM_LOGGER_INIT();

  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    CM_LOGGER_CRITICAL("Could not initialize SDL2!");
    exit(1);
  }

  if (IMG_Init(IMG_INIT_PNG) == 0) {
    CM_LOGGER_CRITICAL("Could not initialize SDL2 Images");
    exit(1);
  }

  // SDL_SetAssertionHandler(appHandler, nullptr);

  CoffeeMaker::Utilities::Init(SDL_GetBasePath());
  CoffeeMaker::Texture::SetTextureDirectory();

  CoffeeMaker::BasicWindow win("Hello, SDL!", 800, 600);
  CoffeeMaker::Renderer renderer;
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<float> elapsedSeconds = end - start;
  CoffeeMaker::Logger::Info(fmt::format(fmt::runtime("Initialization time took: {0}"), elapsedSeconds.count()));

  CoffeeMaker::TextView text{"Hello, World!"};
  text.color = CoffeeMaker::Color(255, 255, 255, 255);
  text.SetFont(fontManager.useFont("Roboto/Roboto-Regular"));
  text.SetTextContentTexture();

  CoffeeMaker::Button button;
  button.clientRect.y = 100;
  button.clientRect.x = 200;
  CoffeeMaker::Texture texture("test.png");
  CoffeeMaker::Widgets::Image img("loaded.png");
  img.LoadImage();
  CoffeeMaker::Shapes::Rect rect(100, 100);
  CoffeeMaker::Shapes::Line line(100, 600 / 2, 800 / 2, 0);

  // CoffeeMaker::Cursor cursor("cursor.png");
  CoffeeMaker::FontManager::Init();
  CoffeeMaker::FontManager::LoadFont("Roboto/Roboto-Regular");
  CoffeeMaker::FontManager::LoadFont("Roboto/Roboto-Black");

  CoffeeMaker::Logger::Info(fmt::format(fmt::runtime("Display count: {0}"), win.DisplayCount()));
  CoffeeMaker::Logger::Info(fmt::format(fmt::runtime("Current Window DPI {0}"), win.GetScreenDPI().toString()));

  CoffeeMaker::Timer timer;
  CoffeeMaker::FPS fpsCounter;

  // SCOREBOARD, use in MainScene
  // CoffeeMaker::Widgets::View scoreView{200, 75, CoffeeMaker::UIProperties::HorizontalAlignment::Centered,
  //                                      CoffeeMaker::UIProperties::VerticalAlignment::Top};
  // CoffeeMaker::Widgets::Text scoreText;
  // scoreText.SetFont(CoffeeMaker::FontManager::UseFont("Roboto/Roboto-Regular"));
  // scoreText.SetText("Hello, World!");
  // scoreText.SetColor(CoffeeMaker::Color(0, 255, 255, 255));
  // scoreView.AppendChild(&scoreText);
  // End SCOREBOARD

  CM_LOGGER_INFO("Initialization time took: {}", elapsedSeconds.count());
  CM_LOGGER_INFO("Display count: {}", win.DisplayCount());
  CM_LOGGER_INFO("Current Window DPI {}", win.GetScreenDPI().toString());

  SceneManager::AddScene(new TitleScene());
  SceneManager::AddScene(new MainScene());
  SceneManager::LoadScene();
  win.ShowWindow();
  CoffeeMaker::InputManager::Init();

  while (!quit) {
    // get input
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      }

      CoffeeMaker::Button::PollEvents(&event);

      if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        CoffeeMaker::InputManager::HandleKeyBoardEvent(&event.key);
      }
    }

    // CoffeeMaker::Inputloop->run();

    // physics step
    Collider::PhysicsUpdate();

    // run logic
    // fpsCounter.Update();
    SceneManager::UpdateCurrentScene();

    // render
    renderer.BeginRender();

    SceneManager::RenderCurrentScene();
    // fpsCounter.Render();

    renderer.EndRender();

    CoffeeMaker::InputManager::ClearAllPresses();

    CoffeeMaker::Button::ProcessEvents();
    // Cap framerate
    SDL_Delay(16);
  }

  SceneManager::DestroyAllScenes();
  CoffeeMaker::FontManager::Destroy();
  renderer.Destroy();
  SDL_Quit();

  incScore.reset();

  CM_LOGGER_DESTROY();

#ifdef _WINDOWS
  _CrtDumpMemoryLeaks();
#endif

  return 0;
}
