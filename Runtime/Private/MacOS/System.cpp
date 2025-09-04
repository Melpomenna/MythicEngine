#include <MacOS/System.h>
#include <signal.h>
#include <string.h>

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

  void GenerateDump(void* pExceptionInfo)
  {
    crashpad::CrashpadInfo::GetCrashpadInfo()->set_simple_annotations(&global_annotations);

    global_annotations.SetKeyValue("PID", std::to_string(getpid()).c_str());
    global_annotations.SetKeyValue("TIME", GetCurrentTimeString());
    global_annotations.SetKeyValue("EXCEPTION", "Здесь как то нужно указать тип исключения");

    
    CRASHPAD_SIMULATE_CRASH();
  }

  // эта функция должна вызваться только раз в начале начал))
  void StartClientForDump()
  {
    //TODO: указать правильные пути
    base::FilePath handler("../../Public/crashpad/out/Default/crashpad_handler");
    base::FilePath db("crashpad_db");
    base::FilePath metrics("crashpad_metrics");

    std::map<std::string, std::string> annotations;
    annotations["name"] = "MythicEngine";
    annotations["version"] = "1.0.0";

    bool success = client.StartHandler(
        handler,
        db,
        metrics,
        "", // тут можно указать свой сервак, но я не знаю какой
        annotations, //для доп инфы, я указал просто имя проекта и типо версию
        {}, // тут можно указывать разные флаги для гибкости (указал их ниже)
        true, //restartable если хендлер крашит, то он перезапуститься 
        false //asynchronous_start это для многопоточности 
    );

    /*
    --database=<path> → путь до базы дампов
    --metrics-dir=<path> → путь до метрик
    --url=<server_url> → куда отправлять отчёты
    --no-upload → чтобы не отправлять дампы на сервер
    --no-rate-limit → снять ограничение на частоту отправки
    --monitor-self → включить мониторинг самого handler’а
    */
  }

  static char *GetCurrentTimeString()
  {
      time_t now = time(0);
      char* dt = ctime(&now);
      return dt;
  }

  

}