// The constructor of the reflected CCharClassBossAIComponent (vtable 0x7101a34380) over the AI
// component base constructor fn_7100416db8: flags around +0x50c, a copy of the global empty string
// id at +0x518, a float at +0x4e0 (1.2), the base's flags at +0xc4 and +0xe8, then the
// post-construction virtual call (slot 3). Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/CCharClassBehaviorTreeAIComponent.h"
#include "types.h"

using base::global::CStrId;

// Declared as src/game/CCharClassGooplotAIComponent/CCharClassGooplotAIComponent.cpp does.
struct UnkObject_7100416db8;
extern void* g_unk_7101a34380[];

struct UnkBossAIDef {
    void** vtable;
    u8 pad_0x8[0xbc];
    bool field_0xc4;
    u8 pad_0xc5[0x23];
    bool field_0xe8;
    u8 pad_0xe9[0x3f7];
    f32 field_0x4e0;
    u8 pad_0x4e4[0x28];
    bool field_0x50c;
    bool field_0x50d;
    bool field_0x50e;
    u8 pad_0x50f;
    bool field_0x510;
    bool field_0x511;
    bool field_0x512;
    u8 pad_0x513[0x5];
    CStrId field_0x518;
    u8 pad_0x520[0x2];
    bool field_0x522;
};

void fn_710041a430(UnkBossAIDef* self) {
    fn_7100416db8(reinterpret_cast<UnkObject_7100416db8*>(self));
    self->vtable = g_unk_7101a34380 + 2;
    self->field_0x50d = true;
    self->field_0x50e = false;
    self->field_0x510 = false;
    self->field_0x511 = true;
    self->field_0x512 = true;
    new (&self->field_0x518) CStrId(g_unk_7101d470e0);
    self->field_0x522 = true;
    self->field_0xc4 = true;
    self->field_0xe8 = false;
    self->field_0x50c = false;
    self->field_0x4e0 = 1.2f;
    reinterpret_cast<void (*)(UnkBossAIDef*)>(self->vtable[3])(self);
}
