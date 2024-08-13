#include <Application/Window.h>
#include <Support/Utils/Utils.h>

namespace {
IN constexpr LONG WIDTH = 800;
IN constexpr LONG HEIGHT = 600;
IN constexpr LPSTR NAME = LPSTR("Particle System Engine");
} // namespace

int CALLBACK WinMain(IN __UNUSED__ HINSTANCE hInstance,
                     IN __UNUSED__ HINSTANCE hPrevInstance,
                     IN __UNUSED__ LPSTR lpCmdLine,
                     IN __UNUSED__ int nCmdShow) {
  using Application = MythicEngine::engine::MainWindow;
  using ApplicationPtr = std::unique_ptr<Application>;
  ApplicationPtr window = std::make_unique<Application>(WIDTH, HEIGHT, NAME);
  return static_cast<int>(window->RunLoop());
}
