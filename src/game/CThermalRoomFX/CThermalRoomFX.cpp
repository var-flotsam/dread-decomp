// CThermalRoomFX: its constructor (fn_710087c0a4). The activatable part (the component base
// constructor fn_71010c4148, CActivatableComponent's vtable, the string-id list at +0x40
// (fn_71007b1528), a cleared flag and word), then its own vtable and the class-entry pass of
// base::core::CBaseObject (fn_7100023854). Names and layouts are placeholders.
#include "base/core/CBaseObject.h"
#include "game/CCharClassActivatableComponent.h"
#include "game/CComponent.h"
#include "game/CDemolitionBlockComponent.h"
#include "types.h"

struct UnkStateObject;
struct UnkNode;
extern void* g_unk_7101a7b1e0[];

struct UnkThermalRoomFX {
    void** vtable;
    u8 pad_0x8[0x38];
    u8 field_0x40[0x38];
    bool field_0x78;
    u32 field_0x7c;
};

void fn_710087c0a4(UnkThermalRoomFX* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a7ac20 + 2;
    fn_71007b1528(reinterpret_cast<char*>(self->field_0x40));
    self->field_0x78 = false;
    self->field_0x7c = 0;
    self->vtable = g_unk_7101a7b1e0 + 2;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
