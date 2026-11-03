#pragma once

// The init of CHydrogigaAttack's subclasses with the AI component's class data: the base init
// (fn_71005b2bf8), the argument cached at +0x60 when it is of the subclass's class-data type, then
// names appended to the string-id array at +0x28, each moved in (the temporary is left holding the
// empty id).

#include "base/UnkCloneRecords.h"
#include "base/reflection/UnkReflCast.h"

void fn_71005b2bf8(char* s, base::reflection::UnkReflObject* a);

struct UnkHydrogigaAttack {
    u8 pad_0x0[0x28];
    base::UnkStrIdVec field_0x28;
    u8 pad_0x50[0x10];
    base::reflection::UnkReflObject* field_0x60;
};

static inline __attribute__((always_inline)) void PushMove(base::UnkStrIdVec* ids, base::global::CStrId&& v) {
    u32 n = ids->count;
    u32 c = ids->capacity;
    if (c <= n && ids->step != 0)
        c = (n / ids->step + 1) * ids->step;
    if ((s32)n >= (s32)c)
        for (;;) {
        }
    ids->Reserve(c);
    __builtin_memcpy(&ids->data[ids->count], &v.data, 8);
    v.data = &g_unk_7101d47108->empty;
    ++ids->count;
}

template <const base::reflection::CClass* (*Class)()>
static inline __attribute__((always_inline)) void InitHydrogigaAttack(UnkHydrogigaAttack* self,
                                                                      base::reflection::UnkReflObject* arg) {
    fn_71005b2bf8(reinterpret_cast<char*>(self), arg);
    if (arg)
        self->field_0x60 = fn_710009d50c(arg->vf_0x0(), Class()) ? arg : nullptr;
}
