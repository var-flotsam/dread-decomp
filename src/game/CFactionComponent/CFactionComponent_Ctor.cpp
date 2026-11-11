// CFactionComponent's constructor (the name from reflection): the component base's (fn_71010c4148),
// an empty string at +0x40, a cleared word at +0x70, then the component's own set-up through its
// vtable slot 0x18. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntString.h"
#include "game/CComponent.h"
#include "types.h"

using base::global::CRntString;

extern char g_unk_7101cb2ee0[];  // CFactionComponent's vtable
struct UnkStateObject;

struct UnkFactionComponent {
    char* vtable;
    u8 base_0x8[0x38];
    CRntString name;  // 0x40
    u32 field_0x70;

    void vf_0x18() { (*reinterpret_cast<void (**)(UnkFactionComponent*)>(vtable + 0x18))(this); }
};
static_assert(__builtin_offsetof(UnkFactionComponent, field_0x70) == 0x70, "UnkFactionComponent layout");

void fn_71011396d8(UnkFactionComponent* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101cb2ee0 + 0x10;
    new (&self->name) CRntString("");
    self->field_0x70 = 0;
    self->vf_0x18();
}
