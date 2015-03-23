#pragma once

/*
The SmallVector class is taken from LLVM under the following license.

==============================================================================
LLVM Release License
==============================================================================
University of Illinois/NCSA
Open Source License

Copyright (c) 2003-2010 University of Illinois at Urbana-Champaign.
All rights reserved.

Developed by:

    LLVM Team
    University of Illinois at Urbana-Champaign
    http://llvm.org

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal with
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimers.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimers in the
      documentation and/or other materials provided with the distribution.

    * Neither the names of the LLVM Team, University of Illinois at
      Urbana-Champaign, nor the names of its contributors may be used to
      endorse or promote products derived from this Software without specific
      prior written permission.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE
SOFTWARE.
*/


#include "detail/SmallVectorBase.h"


namespace base {


/**
 * SmallVector - This is a 'vector' (really, a variable-sized array), optimized
 * for the case when the array is small.  It contains some number of elements
 * in-place, which allows it to avoid heap allocation when the actual number of
 * elements is below that threshold.  This allows normal "small" cases to be
 * fast without losing generality for large inputs.
 *
 * Note that this does not attempt to be exception safe.
 */
template <typename T, unsigned N>
class SmallVector : public detail::SmallVectorImpl<T>
{

  /// Storage - Inline space for elements which aren't stored in the base class.
  detail::SmallVectorStorage<T, N> Storage;

public:

  SmallVector() : detail::SmallVectorImpl<T>(N) {
  }

  SmallVector(std::initializer_list<T> a_initializerList)
   : detail::SmallVectorImpl<T>(N)
  {
      this->append(a_initializerList.begin(), a_initializerList.end());
  }

  explicit SmallVector(unsigned Size, const T &Value = T())
    : detail::SmallVectorImpl<T>(N) {
    this->assign(Size, Value);
  }

  template<typename ItTy>
  SmallVector(ItTy S, ItTy E) : detail::SmallVectorImpl<T>(N) {
    this->append(S, E);
  }
/*
  template <typename RangeTy>
  explicit SmallVector(const iterator_range<RangeTy> R)
      : detail::SmallVectorImpl<T>(N) {
    this->append(R.begin(), R.end());
  }
*/
  SmallVector(const SmallVector &RHS) : detail::SmallVectorImpl<T>(N) {
    if (!RHS.empty())
      detail::SmallVectorImpl<T>::operator=(RHS);
  }

  const SmallVector &operator=(const SmallVector &RHS) {
    detail::SmallVectorImpl<T>::operator=(RHS);
    return *this;
  }

  SmallVector(SmallVector &&RHS) : detail::SmallVectorImpl<T>(N) {
    if (!RHS.empty())
      detail::SmallVectorImpl<T>::operator=(::std::move(RHS));
  }

  const SmallVector &operator=(SmallVector &&RHS) {
    detail::SmallVectorImpl<T>::operator=(::std::move(RHS));
    return *this;
  }

};


template<typename T, unsigned N>
static inline size_t capacity_in_bytes(const SmallVector<T, N> &X) {
  return X.capacity_in_bytes();
}

template<typename T, unsigned N>
typename SmallVector<T, N>::iterator
begin(SmallVector<T, N>& a_vector)
{
    return a_vector.begin();
}

template<typename T, unsigned N>
typename SmallVector<T, N>::const_iterator
begin(const SmallVector<T, N>& a_vector)
{
    return a_vector.begin();
}

template<typename T, unsigned N>
typename SmallVector<T, N>::iterator
end(SmallVector<T, N>& a_vector)
{
    return a_vector.end();
}

template<typename T, unsigned N>
typename SmallVector<T, N>::const_iterator
end(const SmallVector<T, N>& a_vector)
{
    return a_vector.end();
}

} // namespace base


namespace std {
  /// Implement std::swap in terms of SmallVector swap.
  template<typename T>
  inline void
  swap(base::detail::SmallVectorImpl<T> &LHS, base::detail::SmallVectorImpl<T> &RHS) {
    LHS.swap(RHS);
  }

  /// Implement std::swap in terms of SmallVector swap.
  template<typename T, unsigned N>
  inline void
  swap(base::SmallVector<T, N> &LHS, base::SmallVector<T, N> &RHS) {
    LHS.swap(RHS);
  }
}

