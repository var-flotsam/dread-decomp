// GUI::CDisplayObject unit (tools/tu_map.py, the name from reflection): handles an event of kind 2 (its
// +0x20): marks the object changed as its setters do (the first change runs vf_0xa0; the derived
// flags go stale; change bit 0x40), tells the GUI root the object lost the focus when it has it,
// runs vtable slot 0x1e0 with true, empties the string id at +0x20, and flags the event handled
// (+0x1a). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

struct UnkGuiEvent_b44a3 {
    u8 pad_0x0[0x1a];
    bool handled;
    u8 pad_0x1b[0x5];
    u64 kind;  // 0x20
};

void fn_71011cf7e4(UnkGuiDisplayObject* self, UnkGuiEvent_b44a3* event) {
    if (event->kind != 2)
        return;
    if (!self->field_0x184) {
        self->field_0x184 = true;
        self->vf_0xa0();
    }
    self->field_0x183 = true;
    self->field_0x144 |= 0x40;
    if (self->field_0x181)
        fn_71011b5d94(g_unk_717271e8f8, self);
    (*reinterpret_cast<void (***)(UnkGuiDisplayObject*, bool)>(self))[0x1e0 / 8](self, true);
    using Id = base::UnkStrIdMember;
    reinterpret_cast<Id*>(reinterpret_cast<char*>(self) + 0x20)->~Id();
    event->handled = true;
}
