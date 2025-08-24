#pragma once

#include <expected>
#include <memory>
#include <shared_mutex>
#include <tuple>
#include <type_traits>
#include "Config.h"
#include "SystemHelper.h"

namespace Runtime::Parallel
{
    struct ThreadOptionsHelper final
    {
        using StartAddressFuncType = unsigned long(__stdcall*)(void*);
        StartAddressFuncType startAddress{nullptr};
        void* paramsAddress{nullptr};
        unsigned long long stackSize{0};
        unsigned long threadId{0};
        bool isSuspendedOnStart{false};
        bool enableAutoJoin{false};
    };

    class Thread final
    {
        struct ThreadProvaider final
        {
            ThreadProvaider(const std::unexpected<std::exception_ptr>& except) : provaider(except)
            {
            }
            std::shared_mutex mt;
            std::expected<std::unique_ptr<void, void (*)(void*)>, std::exception_ptr> provaider;
        };

    public:
        Thread() = delete;

        template <class Function, class... Args>
        explicit Thread(Function&& func, const ThreadOptionsHelper& options, Args&&... args) :
            options_(options), valueProvaider_(std::unexpected<std::exception_ptr>{nullptr})
        {
            Run(std::forward<Function>(func), std::forward<Args>(args)...);
        }

        RUNTIME_API Thread(const Thread& other) = delete;
        RUNTIME_API Thread& operator=(const Thread& other) = delete;

        RUNTIME_API Thread(Thread&& other) noexcept = default;
        RUNTIME_API Thread& operator=(Thread&& other) noexcept = default;

        RUNTIME_API ~Thread();

        RUNTIME_API void Join();
        template <class Function, class... Args>
        void Run(Function&& func, Args&&... args)
        {
            using TupledFunction =
                std::tuple<std::decay_t<Function>, std::reference_wrapper<ThreadProvaider>, std::decay_t<Args>...>;
            auto tupledFuncPointer = std::make_unique<TupledFunction>(
                std::forward<Function>(func), std::ref(valueProvaider_), std::forward<Args>(args)...);
            auto invokerPointer = GetInvoker<Function, Args...>();
            options_.startAddress = invokerPointer;
            options_.paramsAddress = static_cast<void*>(tupledFuncPointer.get());
            handle_ = Runtime::System::CreateThread(options_);

            if (!handle_)
            {
                MessageOnThreadCreateFailed(handle_);
                std::terminate();
            }
            MessageOnThreadCreate(options_.threadId);
            tupledFuncPointer.release();
        }
        RUNTIME_API void Start();
        RUNTIME_API void Detach();

        template <class T>
        RUNTIME_NODISCARD T* GetValue() noexcept
        {
            if (valueProvaider_.provaider.has_value())
                return static_cast<T*>(valueProvaider_.provaider.value().get());
            return nullptr;
        }

        RUNTIME_NODISCARD RUNTIME_API std::exception_ptr CurrentException() noexcept;
        RUNTIME_NODISCARD RUNTIME_API bool HasException() noexcept;
        RUNTIME_NODISCARD RUNTIME_API unsigned long GetThreadId() const noexcept;

        RUNTIME_NODISCARD RUNTIME_API bool IsJoinable() const noexcept;
        RUNTIME_NODISCARD RUNTIME_API void* GetHandle() const noexcept;

        RUNTIME_NODISCARD RUNTIME_API const ThreadOptionsHelper& GetOptions() const noexcept;

    private:
        RUNTIME_API static void MessageOnThreadCreate(int threadId);
        RUNTIME_API static void MessageOnThreadCreateFailed(void* handle);
        RUNTIME_API static void MessageOnThreadException();

        template <class Function, class... Args>
        RUNTIME_NODISCARD static typename ThreadOptionsHelper::StartAddressFuncType GetInvoker() noexcept
        {
            return &InvokeHelper<Function, Args...>;
        }

        template <class Function, class... Args>
        RUNTIME_NODISCARD static unsigned long __stdcall InvokeHelper(void* params) noexcept
        {
            using TupledFunction =
                std::tuple<std::decay_t<Function>, std::reference_wrapper<ThreadProvaider>, std::decay_t<Args>...>;
            auto tupleParams = std::unique_ptr<TupledFunction>(static_cast<TupledFunction*>(params));
            try
            {
                if constexpr (std::is_void_v<std::invoke_result_t<Function, Args...>>)
                {
                    std::invoke(std::get<Function>(*tupleParams.get()), std::get<Args>(*tupleParams.get())...);
                }
                else
                {
                    using ReturnType = std::invoke_result_t<Function, Args...>;
                    auto retVal =
                        std::invoke(std::get<Function>(*tupleParams.get()), std::get<Args>(*tupleParams.get())...);
                    ReturnType* retValPtr = new ReturnType(std::move(retVal));
                    auto provaider = std::get<std::reference_wrapper<ThreadProvaider>>(*tupleParams.get());
                    {
                        std::unique_lock<std::shared_mutex> lock{provaider.get().mt};
                        provaider.get().provaider = std::unique_ptr<void, void (*)(void*)>(
                            static_cast<void*>(retValPtr), [](void* p) { delete static_cast<ReturnType*>(p); });
                    }
                }
            }
            catch (...)
            {
                auto provaider = std::get<std::reference_wrapper<ThreadProvaider>>(*tupleParams.get());
                {
                    std::unique_lock<std::shared_mutex> lock{provaider.get().mt};
                    provaider.get().provaider = std::unexpected(std::current_exception());
                }
                MessageOnThreadException();
            }
            return 0;
        }

        ThreadOptionsHelper options_;
        void* handle_{nullptr};
        ThreadProvaider valueProvaider_;
    };
} // namespace Runtime::Parallel
