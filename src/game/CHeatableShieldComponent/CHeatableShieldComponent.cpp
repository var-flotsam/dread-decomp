// Constructor of a component in the CHeatableShieldComponent unit: the component base constructor
// fn_71010c4148, the vtable (0x7101a7a260), an empty array at +0x48 (default allocator), the member
// at +0x68 (fn_710086abb4), two cleared words, then the post-construction virtual call (slot 3).
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkComponent.h"
#include "types.h"

struct UnkStateObject;
void fn_71010c4148(UnkStateObject* self);
void fn_710086abb4(void* self);
extern void* g_unk_7101a7a260[];

struct UnkHeatableShield : UnkBaseComponent {
    u64 field_0x40;
    u64 field_0x48;
    u64 field_0x50;
    u32 field_0x58;
    u32 field_0x5c;
    base::UnkAllocator* field_0x60;
    u8 field_0x68[0x200];
    u64 field_0x268;
    u64 field_0x270;
};

void fn_710086cab8(UnkHeatableShield* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->field_0x40 = 0;
    self->field_0x48 = 0;
    self->vtable = g_unk_7101a7a260 + 2;
    self->field_0x50 = 0;
    self->field_0x58 = 10;
    self->field_0x5c = 0xe;
    self->field_0x60 = fn_7100083990();
    fn_710086abb4(self->field_0x68);
    self->field_0x270 = 0;
    self->field_0x268 = 0;
    reinterpret_cast<void (*)(UnkHeatableShield*)>(self->vtable[3])(self);
}
