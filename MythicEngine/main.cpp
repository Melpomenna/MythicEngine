#include <Application/Window.h>
#include <Core/Memory/GarbageCollector.h>
#include <Core/Memory/UniquePtr.h>
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
  int result = 0;
  {
    using Application = MythicEngine::engine::MainWindow;
    using ApplicationPtr = MythicEngine::core::memory::UniquePtr<Application>;
    ApplicationPtr window = MythicEngine::core::memory::MakeUnique<Application>(
        WIDTH, HEIGHT, NAME);
    result = static_cast<int>(window->RunLoop());
  }
  MythicEngine::core::memory::GarbageCollector::Release();
  return result;
}
