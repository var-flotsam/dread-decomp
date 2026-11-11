// CCooldownXBossLavaCarpetDef's unit (tools/tu_map.py unit 740): the definition's constructor. The
// CCharClassCooldownXBossAttack base (fn_71004a09b4), the vtable (0x7101a3dfc0), an empty array at +0x100 (growth
// 10 at +0x110, a set flag at +0x114, flags 15 at +0x118, the default allocator), then 0.1 and 0.5 at +0x128 and
// +0x12c. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU740.h"
#include "types.h"

#include <new>

struct UnkCooldownXBossAttackDef;
void fn_71004a09b4(UnkCooldownXBossAttackDef* self);

struct UnkLavaCarpetDef_A2_b74 {
    void** vtable;
    u8 pad_0x8[0xf8];
    base::UnkVector<void*> field_0x100;
    f32 field_0x128;
    f32 field_0x12c;
};

void fn_71004a4ffc(UnkLavaCarpetDef_A2_b74* self) {
    fn_71004a09b4(reinterpret_cast<UnkCooldownXBossAttackDef*>(self));
    self->vtable = g_unk_7101a3dfc0 + 2;
    new (&self->field_0x100) base::UnkVector<void*>(15);
    self->field_0x128 = 0.1f;
    self->field_0x12c = 0.5f;
}
