#include <new>

#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkFadeInit {
    f32 field_0x0[12];     // 0x0
    f32 field_0x30[3];     // 0x30
    u8 block_0x3c[0x195];  // 0x3c
    u8 pad_0x1d1[3];
    u8 block_0x1d4[0x9e];  // 0x1d4
    u8 pad_0x272[6];
    CFilePathStrId path;   // 0x278
    void* texture;         // 0x280
    f32 half[6];           // 0x288
    u16 flags_0x2a0;       // 0x2a0
    u8 pad_0x2a2[2];
    f32 field_0x2a4[2];    // 0x2a4
    f32 field_0x2ac;
    f32 field_0x2b0;
    f32 ones[10];          // 0x2b4
    f32 field_0x2dc;       // 0x2dc
    u8 block_0x2e0[0x3d];  // 0x2e0
    u8 pad_0x31d[3];
    f32 field_0x320[2];    // 0x320
    f32 field_0x328;       // 0x328
    u8 block_0x32c[0x19];  // 0x32c
    u8 pad_0x345[3];
    u64 zero_0x348[0x1d];  // 0x348
    bool done[29];         // 0x430
};
static_assert(__builtin_offsetof(UnkFadeInit, path) == 0x278, "layout");
static_assert(__builtin_offsetof(UnkFadeInit, done) == 0x430, "layout");

void fn_7100f97d90(void* state) {
    UnkFadeInit* s = static_cast<UnkFadeInit*>(state);
    __builtin_memset(s->field_0x0, 0, sizeof(s->field_0x0));
    s->field_0x30[0] = 1.0f;
    s->field_0x30[1] = 1.0f;
    s->field_0x30[2] = 1.0f;
    u8* block = s->block_0x1d4;
    asm("" : "+r"(block));
    __builtin_memset(s->block_0x3c, 0, sizeof(s->block_0x3c));
    __builtin_memset(block, 0, sizeof(s->block_0x1d4));
    new (&s->path) CFilePathStrId(g_unk_7101d470e8);
    s->texture = nullptr;
    for (f32& h : s->half)
        h = 0.5f;
    s->flags_0x2a0 = 0;
    s->field_0x2a4[0] = 0.0f;
    s->field_0x2a4[1] = 0.0f;
    s->field_0x2ac = 0.0f;
    s->field_0x2b0 = 1.0f;
    for (f32& o : s->ones)
        o = 1.0f;
    s->field_0x2dc = 1.0f;
    __builtin_memset(s->block_0x2e0, 0, sizeof(s->block_0x2e0));
    s->field_0x320[0] = 1.0f;
    s->field_0x320[1] = 1.0f;
    s->field_0x328 = 1.0f;
    __builtin_memset(s->block_0x32c, 0, sizeof(s->block_0x32c));
    __builtin_memset(s->zero_0x348, 0, sizeof(s->zero_0x348));
    __builtin_memset(s->done, 1, sizeof(s->done));
}
