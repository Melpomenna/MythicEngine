#include <MacOS/System.h>
#include <signal.h>


namespace Runtime::System::MacOS{


  using UnhandledHandler = void(*)(int, siginfo_t*, void*);

  void SetUnhandledExceptionHandler(UnhandledHandler handler) {
      struct sigaction sa;
      sa.sa_sigaction = handler;
      sigemptyset(&sa.sa_mask);
      sa.sa_flags = SA_SIGINFO | SA_RESTART; //SA_SIGINFO для своего обработчика, а SA_RESTART - для того, чтобы сигнал не прерывал процессы

      
     /*
        SIGSEGV (ошибка доступа к памяти)
        SIGBUS (ошибка шины/выравнивания)
        SIGILL (недопустимая инструкция)
        SIGFPE (деление на 0 и пр. арифметические)
        SIGABRT (вызван abort())
     */
      sigaction(SIGSEGV, &sa, nullptr);
      sigaction(SIGBUS,  &sa, nullptr);
      sigaction(SIGILL,  &sa, nullptr);
      sigaction(SIGFPE,  &sa, nullptr);
      sigaction(SIGABRT, &sa, nullptr);
  }

}