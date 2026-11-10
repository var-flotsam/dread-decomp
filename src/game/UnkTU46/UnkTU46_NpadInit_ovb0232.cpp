// Unit 46 of tools/tu_map.py: the constructor of the pad group over the base of fn_710006daf0 (see
// src/base/global/UnkSoundHolder_Init.cpp): clears its state, initialises the Npad layer for the full key and handheld
// styles and nine supported ids (handheld first, then 0 to 7). Names and layouts are placeholders.
#include <nn/hid.h>

#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a1a2b0[];
extern void* g_unk_7101a1a218[];

struct UnkSoundGroup_710006cdb8;

struct UnkNpadGroup_b0232 {
    void* vtable;  // 0x0
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 pad_0x14;
    void* field_0x18;
    void* field_0x20;
    u64 field_0x28;
    u32 field_0x30;
    u32 field_0x34;
    base::UnkAllocator* allocator;  // 0x38
    u64 field_0x40;
    u64 ids[4];  // 0x48
    u32 id_8;  // 0x68
    u8 pad_0x6c[0x45c - 0x6c];
    u32 modes[9];  // 0x45c
    u8 pad_0x480[0x7a0 - 0x480];
    u8 buffer[0x168];  // 0x7a0
    u8 pad_0x908[0x908 - 0x908];
    u64 zeros[18];  // 0x908
};
static_assert(__builtin_offsetof(UnkNpadGroup_b0232, zeros) == 0x908, "layout");

void fn_710006daf0(UnkSoundGroup_710006cdb8* self_) {
    UnkNpadGroup_b0232* self = reinterpret_cast<UnkNpadGroup_b0232*>(self_);
    self->field_0x10 = 1;
    self->vtable = reinterpret_cast<char*>(g_unk_7101a1a2b0) + 0x10;
    self->field_0x8 = 0xfe;
    self->field_0xc = 2;
    self->field_0x18 = nullptr;
    self->field_0x20 = nullptr;
    self->field_0x28 = 0;
    self->field_0x30 = 10;
    self->field_0x34 = 14;
    self->allocator = fn_7100083990();
    self->vtable = reinterpret_cast<char*>(g_unk_7101a1a218) + 0x10;
    __builtin_memset(self->buffer, 0, sizeof(self->buffer));
    nn::hid::InitializeNpad();
    nn::hid::NpadStyleSet styles;
    styles.Set(0, true);
    styles.Set(1, true);
    nn::hid::SetSupportedNpadStyleSet(styles);
    self->field_0x40 = 0x9ffffffffULL;
    for (u64& z : self->zeros)
        z = 0;
    self->ids[0] = 0x20;
    self->ids[1] = 0x200000001ULL;
    self->ids[2] = 0x400000003ULL;
    self->ids[3] = 0x600000005ULL;
    self->id_8 = 7;
    nn::hid::SetSupportedNpadIdType(reinterpret_cast<u32*>(self->ids), 9);
    self->modes[0] = 2;
    for (u32 i = 1; i < 9; ++i)
        self->modes[i] = 1;
    u64 pair = 0x200000001ULL;
    __builtin_memcpy(reinterpret_cast<u8*>(self) + 0xc, &pair, 8);
}
