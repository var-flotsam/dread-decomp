// CSamusAnalogAimMovementState unit (tools/tu_map.py): after the base state's step (fn_71009b0ce4),
// when the movement's (+0x38) shape (fn_7100976588) is of the type in g_unk_717273b390 and
// fn_71009e84e4 fails, feeds the movement's virtual at +0x2a8 into its +0x2b0. Names and layouts are
// placeholders.
#include "game/CCharacterMovement.h"
#include "game/CSamusMovementState/CParams.h"
#include "game/UnkTU1501.h"
#include "types.h"

struct CSamusAirMovementState;
struct UnkFlagged;
struct UnkShape;

extern void* g_unk_717273b390 __attribute__((visibility("hidden")));

struct UnkAimShape_b49a4 {
    u8 pad_0x0[0x20];
    void* type;
};

struct UnkAimMovement_b49a4 {
    void** vtable;

    void* vf_0x2a8() { return reinterpret_cast<void* (*)(UnkAimMovement_b49a4*)>(vtable[0x2a8 / 8])(this); }
    void vf_0x2b0(void* v) { reinterpret_cast<void (*)(UnkAimMovement_b49a4*, void*)>(vtable[0x2b0 / 8])(this, v); }
};

struct UnkAnalogAimState_b49a4 {
    u8 pad_0x0[0x38];
    UnkAimMovement_b49a4* movement;
};

void fn_710098e7a8(UnkAnalogAimState_b49a4* self, void* arg) {
    fn_71009b0ce4(reinterpret_cast<CSamusAirMovementState*>(self), arg);
    auto* shape = reinterpret_cast<UnkAimShape_b49a4*>(fn_7100976588(reinterpret_cast<UnkFlagged*>(self->movement)));
    if (shape->type != g_unk_717273b390)
        return;
    if (fn_71009e84e4(reinterpret_cast<char*>(self->movement)))
        return;
    void* value = self->movement->vf_0x2a8();
    self->movement->vf_0x2b0(value);
}
