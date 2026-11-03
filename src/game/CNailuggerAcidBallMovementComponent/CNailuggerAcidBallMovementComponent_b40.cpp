// CNailuggerAcidBallMovementComponent unit (tools/tu_map.py unit 943): the constructor (fn_71006263dc).
// The projectile movement's base constructor (fn_7100ac2c5c), the vtable, then two empty string ids at
// +0x5e0 and +0x5e8 (cleared, then pointed at the pool's empty string, creating the pool first when there
// is none). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

void fn_7100ac2c5c(void* self);
extern void* g_unk_7101a57008[];

struct UnkNailuggerAcidBallMovement {
    void** vtable;
    u8 pad_0x8[0x5d8];
    base::global::UnkStringData* field_0x5e0;
    base::global::UnkStringData* field_0x5e8;
};

static inline __attribute__((always_inline)) void InitEmpty_na(base::global::UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_71006263dc(UnkNailuggerAcidBallMovement* self) {
    fn_7100ac2c5c(self);
    self->vtable = g_unk_7101a57008 + 2;
    InitEmpty_na(&self->field_0x5e0);
    InitEmpty_na(&self->field_0x5e8);
}
