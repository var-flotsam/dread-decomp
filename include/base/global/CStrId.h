#pragma once

// base::global::CStrId: a pooled, reference-counted string id. The name comes from
// reflection: fn_7100009658 (refl_GetType<base::global::CStrId>) describes the values built by
// fn_71000003d4. Reflection descriptors take their type name as one. Members keep
// offset-based names.

#include "types.h"

namespace base::global {

struct UnkStringData {
    u8 unk_0x0[0x8];
    // A union member: Clang gives union accesses no type-based alias info, so the decrement
    // forces the reloads the original has (CStrId::data and ref_count after `--ref_count`).
    // A union member: Clang gives union accesses no type-based alias info, so the decrement in
    // CStrId's move-assignment forces the reloads the original has.
    union {
        s32 ref_count;
    };
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

// What CStrId and UnkStrId share: the pointer and the constructor (fn_71000003d4). Its
// destructor is trivial; each id type releases the string in its own destructor.
class UnkStrIdBase {
public:
    UnkStringData* data;

    UnkStrIdBase(const char* str, bool flag);  // fn_71000003d4
    UnkStrIdBase(const UnkStrIdBase& other) = default;

protected:
    // Inlined into the move-assignments. Unlike the destructors, it decrements and tests in
    // two steps (the original reloads `data` and the count after the decrement). The
    // moved-from id gets the empty string of a freshly loaded pool.
    void MoveFrom(UnkStrIdBase& other) {
        UnkStringPool* pool = g_unk_7101d47108;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
        }
        data = other.data;
        other.data = &g_unk_7101d47108->empty;
    }
};

class CStrId : public UnkStrIdBase {
public:
    using UnkStrIdBase::UnkStrIdBase;
    CStrId(const CStrId& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    CStrId& operator=(CStrId&& other) {
        MoveFrom(other);
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

// The id type of a reflected function's name (UnkReflFunction+0x30). It is built by the same
// constructor as CStrId but destroyed differently: in fn_71004f01a8 the CStrId passed to
// fn_7100097550 is released by CStrId's destructor (empty id from the pool reloaded after a
// release), while the moved-from temporary assigned to the name folds away only if its
// destructor ends with `data = &pool->empty` on the pool it loaded first. The getter and
// setter initialisers (fn_71004f01a8, fn_7100a19530) match that way. Their release branch
// is dead, so the release part of this destructor is unverified.
class UnkStrId : public UnkStrIdBase {
public:
    using UnkStrIdBase::UnkStrIdBase;
    UnkStrId& operator=(UnkStrId&& other) {
        MoveFrom(other);
        return *this;
    }
    ~UnkStrId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = &pool->empty;
        }
    }
};

}  // namespace base::global
