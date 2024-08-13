#pragma once

#include <Support/Utils/NonCopy.h>
#include <Support/Utils/NonMove.h>
#include <Support/Utils/Utils.h>

namespace MythicEngine::support::utils {
template <bool EnableDefaultCopyConstruct, bool EnableDefaultCopyAssign>
struct MYTHIC_ENGINE_EXPORT CopyConstruct {};

template <> struct MYTHIC_ENGINE_EXPORT CopyConstruct<true, true> {
  CopyConstruct() = default;
  virtual ~CopyConstruct() = default;

  CopyConstruct(CopyConstruct &&) noexcept = default;
  CopyConstruct &operator=(CopyConstruct &&) noexcept = default;

  CopyConstruct(const CopyConstruct &) = default;
  CopyConstruct &operator=(const CopyConstruct &) = default;
};

template <> struct MYTHIC_ENGINE_EXPORT CopyConstruct<false, true> {
  CopyConstruct() = default;
  virtual ~CopyConstruct() = default;

  CopyConstruct(CopyConstruct &&) noexcept = default;
  CopyConstruct &operator=(CopyConstruct &&) noexcept = default;

  CopyConstruct(const CopyConstruct &) = delete;
  CopyConstruct &operator=(const CopyConstruct &) = default;
};

template <> struct MYTHIC_ENGINE_EXPORT CopyConstruct<true, false> {
  CopyConstruct() = default;
  virtual ~CopyConstruct() = default;

  CopyConstruct(CopyConstruct &&) noexcept = default;
  CopyConstruct &operator=(CopyConstruct &&) noexcept = default;

  CopyConstruct(const CopyConstruct &) = default;
  CopyConstruct &operator=(const CopyConstruct &) = delete;
};

template <> struct MYTHIC_ENGINE_EXPORT CopyConstruct<false, false> : NonCopy {
  CopyConstruct() = default;
  ~CopyConstruct() override = default;

  CopyConstruct(CopyConstruct &&) noexcept = default;
  CopyConstruct &operator=(CopyConstruct &&) noexcept = default;
};

template <bool EnableDefaultMoveConstruct, bool EnableDefaultMoveAssign>
struct MYTHIC_ENGINE_EXPORT MoveConstruct {};

template <> struct MYTHIC_ENGINE_EXPORT MoveConstruct<true, true> {
  MoveConstruct() = default;
  virtual ~MoveConstruct() = default;

  MoveConstruct(MoveConstruct &&) noexcept = default;
  MoveConstruct &operator=(MoveConstruct &&) noexcept = default;

  MoveConstruct(const MoveConstruct &) = default;
  MoveConstruct &operator=(const MoveConstruct &) = default;
};

template <> struct MYTHIC_ENGINE_EXPORT MoveConstruct<false, true> {
  MoveConstruct() = default;
  virtual ~MoveConstruct() = default;

  MoveConstruct(MoveConstruct &&) noexcept = delete;
  MoveConstruct &operator=(MoveConstruct &&) noexcept = default;

  MoveConstruct(const MoveConstruct &) = default;
  MoveConstruct &operator=(const MoveConstruct &) = default;
};

template <> struct MYTHIC_ENGINE_EXPORT MoveConstruct<true, false> {
  MoveConstruct() = default;
  virtual ~MoveConstruct() = default;

  MoveConstruct(MoveConstruct &&) noexcept = default;
  MoveConstruct &operator=(MoveConstruct &&) noexcept = delete;

  MoveConstruct(const MoveConstruct &) = default;
  MoveConstruct &operator=(const MoveConstruct &) = default;
};

template <> struct MYTHIC_ENGINE_EXPORT MoveConstruct<false, false> : NonMove {
  MoveConstruct() = default;
  ~MoveConstruct() override = default;

  MoveConstruct(const MoveConstruct &) = default;
  MoveConstruct &operator=(const MoveConstruct &) noexcept = default;
};

template <bool EnableDefaultCopyConstruct, bool EnableDefaultMoveConstruct,
          bool EnableDefaultCopyAssign, bool EnableDefaultMoveAssigne>
struct MYTHIC_ENGINE_EXPORT EnableDefaultConstructable
    : CopyConstruct<EnableDefaultCopyConstruct, EnableDefaultCopyAssign>,
      MoveConstruct<EnableDefaultMoveConstruct, EnableDefaultMoveAssigne> {
  EnableDefaultConstructable() = default;
  ~EnableDefaultConstructable() override = default;
};

template <class T> struct MYTHIC_ENGINE_EXPORT RemoveReference final {
  using Type = T;
  using CostTrueType = const T;
};

template <class T> struct MYTHIC_ENGINE_EXPORT RemoveReference<T &> final {
  using Type = T;
  using CostTrueType = const T &;
};

template <class T> struct MYTHIC_ENGINE_EXPORT RemoveReference<T &&> final {
  using Type = T;
  using CostTrueType = const T &&;
};

template <class> __INLINE__ __CONSTEXPR__ bool IsLValueReference_v = false;

template <class T>
__INLINE__ __CONSTEXPR__ bool IsLValueReference_v<T &> = true;

template <class T> using RemoveReference_t = typename RemoveReference<T>::Type;

template <class T>
__NODISCARD__ __CONSTEXPR__ RemoveReference_t<T> &&Move(T &&arg) noexcept {
  return static_cast<RemoveReference_t<T> &&>(arg);
}

template <class T>
__NODISCARD__ __CONSTEXPR__ T &&MYTHIC_ENGINE_WIN_API
Forward(RemoveReference_t<T> &arg) noexcept {
  return static_cast<T &&>(arg);
}

template <class T>
__NODISCARD__ __CONSTEXPR__ T &&MYTHIC_ENGINE_WIN_API
Forward(RemoveReference_t<T> &&arg) noexcept {
  static_assert(!IsLValueReference_v<T>, "Bad forward Call");
  return static_cast<T &&>(arg);
}

template <class T>
concept LessComparable = requires(T value) {
  { value < T{} };
};

template <class T>
concept LessEqualComparable = requires(T value) {
  { value <= T{} };
};

template <class T>
concept GreaterComparable = requires(T value) {
  { value > T{} };
};

template <class T>
concept GreaterEqualComparable = requires(T value) {
  { value >= T{} };
};

template <class T>
concept EqualComparable = requires(T value) {
  { value == T{} };
};

template <class T>
concept NotEqualComparable = requires(T value) {
  { value != T{} };
};

template <class T>
concept Comparable = requires(T value) {
  { value < T{} };
  { value <= T{} };
  { value > T{} };
  { value >= T{} };
  { value == T{} };
  { value != T{} };
};

} // namespace MythicEngine::support::utils