// Unit 1161 (tools/tu_map.py, no name hint). fn_7100745b74: the deleting destructor beside fn_7100745b24
// (src/game/UnkUnit1161/UnkUnit1161_b42.cpp): the vtable, then the member at +0xe8 (fn_7100745804
// destroys it) and the array of pointers at +0xb8 (freed when it holds one), then the object is freed.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU1160.h"
#include "types.h"

extern void* g_unk_7101a6da30[];

// Declared as src/game/UnkUnit1161/UnkUnit1161_b42.cpp does.
struct UnkMember_e8_b42;

struct UnkHolder_1161_b43a2 {
    void** vtable;
    u8 pad_0x8[0xb0];
    base::UnkVector<void*> items;  // 0xb8
    u8 pad_0xe0[0x8];
    UnkMember_e8_b42* member() { return reinterpret_cast<UnkMember_e8_b42*>(reinterpret_cast<char*>(this) + 0xe8); }
};

void fn_7100745b74(UnkHolder_1161_b43a2* self) {
    self->vtable = &g_unk_7101a6da30[2];
    fn_7100745804(self->member());
    self->items.Free();
    delete reinterpret_cast<char*>(self);
}
