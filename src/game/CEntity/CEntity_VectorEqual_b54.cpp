// CEntity's unit (tools/tu_map.py unit 2523): equality of two base::UnkVectors of 0x40-byte lazy links,
// a copy of fn_710113192c (src/game/CSpawnPointComponent/CSpawnPointComponent_c3_1.cpp) resolving
// through this unit's fn_7100353be8 and fn_710031d208. The signature is the one
// src/game_s1/fn_71000d4d50.cpp declares (the 0x28-byte operands are the vectors). Names and
// layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "game/UnkTU437.h"
#include "types.h"

using UnkRef = base::UnkRefPtrMember;

struct UnkLinkHandle_b54 {
    u8 pad_0x0[0x10];
    void* target;
};

struct UnkEntityLink_b54 {
    u8 pad_0x0[0x8];
    u8 key[0x28];
    u32 state;
    u32 pad_0x34;
    UnkLinkHandle_b54* handle;

    void* Get() {
        if (state == 1) {
            {
                UnkRef ref = fn_7100353be8(key);
                fn_710031d208(reinterpret_cast<base::UnkRefHandle*>(&handle), reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
        return handle ? handle->target : nullptr;
    }
};
static_assert(sizeof(UnkEntityLink_b54) == 0x40, "UnkEntityLink_b54 layout");

struct UnkCompared_0x28b {
    u64 words[5];
};

bool fn_7100d883f0(const UnkCompared_0x28b* a, const UnkCompared_0x28b* b) {
    auto* self = reinterpret_cast<base::UnkVector<UnkEntityLink_b54>*>(const_cast<UnkCompared_0x28b*>(a));
    auto* other = reinterpret_cast<base::UnkVector<UnkEntityLink_b54>*>(const_cast<UnkCompared_0x28b*>(b));
    if (self->count != other->count)
        return false;
    for (u64 i = 0; i < self->count; ++i) {
        UnkEntityLink_b54& x = self->data[i];
        UnkEntityLink_b54& y = other->data[i];
        void* target = x.Get();
        if (y.Get() != target)
            return false;
    }
    return true;
}
