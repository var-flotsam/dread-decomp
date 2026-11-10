// CLogicActionTriggerComponent's unit (tools/tu_map.py unit 1398): the destructor (fn_710090f358): the
// string-id table at +0x78 is reset and freed, the string id at +0x70 released and the owned actions at
// +0x40 destroyed before the base destructor (fn_71010c4194). Names and layouts are placeholders.
#include "base/UnkStrIdTable.h"
#include "base/global/CStrId.h"
#include "game/UnkComponent.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101a81370[];

struct UnkLogicActionTrigger_ovb0125 : UnkBaseComponent {
    UnkOwnedList actions;  // 0x40
    u8 pad_0x68[0x8];
    CStrId field_0x70;
    UnkStrIdTable field_0x78;
};
static_assert(__builtin_offsetof(UnkLogicActionTrigger_ovb0125, field_0x78) == 0x78, "layout");

void fn_710090f358(UnkLogicActionTrigger_ovb0125* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101a81370 + 0x10);
    fn_71004f11f8(&self->field_0x78);
    if (self->field_0x78.keys)
        self->field_0x78.allocator->vf_0x48(self->field_0x78.keys);
    if (self->field_0x78.values)
        self->field_0x78.allocator->vf_0x48(self->field_0x78.values);
    self->field_0x70.Reset();
    self->actions.~UnkOwnedList();
    fn_71010c4194(self);
}
