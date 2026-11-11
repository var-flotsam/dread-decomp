// Two identical constructors in the CCharClassCooldownXBossAttack unit (vtable 0x7101a3de78): the
// attack base constructor fn_71003641fc, the vtable, a float at +0xec (250) and the string ids
// "startlava" and "endlava" at +0xf0 and +0xf8. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/CCharClassAttack.h"
#include "types.h"

using base::global::CStrId;

// Declared as src/game/CCharClassAttack/fn_71003641fc.cpp defines it.
struct UnkStruct_71003641fc;
extern void* g_unk_7101a3de78[];

struct UnkCooldownXBossAttackDef {
    void** vtable;
    u8 pad_0x8[0xe4];
    f32 field_0xec;
    CStrId start;  // 0xf0
    CStrId end;    // 0xf8
};

static inline __attribute__((always_inline)) void Construct(UnkCooldownXBossAttackDef* self) {
    fn_71003641fc(reinterpret_cast<UnkStruct_71003641fc*>(self));
    self->vtable = g_unk_7101a3de78 + 2;
    self->field_0xec = 250.0f;
    new (&self->start) CStrId("startlava", true);
    new (&self->end) CStrId("endlava", true);
}

void fn_71004a09b4(UnkCooldownXBossAttackDef* self) { Construct(self); }
void fn_71004a0c48(UnkCooldownXBossAttackDef* self) { Construct(self); }
