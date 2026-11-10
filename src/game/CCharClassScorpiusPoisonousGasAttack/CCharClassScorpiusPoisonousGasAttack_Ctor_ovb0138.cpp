// CCharClassScorpiusPoisonousGasAttack's unit (tools/tu_map.py): the attack's constructor, one of the
// members CCharClassScorpiusAIComponent's constructor builds (its +0xb08, fn_7100690498 in
// src/game/CCharClassScorpiusAIComponent/CCharClassScorpiusAIComponent_b40.cpp): the base attack's
// constructor (fn_710067dfdc), the vtable, two empty string ids at +0xf8 and +0x100 (creating the pool
// first when there is none), then the attack's name, "PoisonousGas", at +0xc8. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;
using base::global::UnkStringData;

extern void* g_unk_7101a60148[];
void fn_710067dfdc(char*);

struct UnkScorpiusGasAttack_ovb0138 {
    void** vtable;
    u8 pad_0x8[0xc0];
    UnkStrId name;  // 0xc8
    u8 pad_0xd0[0x28];
    UnkStringData* ids[2];  // 0xf8
};
static_assert(__builtin_offsetof(UnkScorpiusGasAttack_ovb0138, ids) == 0xf8, "layout");

static inline void MakeEmpty(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

// Declared as src/game/CCharClassScorpiusAIComponent/CCharClassScorpiusAIComponent_b40.cpp declares it.
void fn_7100681a6c(void* member) {
    UnkScorpiusGasAttack_ovb0138* self = static_cast<UnkScorpiusGasAttack_ovb0138*>(member);
    fn_710067dfdc(reinterpret_cast<char*>(self));
    self->vtable = g_unk_7101a60148 + 2;
    MakeEmpty(&self->ids[0]);
    MakeEmpty(&self->ids[1]);
    self->name = UnkStrId("PoisonousGas", true);
}
