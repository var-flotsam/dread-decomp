// Destructor of CFloatingPropActingComponent (the shape of fn_71006bd00c): restore the vtable, destroy
// the string member, drop the counted reference and run the component destructor fn_71010c4194.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "game/CFloatingPropActingComponent.h"
#include "game/UnkComponent.h"
#include "game_s2/UnkWeakHandle.h"

struct UnkFloatingPropActingComponent : UnkBaseComponent {
    UnkRefObject* field_0x40;
    u8 pad_0x48[0x18];
    base::global::CRntString field_0x60;
};

void fn_71007e62f0(UnkFloatingPropActingComponent* self) {
    self->vtable = g_unk_7101a73768 + 2;
    fn_710007ecd4(&self->field_0x60);
    if (UnkRefObject* p = self->field_0x40) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
    fn_71010c4194(self);
}
