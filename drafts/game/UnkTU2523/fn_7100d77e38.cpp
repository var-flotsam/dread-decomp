// UnkTU2523 (CEntity): a constructor: a base built from the global empty string's text, three vtables
// (one at +0x10), two cleared lists with the default allocator, flags and counters, and a cleared
// 0x158-byte block at +0x1b8. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTU604.h"
#include "game/UnkTU63.h"
#include "types.h"

extern void* g_unk_7101aa8e80[];
void fn_7100231cf8(void* self, const char* str);
void* memset(void* dst, int c, u64 n);

struct UnkCtorList_ld77e38 {
    void** vtable;
    u32 field_0x8;
    void* field_0x10;
    void* field_0x18;
    base::UnkAllocator* allocator;
    u32 field_0x28;
};

// Packed: several members sit at offsets the type's alignment would not allow.
struct __attribute__((packed)) UnkCtorObject_ld77e38 {
    void** vtable;
    u8 pad_0x8[0x8];
    void** vtable_0x10;
    u8 pad_0x18[0x1a0];
    u8 block_0x1b8[0x158];
    bool field_0x310;
    bool field_0x311;
    bool field_0x312;
    UnkCtorList_ld77e38 list_0x318;
    UnkCtorList_ld77e38 list_0x348;
    u32 field_0x378;
    u64 field_0x37c;
    u64 field_0x384;
    u64 field_0x38c;
    u64 field_0x394;
    u64 field_0x39c;
    u8 field_0x3a4;
    u8 pad_0x3a5[3];
    f32 field_0x3a8;
    f32 field_0x3ac;
    bool field_0x3b0;
    u64 field_0x3b4;
    u32 field_0x3bc;
    bool field_0x3c0;
    u64 field_0x3c4;
    u32 field_0x3cc;
    u16 field_0x3d0;
    u8 field_0x3d2;
};

void fn_7100d77e38(UnkCtorObject_ld77e38* self) {
    const char* text = refl_GlobalRntString().data;
    if (!text)
        text = g_unk_710162e2ed;
    fn_7100231cf8(self, text);
    self->field_0x312 = false;
    self->field_0x310 = false;
    self->field_0x311 = true;
    self->vtable = g_unk_7101aa8e80 + 2;
    self->vtable_0x10 = g_unk_7101aa8e80 + 0x2d;
    self->list_0x318.vtable = g_unk_7101a32680 + 2;
    self->list_0x318.field_0x8 = 0;
    self->list_0x318.field_0x10 = nullptr;
    self->list_0x318.field_0x18 = nullptr;
    self->list_0x318.allocator = fn_7100083990();
    self->list_0x318.field_0x28 = 0x13;
    self->list_0x348.vtable = g_unk_7101a1b030 + 2;
    self->list_0x348.field_0x8 = 0;
    self->list_0x348.field_0x10 = nullptr;
    self->list_0x348.field_0x18 = nullptr;
    self->list_0x348.allocator = fn_7100083990();
    self->list_0x348.field_0x28 = 0x13;
    self->field_0x3a8 = 0.0f;
    self->field_0x3ac = -1.0f;
    self->field_0x37c = 0;
    self->field_0x384 = 0;
    self->field_0x39c = 0;
    self->field_0x3a4 = 0;
    self->field_0x38c = 0;
    self->field_0x394 = 0;
    self->field_0x3d0 = 0;
    self->field_0x3d2 = 0;
    self->field_0x3b0 = true;
    self->field_0x3b4 = 0;
    self->field_0x3bc = 0;
    self->field_0x3c0 = true;
    self->field_0x3c4 = 0;
    self->field_0x3cc = 0;
    self->field_0x378 = 1;
    memset(self->block_0x1b8, 0, 0x158);
}
