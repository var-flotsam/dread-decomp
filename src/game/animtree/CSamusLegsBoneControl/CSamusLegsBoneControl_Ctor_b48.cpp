// animtree::CSamusLegsBoneControl unit (tools/tu_map.py): the constructor: the base (fn_71002e3b24),
// this class's vtable (g_unk_7101a22128), cleared state, an index of -1, and the node registration
// fn_7100023854 last. Names and layouts are placeholders.
#include "base/core/CBaseObject.h"
#include "types.h"

struct UnkNode;

extern char g_unk_7101a22128[];

struct UnkSamusLegsBoneControl {
    char* vtable;
    u8 base_0x8[0x58];
    u64 field_0x60;
    u32 field_0x68;
    u64 field_0x70[4];
    u32 field_0x90;
    u64 field_0x98[2];
    s32 index;          // 0xa8
    f32 field_0xac[12];
};

void fn_71002e3b24(UnkSamusLegsBoneControl* self);  // the base constructor

void fn_71002e89a8(UnkSamusLegsBoneControl* self) {
    fn_71002e3b24(self);
    self->vtable = g_unk_7101a22128 + 0x10;
    self->field_0x60 = 0;
    self->field_0x68 = 0;
    __builtin_memset(self->field_0x70, 0, sizeof(self->field_0x70));
    self->field_0x90 = 0;
    __builtin_memset(self->field_0x98, 0, sizeof(self->field_0x98));
    self->index = -1;
    for (f32& v : self->field_0xac)
        v = 0.0f;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
