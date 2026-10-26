#pragma once

// base::global::CName: a name kept as its 64-bit hash (fn_7100001570, the hash of `length`
// bytes). The name comes from reflection (fn_710000fd7c describes it); reflected default
// values build one from a literal.

#include "base/global/CStrId.h"
#include "types.h"

namespace base::global {

// Tag for a CName built from its hash (reflected default values store the hash directly).
struct UnkNameHash {};

struct CName {
    u64 hash;

    explicit CName(const char* str) : hash(fn_7100001570(str, __builtin_strlen(str))) {}
    CName(UnkNameHash, u64 h) : hash(h) {}
};
static_assert(sizeof(CName) == 0x8, "CName layout");

}  // namespace base::global
