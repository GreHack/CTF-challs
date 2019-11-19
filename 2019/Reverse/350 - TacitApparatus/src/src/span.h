#pragma once

#include <stddef.h> // size_t

template<typename T>
struct Span
{
  T* ptr;
  size_t len;

  constexpr Span() = default;
  constexpr Span(T* ptr_, size_t len_) : ptr(ptr_), len(len_) {}

  template<size_t N>
  constexpr Span(T (& tab)[N]) : ptr(tab), len(N) {}

  T* begin() { return ptr; }
  T* end() { return ptr + len; }

  constexpr bool isEqual(Span<T> other) const
  {
    if(other.len != len)
      return false;

    for(size_t i = 0; i < len; ++i)
      if(ptr[i] != other.ptr[i])
        return false;

    return true;
  }
};

