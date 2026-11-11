// CTunnelTrapMorphballComponent: its constructor (fn_7100a44f10). The component base constructor
// fn_71010c4148, the class vtable and a second vtable at +0x40, a cleared flag, an embedded object
// at +0x50 (its own vtable and three cleared words), the string-id list at +0x70 (fn_7100a44d38),
// a pair of cleared words, three small defaults, an allocator-backed buffer at +0xc8, then the
// object's own vf_0x18. Names and layouts are placeholders.
#include "game/CAbilityGhostAura.h"
#include "game/CCharClassTunnelTrapMorphballComponent.h"
#include "game/CComponent.h"
#include "types.h"

#include "base/UnkVector.h"

struct UnkStateObject;
void fn_7100a44d38(char*);

struct UnkTunnelTrapMorphballComponent {
    void** vtable;
    u8 pad_0x8[0x38];
    void** vtable_0x40;
    bool field_0x48;
    u8 pad_0x49[7];
    void** field_0x50;
    u64 field_0x58;
    u64 field_0x60;
    u64 field_0x68;
    u8 field_0x70[0x38];
    u64 field_0xa8;
    u64 field_0xb0;
    u32 field_0xb8;
    bool field_0xbc;
    u32 field_0xc0;
    base::UnkAllocator* field_0xc8;
    u64 field_0xd0;
    bool field_0xd8;
};

void fn_7100a44f10(UnkTunnelTrapMorphballComponent* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a8e700 + 2;
    self->vtable_0x40 = g_unk_7101a8e700 + 53;
    self->field_0x48 = false;
    self->field_0x50 = g_unk_7101a22d48 + 2;
    __builtin_memset(&self->field_0x58, 0, 24);
    fn_7100a44d38(reinterpret_cast<char*>(self->field_0x70));
    self->field_0xa8 = 0;
    self->field_0xb0 = 0;
    self->field_0xb8 = 10;
    self->field_0xbc = true;
    self->field_0xc0 = 15;
    self->field_0xc8 = fn_7100083990();
    self->field_0xd0 = 0;
    self->field_0xd8 = false;
    reinterpret_cast<void (*)(UnkTunnelTrapMorphballComponent*)>(self->vtable[3])(self);
}
