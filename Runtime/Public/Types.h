#pragma once

#include <cstdint>

using Int8 = std::int8_t;
using Int16 = std::int16_t;
using Int32 = std::int32_t;
using Int64 = std::int64_t;
using UI8 = std::uint8_t;
using UI16 = std::uint16_t;
using UI32 = std::uint32_t;
using UI64 = std::uint64_t;
using Float = float;
using Double = double;
using Coord = Float;
using Angle = Float;
using Radian = Float;
using Symbol = char;
using USymbol = unsigned char;
using Mem = char;

#if defined(_WIN32) || defined(_WIN64)
using Handle = void*;
#else
using Handle = Int32;
#endif

#if !defined(RUNTIME_HANDLE_INIT)
#if defined(_WIN32) || defined(_WIN64)
#define RUNTIME_HANDLE_INIT nullptr
#else
#define RUNTIME_HANDLE_INIT 0
#endif
#endif

struct Point
{
    Coord x_{0.0};
    Coord y_{0.0};
    Coord z_{0.0};
};


struct Vec
{
    Point begin_;
    Point end_;
};
