// Constructor in the CCharClassEmmyAIComponent unit (tools/tu_map.py, the name from reflection): the
// base constructor fn_7100416db8, the vtable (0x7101a480a8), the member at +0x510 (fn_7100504b9c), a
// count of 1, a copy of the empty string id and an empty id of its own, flag bits and a few bytes.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CCharClassBehaviorTreeAIComponent.h"
#include <new>

#include "types.h"

using base::global::CStrId;

// Declared as src/game/CCharClassGooplotAIComponent/CCharClassGooplotAIComponent.cpp does.
struct UnkObject_7100416db8;
void fn_7100504b9c(void* self);
extern void* g_unk_7101a480a8[];

struct UnkEmmyDef_b55 {
    void** vtable;
    u8 pad_0x8[0x158];
    u32 field_0x160;
    u32 field_0x164;
    u8 pad_0x168[0x238];
    u16 field_0x3a0;
    u8 pad_0x3a2[0xe8];
    bool field_0x48a;
    u8 pad_0x48b[0x65];
    bool field_0x4f0;
    u8 pad_0x4f1[0x1f];
    u8 field_0x510[0xf0];
    u32 field_0x600;
    u8 pad_0x604[4];
    CStrId field_0x608;
    u16 field_0x610;
    u8 pad_0x612[6];
    base::global::UnkStrIdBase field_0x618;
};

void fn_710051eaa0(UnkEmmyDef_b55* self) {
    fn_7100416db8(reinterpret_cast<UnkObject_7100416db8*>(self));
    self->vtable = g_unk_7101a480a8 + 2;
    fn_7100504b9c(self->field_0x510);
    self->field_0x600 = 1;
    new (&self->field_0x608) CStrId(g_unk_7101d470e0);
    self->field_0x610 = 0x101;
    // An empty id: the string-id constructor with no string, inlined (the null data is known).
    self->field_0x618.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x618.data = &g_unk_7101d47108->empty;
    self->field_0x160 |= 0xe;
    self->field_0x164 |= 0x1e;
    self->field_0x3a0 = 0x100;
    self->field_0x48a = false;
    self->field_0x4f0 = false;
}
