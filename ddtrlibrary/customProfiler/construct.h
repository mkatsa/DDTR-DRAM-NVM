#ifndef DDT_UTIL_HPP
#define DDT_UTIL_HPP
#include <new>
#include "bintraits.h"
#include "var.h"

template <typename Tp>
inline void Constructor(Tp * p) {
  ::new(static_cast<void*>(p)) Tp();
}

template <typename T, int id>
inline void Constructor(Tracing::var<T, Tracing::ddtr_traits<id> > & p) {
  ::new(static_cast<void*>(p.getData())) T();
}
/*
template <typename Tp, typename Tval>
inline void Constructor(Tp * p, const Tval* value) {
  ::new(static_cast<void*>(p)) Tp(*value);
}
*/
template <typename Tp, typename Tval>
inline void Constructor(Tp * p, const Tval& value) {
  ::new(static_cast<void*>(p)) Tp(value);
}


template <typename T, typename Tval, int id>
inline void Constructor(Tracing::var<T, Tracing::ddtr_traits<id> > & p, const Tval& value) {
  ::new(static_cast<void*>(p.getData())) T(value);
}

template <typename Tp>
inline void Destructor(Tp * p) {
  p->~Tp();
}

template <typename T, int id>
void Destructor(Tracing::var<T, Tracing::ddtr_traits<id> > & p) {
    p.getData().~T();
}

template <typename Iter>
inline void Destructor(Iter pstart, Iter pend) {
  while (pstart != pend) {
    Destructor(&*pstart);
    ++pstart;
  }
}

/*
template <typename Iter>
inline void Destructor2(Iter pstart, Iter pend) {
	while (pstart != pend) {
		Destructor(&*pstart.getData());
		++pstart;
	}
}
*/

#endif
