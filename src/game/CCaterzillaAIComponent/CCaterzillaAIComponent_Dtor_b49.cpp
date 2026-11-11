// CCaterzillaAIComponent (tools/tu_map.py): the destructor. Stores the class's vtable, releases the
// counted reference at +0x146d0 and runs the base AI component's destructor (fn_7100369380).
// Names and layouts are placeholders.
#include "../../game_5821e4/CAttackDtor.h"
#include "game/CBehaviorTreeAIComponent.h"

extern void* g_unk_7101a31d90[];  // the component's vtable

struct UnkCaterzillaAI_b49 {
    void** vtable;
    u8 pad_0x8[0x146c8];
    UnkAttackOwner* field_0x146d0;
};

void fn_71003f66e0(UnkCaterzillaAI_b49* self) {
    self->vtable = g_unk_7101a31d90 + 2;
    if (UnkAttackOwner* p = self->field_0x146d0) {
        if (--p->ref_count == 0)
            p->Delete();
    }
    fn_7100369380(self);
}
