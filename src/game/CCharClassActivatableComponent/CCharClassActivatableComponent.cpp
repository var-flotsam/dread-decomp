// Constructor of a component in the CCharClassActivatableComponent unit: the component base
// constructor fn_71010c4148, the vtable, the string-id list at +0x40 (fn_71007b1528), a cleared flag
// and word, then the post-construction virtual call (slot 3). Names and layouts are placeholders.
#include "game/CComponent.h"
#include "game/CDemolitionBlockComponent.h"
#include "types.h"

struct UnkStateObject;
extern void* g_unk_7101a7ac20[];

struct UnkActivatableComponent {
    void** vtable;
    u8 pad_0x8[0x38];
    u8 field_0x40[0x38];
    bool field_0x78;
    u32 field_0x7c;
};

void fn_7100878d24(UnkActivatableComponent* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a7ac20 + 2;
    fn_71007b1528(reinterpret_cast<char*>(self->field_0x40));
    self->field_0x78 = false;
    self->field_0x7c = 0;
    reinterpret_cast<void (*)(UnkActivatableComponent*)>(self->vtable[3])(self);
}
