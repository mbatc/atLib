#include "atWindowDefinitions.h"

atWindowStyle operator|(const atWindowStyle &a, const atWindowStyle &b) { return atWindowStyle(int64_t(a) | int64_t(b)); }
atWindowStyle operator&(const atWindowStyle &a, const atWindowStyle &b) { return atWindowStyle(int64_t(a) & int64_t(b)); }
atWindowFlags operator|(const atWindowFlags &a, const atWindowFlags &b) { return atWindowFlags(int64_t(a) | int64_t(b)); }
atWindowFlags operator&(const atWindowFlags &a, const atWindowFlags &b) { return atWindowFlags(int64_t(a) & int64_t(b)); }
