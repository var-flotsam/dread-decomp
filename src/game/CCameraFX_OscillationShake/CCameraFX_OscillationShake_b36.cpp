// CCameraFX_OscillationShake unit (tools/tu_map.py): a constructor shaped like fn_71010f2b68
// (CCameraFX_RandShake): the base fn_71010f3d20, the vtable, an empty string id at +0x58, zeroed
// fields after it, then the post-construction virtual call (slot 3). Names and layouts
// are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/CCameraFX_Shake.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkRandShakeDef;
extern char g_unk_7101a6e8b8[];

struct UnkOscillationShakeDef {
    void** vtable;
    u8 pad_0x8[0x50];
    UnkStrId field_0x58;
    u64 field_0x60;
    u64 field_0x68;
    u64 field_0x70;
    u32 field_0x78;
};

void fn_7100767fb8(UnkOscillationShakeDef* self) {
    fn_71010f3d20(reinterpret_cast<UnkRandShakeDef*>(self));
    self->vtable = reinterpret_cast<void**>(g_unk_7101a6e8b8 + 0x10);
    new (&self->field_0x58) UnkStrId("", true);
    self->field_0x60 = 0;
    self->field_0x68 = 0;
    self->field_0x70 = 0;
    self->field_0x78 = 0;
    reinterpret_cast<void (*)(UnkOscillationShakeDef*)>(self->vtable[3])(self);
}
