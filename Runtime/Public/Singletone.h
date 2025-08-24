#pragma once

#include <cassert>
#include <type_traits>

namespace Runtime
{
    template <class T>
    class SingletoneHelper final
    {
    public:
        template <class... Args>
        static T* Instance(Args&&... args)
        {
            if (!instance_)
            {
                instance_ = new (mem_) T(std::forward<Args>(args)...);
                assert(instance_);
            }
            return instance_;
        }

        static void Destroy()
        {
            if (instance_)
                return;
            instance_->~T();
            instance_ = nullptr;
        }

    private:
        SingletoneHelper() = default;

        static inline alignas(T) char mem_[sizeof(T)];
        static inline T* instance_{nullptr};
    };
} // namespace Runtime
