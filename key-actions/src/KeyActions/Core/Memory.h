#pragma once

#include "Lumina/Core/Base.h"

namespace KeyActions
{
    template<typename T>
    using Ref = Lumina::Ref<T>;
    template<typename T, typename... Args>
    inline Ref<T> CreateRef(Args&&... args)
    {
        return Lumina::CreateRef<T>(std::forward<Args>(args)...);
    }
}