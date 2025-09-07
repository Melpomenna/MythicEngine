#include <MacOS/System.h>
#include <signal.h>
#include <exception> 
#include "ThreadHelper.h"

#include <pthread.h>
#include <cstdint>

#include <unistd.h>

namespace Runtime::System::MacOS{


  

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
  #if 0
  void SetPureVirtualCallHandler(void (*pPureCallHandler)()){
        std::set_terminate(pPureCallHandler);
    }
  #endif

  void* CreateThread(Runtime::Parallel::ThreadOptionsHelper& refThreadOptions)
    {
      pthread_attr_t attr;
      pthread_attr_init(&attr);

      if (refThreadOptions.stackSize > 0) {
          pthread_attr_setstacksize(&attr, refThreadOptions.stackSize);
      }

      int res = pthread_create(
          &refThreadOptions.threadId,
          &attr,
          refThreadOptions.startAddress,
          refThreadOptions.paramsAddress
      );

      pthread_attr_destroy(&attr);
      //незнаю что тут вернуть 
      return nullptr;
    }


}