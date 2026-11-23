#pragma once

#include "types.h"

// A texture record: its size, kind and NVN format, the NVN texture object, its descriptor id and
// the memory it was placed in.
struct UnkGfxTexture {
    s32 width = 0;
    s32 height = 0;
    s32 kind = -1;     // 0x8
    s32 format;        // 0xc
    s32 field_0x10;
    u8 pad_0x14[0x4];
    u8 texture[0xc0];  // 0x18: the NVN texture
    s32 id = 0;               // 0xd8: texture descriptor id
    u8 pad_0xdc[0x4];
    void* storage = nullptr;  // 0xe0
    u32 size = 0;             // 0xe8: of the storage
    s32 index = -1;           // 0xec: in g_unk_716e2a3568
    s32 field_0xf0 = 0;
    u8 pad_0xf4[0x4];
    void* new_storage = nullptr;  // 0xf8
};
