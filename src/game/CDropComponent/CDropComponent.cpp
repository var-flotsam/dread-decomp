// A constructor in the CDropComponent unit (the shape of fn_71006eede8): the component base
// constructor fn_71010c4148, the vtable, two members built by fn_71007c7ddc, two cleared fields, then
// the post-construction virtual call (slot 3). Names and layouts are placeholders.
#include "game/CComponent.h"
#include "game/SDropProbabilities.h"
#include "types.h"

struct UnkStateObject;
extern void* g_unk_7101a715c0[];

struct UnkDropComponent {
    void** vtable;
    u8 pad_0x8[0x38];
    u8 field_0x40[0x90];
    u8 field_0xd0[0x90];
    u64 field_0x160;
    u64 field_0x168;
    u64 field_0x170;
};

void fn_71007c5ccc(UnkDropComponent* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a715c0 + 2;
    fn_71007c7ddc(self->field_0x40);
    fn_71007c7ddc(self->field_0xd0);
    self->field_0x160 = 0;
    self->field_0x170 = 0;
    reinterpret_cast<void (*)(UnkDropComponent*)>(self->vtable[3])(self);
}
