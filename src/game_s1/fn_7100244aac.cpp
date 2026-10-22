// Equality of two holders of a string id and an array of weak handles, held either in place or
// by reference (kind 2), as fn_7100a74690 (src/game_s4/fn_71009c2f58.cpp) does for an array of
// pointers: the same id and handles to the same targets: fn_7100244aac, fn_71002443e0.
#include "base/global/CStrId.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkId {
    UnkStringData* data;
};

struct UnkHandleArray {
    UnkWeakHandle** data;
    u32 count;
};

struct UnkIdHandles {
    UnkId id;
    UnkHandleArray handles;
};

struct UnkIdHandlesRef {
    UnkId* id;
    UnkHandleArray* handles;
};

struct UnkHandlesHolder {
    alignas(8) u8 storage[0x30];
    u32 kind;

    UnkIdHandles& Value() { return *reinterpret_cast<UnkIdHandles*>(storage); }
    UnkIdHandlesRef& Ref() { return *reinterpret_cast<UnkIdHandlesRef*>(storage); }

    UnkId& Id() { return kind == 2 ? *Ref().id : Value().id; }
    UnkHandleArray& Handles() { return kind == 2 ? *Ref().handles : Value().handles; }
};

static inline UnkWeakTarget* Target(UnkWeakHandle* handle) { return handle ? handle->target : nullptr; }

static inline __attribute__((always_inline)) bool Equal(UnkHandlesHolder* self, UnkHandlesHolder* other) {
    UnkId& x = self->Id();
    UnkId& y = other->Id();
    if (x.data != y.data)
        return false;
    UnkHandleArray& a = self->Handles();
    UnkHandleArray& b = other->Handles();
    if (a.count != b.count)
        return false;
    UnkWeakHandle** p = a.data;
    UnkWeakHandle** q = b.data;
    for (u64 n = a.count; n != 0; --n, ++q, ++p) {
        UnkWeakTarget* t = Target(*p);
        if (Target(*q) != t)
            return false;
    }
    return true;
}

bool fn_7100244aac(UnkHandlesHolder* self, UnkHandlesHolder* other) { return Equal(self, other); }

bool fn_71002443e0(UnkHandlesHolder* self, UnkHandlesHolder* other) { return Equal(self, other); }
