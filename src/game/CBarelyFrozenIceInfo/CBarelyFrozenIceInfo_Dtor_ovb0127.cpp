// CBarelyFrozenIceInfo's unit (tools/tu_map.py): its destructor (the owned objects at +0xc8, the
// string ids at +0xb8 and +0xb0, then the base destructor fn_71007ee5bc). Names and layout are
// placeholders.
#include "base/global/CStrId.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

extern void* g_unk_7101a73e38[];  // the class's vtable
void fn_71007ee5bc(char*);

struct UnkFrozenIceInfo_ovb0127 {
    void** vtable;
    u8 pad_0x8[0xa8];
    base::global::CStrId field_0xb0;
    base::global::CStrId field_0xb8;
    u8 pad_0xc0[0x8];
    UnkOwnedList field_0xc8;
};

void fn_71007f0a2c(UnkFrozenIceInfo_ovb0127* self) {
    self->vtable = g_unk_7101a73e38 + 2;
    self->field_0xc8.~UnkOwnedList();
    self->field_0xb8.Reset();
    self->field_0xb0.Reset();
    fn_71007ee5bc(reinterpret_cast<char*>(self));
}
