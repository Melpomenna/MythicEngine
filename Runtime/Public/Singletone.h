#pragma once

#include <memory>
#include <type_traits>
#include "Types.h"

/**
 * @file Singletone.h
 * @brief Provides a helper class for implementing the singleton pattern.
 */

namespace Runtime
{
    /**
     * @brief Helper class to implement the singleton pattern for any type.
     *
     * This class ensures that only one instance of type @c T exists.
     * The instance is constructed in-place using placement new and destroyed manually.
     *
     * @tparam T The type to be managed as a singleton.
     */
    template <class T>
    class SingletoneHelper final
    {
    public:
        /**
         * @brief Returns the singleton instance of type @c T, constructing it if necessary.
         *
         * If the instance does not exist, it is constructed in-place using the provided arguments.
         *
         * @tparam Args Types of constructor arguments.
         * @param args Arguments to forward to the constructor of @c T.
         * @return Pointer to the singleton instance.
         */
        template <class... Args>
            requires std::is_default_constructible_v<T> || std::is_constructible_v<T, Args...>
        static T* Instance(Args&&... args)
        {
            if (!instance_)
            {
                instance_ = reinterpret_cast<T*>(mem_);
                std::construct_at(instance_, std::forward<Args>(args)...);
            }
            return instance_;
        }

        /**
         * @brief Destroys the singleton instance if it exists.
         *
         * Calls the destructor of @c T and resets the instance pointer.
         */
        static void Destroy()
        {
            if (!instance_)
                return;
            instance_->~T();
            instance_ = nullptr;
        }

    private:
        /**
         * @brief Default constructor (deleted).
         *
         * This class should not be instantiated.
         */
        SingletoneHelper() = default;

        /**
         * @brief Aligned storage for the singleton instance.
         */
        static alignas(T) char mem_[sizeof(T)];

        /**
         * @brief Pointer to the singleton instance.
         */
        static T* instance_;
    };

    // Definition outside the class, with alignas(T) attribute.
    template <class T>
    alignas(T) Mem SingletoneHelper<T>::mem_[sizeof(T)]{};

    template <class T>
    T* SingletoneHelper<T>::instance_ = nullptr;
} // namespace Runtime
