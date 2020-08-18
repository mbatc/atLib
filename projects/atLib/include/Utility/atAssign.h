#ifndef atAssign_h__
#define atAssign_h__

// To implement casts between different types that do not implement implicit casts
// internally atAssign should be specialized and used instead.
//
// This makes it clear that the cast is intentional, and the implicit cast is not
// unintentional.
//
// The default implementation just tries to cast directly
template<typename T, typename T2> T& atAssign(T &lhs, const T2 &rhs) { lhs = (T)rhs; return lhs; }
template<typename T, typename T2> T atAssign(const T2 &rhs) { T lhs; return atAssign<T, T2>(lhs, rhs); }

#endif // atAssign_h__
