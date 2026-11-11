#include <new>

#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflObject;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

extern CStrId g_unk_71727c8198;
extern CStrId g_unk_71727c81a0;
extern CStrId g_unk_71727c81a8;
extern CStrId g_unk_71727c83b0;

struct UnkStaticIdSlot_j {
    CStrId id;
    u64 guard;
};
extern UnkStaticIdSlot_j g_unk_71729cdb18 __attribute__((visibility("hidden")));

struct UnkHiddenKinds_j {
    CStrId* data;
    u32 count;
};

struct UnkMinimapOwner_j {
    u8 unk_0x0[0xd642c8];
    UnkHiddenKinds_j hidden_kinds;
};

void fn_7100e9565c(UnkHiddenKinds_j* kinds, const CStrId* kind);
void fn_7100e957a8(UnkMinimapOwner_j* self);
bool fn_71008d2684(UnkReflObject* object);

struct UnkActorDef_j : UnkReflObject {
    u8 unk_0x8[0x90];
    base::global::UnkStringData* field_0x98;
};

struct UnkDoorOwner_j {
    u8 unk_0x0[0x18];
    struct {
        u8 unk_0x0[0x28];
        UnkActorDef_j* field_0x28;
    }* field_0x18;
};

template <const CClass* (*Getter)()>
__attribute__((always_inline)) static inline bool IsA(UnkReflObject* object) {
    return fn_710009d50c(object->vf_0x0(), Getter());
}

bool fn_7100e9c06c(UnkMinimapOwner_j* self, UnkReflObject* object) {
    if (object && IsA<fn_71008cb078>(object)) {
        UnkStaticIdSlot_j& slot = g_unk_71729cdb18;
        if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&slot.guard), __ATOMIC_ACQUIRE) & 1), 0)) {
            if (__cxa_guard_acquire(&slot.guard)) {
                new (&slot.id) CStrId("doorshutter", true);
                fn_7100000250(fn_710000051c, &slot.id, &g_unk_7101a19000);
                __cxa_guard_release(&slot.guard);
            }
        }
        if (fn_71008d2684(object)) {
            fn_7100e9565c(&self->hidden_kinds, &g_unk_71727c8198);
            fn_7100e9565c(&self->hidden_kinds, &g_unk_71727c81a0);
            fn_7100e957a8(self);
        } else {
            UnkActorDef_j* def = reinterpret_cast<UnkDoorOwner_j*>(object)->field_0x18->field_0x28;
            if (!IsA<fn_7100254b14>(def))
                def = nullptr;
            // keeps the cast's null result a real value, as the original reads the field through it
            asm("" : "+r"(def));
            if (def->field_0x98 == slot.id.data) {
                fn_7100e9565c(&self->hidden_kinds, &g_unk_71727c81a8);
                fn_7100e9565c(&self->hidden_kinds, &g_unk_71727c83b0);
                fn_7100e957a8(self);
            }
        }
    }
    UnkHiddenKinds_j* kinds = &self->hidden_kinds;
    CStrId* it = kinds->data;
    for (u64 n = kinds->count; n != 0; --n, ++it)
        if (it->data == g_unk_71727c8198.data)
            return false;
    it = kinds->data;
    for (u64 n = kinds->count; n != 0; --n, ++it)
        if (it->data == g_unk_71727c81a8.data)
            return false;
    return true;
}
