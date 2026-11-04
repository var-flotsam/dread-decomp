// CCharClassSetEmmyDoorNearSamusVisibilityEvent unit (tools/tu_map.py, unit 1849): hands the actor's position
// and the third column of its matrix to the E.M.M.I. registry (fn_7100daeb50) with the event's kind (+0xc), as
// src/game/CEmmyGrabSamusAttack/CEmmyGrabSamusAttack_Send_b58.cpp does. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkEmmyRegistry_b55;
extern UnkEmmyRegistry_b55* g_unk_71726af0a8;

struct UnkGrabVec_b58a3 {
    f32 x, y, z;
};
void fn_7100daeb50(UnkEmmyRegistry_b55* registry, u32 kind, const UnkGrabVec_b58a3* pos,
                   const UnkGrabVec_b58a3* dir);

static inline __attribute__((always_inline)) const UnkGrabVec_b58a3* Addr_A5_b73(const UnkGrabVec_b58a3& v) {
    return &v;
}

static inline __attribute__((always_inline)) UnkGrabVec_b58a3 Dir_A5_b73(const UnkMatrix44& m) {
    return {m.m[2], m.m[6], m.m[10]};
}

static inline __attribute__((always_inline)) UnkGrabVec_b58a3 Pos_A5_b73(const UnkActor* a) {
    return {a->field_0x44, a->field_0x54, a->field_0x64};
}

struct UnkEmmyDoorEvent_A5_b73 {
    u8 pad_0x0[0xc];
    u8 kind;
};

void fn_7100bd52e0(UnkEmmyDoorEvent_A5_b73* self, UnkActor** actor) {
    UnkActor* a = *actor;
    if (!a)
        return;
    fn_7100daeb50(g_unk_71726af0a8, self->kind, Addr_A5_b73(Pos_A5_b73(a)),
                  Addr_A5_b73(Dir_A5_b73(fn_710002614c(reinterpret_cast<const UnkMatrix44*>(a->field_0x38)))));
}
