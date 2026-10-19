#pragma once

// base::global::CName: a name kept as its 64-bit hash (fn_7100001570, the hash of `length`
// bytes). The name comes from reflection (fn_710000fd7c describes it); reflected default
// values build one from a literal.

#include "types.h"

u64 fn_7100001570(const char* str, u64 length);

namespace base::global {

struct CName {
    u64 hash;

    explicit CName(const char* str) : hash(fn_7100001570(str, __builtin_strlen(str))) {}
};
static_assert(sizeof(CName) == 0x8, "CName layout");

}  // namespace base::global
