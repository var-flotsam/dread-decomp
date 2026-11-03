// CCharClassActionSwitcherOnPullGrapplePointComponent unit (tools/tu_map.py unit 1285).
// fn_71008274dc: the constructor; runs the base constructor (fn_7100828940), sets the vtable, starts
// the five ids at +0x78 empty and the word at +0xa0 at zero, then calls its own vtable slot 0x18.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a760c0[];

struct UnkSelf_as41 {
    void** vtable;
    u8 pad_0x8[0x70];
    CStrId id_0x78;
    CStrId id_0x80;
    CStrId id_0x88;
    CStrId id_0x90;
    CStrId id_0x98;
    u32 field_0xa0;
};

void fn_7100828940(UnkSelf_as41* self);

void fn_71008274dc(UnkSelf_as41* self) {
    fn_7100828940(self);
    self->vtable = &g_unk_7101a760c0[2];
    new (&self->id_0x78) CStrId(g_unk_7101d470e0);
    new (&self->id_0x80) CStrId(g_unk_7101d470e0);
    new (&self->id_0x88) CStrId(g_unk_7101d470e0);
    new (&self->id_0x90) CStrId(g_unk_7101d470e0);
    new (&self->id_0x98) CStrId(g_unk_7101d470e0);
    self->field_0xa0 = 0;
    reinterpret_cast<void (*)(UnkSelf_as41*)>(self->vtable[3])(self);
}
