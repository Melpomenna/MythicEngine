#include <MacOS/System.h>
#include <signal.h>
#include <exception> 
#include "ThreadHelper.h"
#include "LogWrapper.h"

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

  void JoinThread(void* thread)
  {
    pthread_t* t = static_cast<pthread_t*>(thread);
    pthread_join(*t, nullptr);
    delete t;                  
  }
  void DetachThread(void* thread)
  {
    pthread_t* t = static_cast<pthread_t*>(thread);
    pthread_detach(*t);
    delete t;
  }

void YieldThread()
{
  if (sched_yield() != 0)
  {
      RUNTIME_LOG_CRITICAL(LogHelper::FileLoggerName, StaticString::FailedToSwitchThreadMessage);
      RUNTIME_CONSOLE_LOG_CRITICAL(StaticString::FailedToSwitchThreadMessage);
  }
}

unsigned long GetCurrentThreadId()
{
  uint64_t tid;
  pthread_threadid_np(nullptr, &tid); // nullptr = текущий поток
  return static_cast<unsigned long>(tid);
}

void YieldCurrentProcessor()
{
  #if defined(__x86_64__) || defined(__i386__)
    __builtin_ia32_pause(); 
  #elif defined(__aarch64__) || defined(__arm__)
    __builtin_arm_yield();  
  #endif
}


}