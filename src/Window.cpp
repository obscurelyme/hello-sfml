#include "Window.hpp"

#include "Logger.hpp"

using namespace CoffeeMaker;

IWindow *GlobalWindow::_instance = nullptr;

GlobalWindow::GlobalWindow() {}

GlobalWindow::GlobalWindow(IWindow *win) {
  if (_instance == nullptr) {
    _instance = win;
  }
}

IWindow *GlobalWindow::Instance() { return _instance; }

void GlobalWindow::Set(IWindow *win) {
  if (_instance == nullptr) {
    _instance = win;
  }
}

void GlobalWindow::Unset() { _instance = nullptr; }

Uint32 GlobalWindow::ID() { return _instance->GetID(); }

std::string ScreenDPI::toString() {
  return fmt::format(fmt::runtime("Diagonal {}, Horizontal {}, Vertical {}"), diagonal, horizontal, vertical);
}

SDL_Window *BasicWindow::_window = nullptr;

BasicWindow::BasicWindow(std::string title, int width, int height, bool fullscreen) {
  int windowFlags = SDL_WINDOW_HIDDEN;
  if (fullscreen) {
    windowFlags = SDL_WINDOW_HIDDEN | SDL_WINDOW_FULLSCREEN_DESKTOP;
  }

  _window =
      SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, windowFlags);
  _screenDpi = SetScreenDPI();
  _dpiScale = _screenDpi.diagonal / ScreenDPI::BASE_DPI;
  SDL_GetDisplayBounds(SDL_GetWindowDisplayIndex(_window), &_screenBounds);
  SDL_GetDisplayUsableBounds(SDL_GetWindowDisplayIndex(_window), &_usableDisplayBounds);
  CM_LOGGER_INFO("Display Bounds: ({},{}) Display Usable Bounds: ({},{})", _screenBounds.w, _screenBounds.h,
                 _usableDisplayBounds.w, _usableDisplayBounds.h);
  CM_LOGGER_INFO("Display DPI Scale: {}", _dpiScale);
  // SDL_SetWindowSize(_window, _usableDisplayBounds.w, _usableDisplayBounds.h);
  // SDL_SetWindowPosition(_window, 0, 0);
  GlobalWindow::Set(this);
}

BasicWindow::~BasicWindow() {
  if (_window != nullptr) {
    if (GlobalWindow::Instance() == this) {
      GlobalWindow::Unset();
    }
    SDL_DestroyWindow(_window);
    _window = nullptr;
  }
}

int BasicWindow::DisplayCount() const { return SDL_GetNumVideoDisplays(); }

ScreenDPI BasicWindow::SetScreenDPI() {
  ScreenDPI dpi;
  SDL_GetDisplayDPI(SDL_GetWindowDisplayIndex(_window), &(dpi.diagonal), &(dpi.horizontal), &(dpi.vertical));
  return dpi;
}

ScreenDPI BasicWindow::GetScreenDPI() const { return _screenDpi; }

float BasicWindow::DPIScale() const {
  return 1.0f;
  // _dpiScale;
}

SDL_DisplayMode BasicWindow::DisplayMode() const {
  SDL_DisplayMode displayMode;
  SDL_GetWindowDisplayMode(_window, &displayMode);
  return displayMode;
}

std::string BasicWindow::DisplayName() const { return SDL_GetDisplayName(SDL_GetWindowDisplayIndex(_window)); }

Uint32 BasicWindow::PixelFormat() const { return SDL_GetWindowPixelFormat(_window); }

std::string BasicWindow::PixelFormatName() const { return SDL_GetPixelFormatName(SDL_GetWindowPixelFormat(_window)); }

SDL_Rect BasicWindow::DisplayBounds() const {
  SDL_Rect bounds;
  SDL_GetDisplayBounds(SDL_GetWindowDisplayIndex(_window), &bounds);
  return bounds;
}

SDL_Rect BasicWindow::DisplayUsableBounds() const {
  SDL_Rect bounds;
  SDL_GetDisplayUsableBounds(SDL_GetWindowDisplayIndex(_window), &bounds);
  return bounds;
}

SDL_Window *BasicWindow::Handle() const { return _window; }

void BasicWindow::ShowWindow() const { SDL_ShowWindow(_window); }

Uint32 BasicWindow::GetID() const { return SDL_GetWindowID(_window); }

void BasicWindow::SetOSXWindowedFullscreen() {
  // int top = 0;
  // int left = 0;
  // int bottom = 0;
  // int right = 0;
  // int result = SDL_GetWindowBordersSize(_window, &top, &left, &bottom, &right);
  // if (result == -1) {
  //   CM_LOGGER_ERROR("Could not fetch window border size: {}", SDL_GetError());
  // } else {
  //   CM_LOGGER_INFO("Window Border Size: ({},{})", right - left, bottom - top);
  // }
  // SDL_SetWindowSize(_window, _usableDisplayBounds.w, _usableDisplayBounds.h - 18);
  // SDL_SetWindowPosition(_window, 0, 0);
  SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}
