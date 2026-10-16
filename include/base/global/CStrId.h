#pragma once

// base::global::CStrId: a pooled, reference-counted string id. The name comes from
// reflection: fn_7100009658 (refl_GetType<base::global::CStrId>) describes the values built by
// fn_71000003d4. Reflection descriptors take their type name as one. Members keep
// offset-based names.

#include "types.h"

namespace base::global {

struct UnkStringData {
    u8 unk_0x0[0x8];
    s32 ref_count;
};

struct UnkStringPool {
    u8 unk_0x0[0x10];
    UnkStringData empty;
};

}  // namespace base::global

// The global string pool (reached through the GOT).
extern base::global::UnkStringPool* g_unk_7101d47108;
void fn_71000803a4(base::global::UnkStringPool* pool, base::global::UnkStringData* data);

namespace base::global {

class CStrId {
public:
    UnkStringData* data;

    CStrId(const char* str, bool flag);  // fn_71000003d4
    CStrId(const CStrId& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    // Not yet matched where it is inlined (see docs/HANDOFF.md: getter initialisers).
    CStrId& operator=(CStrId&& other) {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
        }
        data = other.data;
        other.data = &g_unk_7101d47108->empty;
        return *this;
    }
    ~CStrId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

}  // namespace base::global
