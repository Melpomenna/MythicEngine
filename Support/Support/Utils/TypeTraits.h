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

template <class T> using RemoveReference_t = typename RemoveReference<T>::Type;

template <class T> struct MYTHIC_ENGINE_EXPORT AddReference {
  using LValue = RemoveReference_t<T>;
  using RValue = RemoveReference_t<T>;
};

template <class T> struct MYTHIC_ENGINE_EXPORT AddReference<T &> {
  using LValue = T &;
  using RValue = T &&;
};

template <class T>
using AddRValueReference_t = typename AddReference<T>::RValue;

template <class T>
using AddLValueReference_t = typename AddReference<T>::LValue;

template <class T>
AddRValueReference_t<T> MYTHIC_ENGINE_WIN_API Declval() noexcept;

template <class, class> struct MYTHIC_ENGINE_EXPORT IsSame {
  static __CONSTEXPR__ bool Value = false;
};

template <class T> struct MYTHIC_ENGINE_EXPORT IsSame<T, T> {
  static __CONSTEXPR__ bool Value = true;
};

template <class T, class Q>
__INLINE__ __CONSTEXPR__ bool IsSame_v = IsSame<T, Q>::Value;

struct MYTHIC_ENGINE_EXPORT TrueType {
  static __CONSTEXPR__ bool Type = true;
};

struct MYTHIC_ENGINE_EXPORT FalseType {
  static __CONSTEXPR__ bool Type = false;
};

template <bool Condition, class L, class R>
struct MYTHIC_ENGINE_EXPORT EnableIf {};

template <class L, class R> struct MYTHIC_ENGINE_EXPORT EnableIf<true, L, R> {
  using Type = L;
};

template <class L, class R> struct MYTHIC_ENGINE_EXPORT EnableIf<false, L, R> {
  using Type = R;
};

template <bool Condition, class L, class R>
using EnableIf_t = typename EnableIf<Condition, L, R>::Type;

template <class T> struct MYTHIC_ENGINE_EXPORT IsNoexceptMoveAssign {
private:
  static void detect(...);
  template <class Q>
  static EnableIf_t<noexcept(Declval<Q &>() == Declval<Q>()), Q, void>
  detect(const Q &);

public:
  static __CONSTEXPR__ bool Value = IsSame_v<T, decltype(detect(Declval<T>()))>;
};

template <class T> struct MYTHIC_ENGINE_EXPORT HasCopyOperator {
public:
private:
  static void detect(...);
  template <class Q>
  static EnableIf_t<Declval<Q &>() == Declval<Q &>(), Q, void>
  detect(const Q &);

public:
  static __CONSTEXPR__ bool Value = IsSame_v<T, decltype(detect(Declval<T>()))>;
};

template <class T>
__INLINE__ __CONSTEXPR__ bool IsNoexceptMoveAssign_v =
    IsNoexceptMoveAssign<T>::Value;

template <class T>
__INLINE__ __CONSTEXPR__ bool HasCopyOperator_v = HasCopyOperator<T>::Value;

template <class> __INLINE__ __CONSTEXPR__ bool IsLValueReference_v = false;

template <class T>
__INLINE__ __CONSTEXPR__ bool IsLValueReference_v<T &> = true;

template <class T>
__NODISCARD__ __CONSTEXPR__ RemoveReference_t<T> &&Move(T &&arg) noexcept {
  return static_cast<RemoveReference_t<T> &&>(arg);
}

template <class T>
__NODISCARD__ __CONSTEXPR__ decltype(auto) MoveIfNoexcept(T &&arg) noexcept {
  return static_cast<
      EnableIf_t<IsNoexceptMoveAssign_v<T> && HasCopyOperator_v<T>,
                 RemoveReference_t<T> &, RemoveReference_t<T> &&>>(arg);
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