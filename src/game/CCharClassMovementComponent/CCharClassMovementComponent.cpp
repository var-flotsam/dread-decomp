// CCharClassMovementComponent: its constructor (fn_710097b304). The base constructor fn_71011acd10, a
// cleared word at +0x5c, the vtable, a set flag at +0x58, 4000 at +0x18, then the class-entry pass of
// base::core::CBaseObject (fn_7100023854). Names and layouts are placeholders.
#include "base/core/CBaseObject.h"
#include "game/CCharClassComponent.h"
#include "game/CCharClassMovementComponent.h"
#include "types.h"

struct UnkNode;
struct UnkStruct_71008bca30;

struct UnkCharClassMovementComponent {
    void** vtable;
    u8 pad_0x8[0x10];
    u64 field_0x18;
    u8 pad_0x20[0x38];
    bool field_0x58;
    u8 pad_0x59[3];
    u32 field_0x5c;
};

void fn_710097b304(UnkCharClassMovementComponent* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->field_0x5c = 0;
    self->vtable = g_unk_7101a85840 + 2;
    self->field_0x58 = true;
    self->field_0x18 = 4000;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
