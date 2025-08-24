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
    /**
     * @struct ThreadOptionsHelper
     * @brief Encapsulates options for thread creation.
     *
     * Contains parameters for thread start address, stack size, thread ID,
     * suspension state, and auto-join behavior.
     */
    struct ThreadOptionsHelper final
    {
        /// Function pointer type for thread start routine.
        using StartAddressFuncType = unsigned long(__stdcall*)(void*);
        StartAddressFuncType startAddress{nullptr}; ///< Pointer to thread start function.
        void* paramsAddress{nullptr}; ///< Pointer to parameters for the thread function.
        unsigned long long stackSize{0}; ///< Stack size for the thread.
        unsigned long threadId{0}; ///< Thread identifier.
        bool isSuspendedOnStart{false}; ///< If true, thread starts suspended.
        bool enableAutoJoin{false}; ///< If true, thread will auto-join on destruction.
    };

    /**
     * @class Thread
     * @brief Manages a thread and its result or exception.
     *
     * Provides facilities to run a function in a separate thread,
     * retrieve its result, handle exceptions, and manage thread lifecycle.
     */
    class Thread final
    {
        /**
         * @struct ThreadProvaider
         * @brief Internal structure for storing thread result or exception.
         *
         * Uses a shared mutex for thread-safe access and std::expected for value/exception storage.
         */
        struct ThreadProvaider final
        {
            /**
             * @brief Constructs a ThreadProvaider with an exception state.
             * @param except The initial exception state.
             */
            RUNTIME_API ThreadProvaider(const std::unexpected<std::exception_ptr>& except) : provaider(except)
            {
            }

            std::shared_mutex mt; ///< Mutex for synchronizing access.
            std::expected<std::unique_ptr<void, void (*)(void*)>, std::exception_ptr>
                provaider; ///< Value or exception.
        };

    public:
        Thread() = delete;

        /**
         * @brief Constructs a thread and starts execution.
         * @tparam Function Callable type.
         * @tparam Args Argument types.
         * @param func Function to execute in the thread.
         * @param options Thread creation options.
         * @param args Arguments to pass to the function.
         */
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

        /**
         * @brief Destructor. Joins the thread if auto-join is enabled.
         */
        RUNTIME_API ~Thread();

        /**
         * @brief Joins the thread, blocking until it finishes.
         */
        RUNTIME_API void Join();

        /**
         * @brief Starts the thread if it was created suspended.
         */
        RUNTIME_API void Start();

        /**
         * @brief Detaches the thread, allowing it to run independently.
         */
        RUNTIME_API void Detach();

        /**
         * @brief Retrieves the result value from the thread.
         * @tparam T Type of the result.
         * @return Pointer to the result value, or nullptr if not available.
         */
        template <class T>
        RUNTIME_NODISCARD T* GetValue() noexcept
        {
            if (valueProvaider_.provaider.has_value())
                return static_cast<T*>(valueProvaider_.provaider.value().get());
            return nullptr;
        }

        /**
         * @brief Gets the current exception thrown in the thread, if any.
         * @return Exception pointer, or nullptr if no exception.
         */
        RUNTIME_NODISCARD RUNTIME_API std::exception_ptr CurrentException() noexcept;

        /**
         * @brief Checks if the thread has thrown an exception.
         * @return True if an exception was thrown, false otherwise.
         */
        RUNTIME_NODISCARD RUNTIME_API bool HasException() noexcept;

        /**
         * @brief Gets the thread identifier.
         * @return Thread ID.
         */
        RUNTIME_NODISCARD RUNTIME_API unsigned long GetThreadId() const noexcept;

        /**
         * @brief Checks if the thread is joinable.
         * @return True if joinable, false otherwise.
         */
        RUNTIME_NODISCARD RUNTIME_API bool IsJoinable() const noexcept;

        /**
         * @brief Gets the native thread handle.
         * @return Pointer to the thread handle.
         */
        RUNTIME_NODISCARD RUNTIME_API void* GetHandle() const noexcept;

        /**
         * @brief Gets the thread options used for creation.
         * @return Reference to ThreadOptionsHelper.
         */
        RUNTIME_NODISCARD RUNTIME_API const ThreadOptionsHelper& GetOptions() const noexcept;

    private:
        /**
         * @brief Runs a function in the thread.
         * @tparam Function Callable type.
         * @tparam Args Argument types.
         * @param func Function to execute.
         * @param args Arguments to pass to the function.
         */
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
        }


        /**
         * @brief Logs a message when a thread is created.
         * @param threadId The ID of the created thread.
         */
        RUNTIME_API static void MessageOnThreadCreate(int threadId);

        /**
         * @brief Logs a message when thread creation fails.
         * @param handle The thread handle.
         */
        RUNTIME_API static void MessageOnThreadCreateFailed(void* handle);

        /**
         * @brief Logs a message when an exception occurs in the thread.
         */
        RUNTIME_API static void MessageOnThreadException();

        /**
         * @brief Gets the invoker function pointer for the thread.
         * @tparam Function Callable type.
         * @tparam Args Argument types.
         * @return Function pointer to the invoker.
         */
        template <class Function, class... Args>
        RUNTIME_NODISCARD static typename ThreadOptionsHelper::StartAddressFuncType GetInvoker() noexcept
        {
            return &InvokeHelper<Function, Args...>;
        }

        /**
         * @brief Helper function to invoke the thread function and handle result/exception.
         * @tparam Function Callable type.
         * @tparam Args Argument types.
         * @param params Pointer to tuple containing function, provider, and arguments.
         * @return Always returns 0.
         */
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

        ThreadOptionsHelper options_; ///< Thread creation options.
        void* handle_{nullptr}; ///< Native thread handle.
        ThreadProvaider valueProvaider_; ///< Provider for result or exception.
    };
} // namespace Runtime::Parallel
