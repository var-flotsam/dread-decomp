// GUI::CSlotInfoComposition (the name from reflection: its class's create function and getter,
// src/reflection/GUI_CSlotInfoComposition.cpp). The constructor: GUI::CDisplayObjectContainer's
// (fn_71011d8d24), the vtable, no slot (-1), and the display object's set-up (fn_71011cd128). The
// virtual class getter (vtable slot 0): the class's getter, then its class object, as every
// reflected class's slot 0 (the others' are generated, src/lifted/). Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

extern char g_unk_7101a9ee70[];  // CSlotInfoComposition's vtable
extern char g_unk_7172766bf8[] __attribute__((visibility("hidden")));  // its class object (fn_7100b79b74 builds it)

struct UnkSlotInfoComposition {
    char* vtable;
    u8 base_0x8[0x230 - 0x8];
    s32 slot;  // 0x230
};
static_assert(__builtin_offsetof(UnkSlotInfoComposition, slot) == 0x230, "UnkSlotInfoComposition layout");

void fn_7100b7b770(UnkSlotInfoComposition* self) {
    fn_71011d8d24(self);
    self->vtable = g_unk_7101a9ee70 + 0x10;
    self->slot = -1;
    fn_71011cd128(reinterpret_cast<char*>(self));
}

const base::reflection::CClass* fn_7100b7b040() {
    fn_7100b79b74();
    return reinterpret_cast<const base::reflection::CClass*>(g_unk_7172766bf8);
}
