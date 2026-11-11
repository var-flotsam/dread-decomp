// tools/tu_map.py unit 218 (no class hint): a constructor over the base built by fn_710019e76c
// (fn_7100198aac, created by fn_71001b5b7c-style pooled creation at 0x29d0 bytes): sets the vtable
// g_unk_7101a1e0a8, an empty array (flags 0xf), an identity 4x4 matrix at +0x88, three pointer-array holders
// (0xd8, 0x120, 0x168) and a cleared 0x2801-byte block. Names and layouts are placeholders.
#include <new>

#include "../UnkUnit211/UnkPtrArrayHolder_b70.h"
#include "base/UnkVector.h"
#include "game/UnkTU218.h"
#include "game/UnkTU226.h"
#include "types.h"

extern void* g_unk_7101a1e0a8[];

namespace {

struct UnkIdentity44_218_b70a1 {
    f32 m[16];

    UnkIdentity44_218_b70a1() {
        __builtin_memset(m, 0, sizeof(m));
        m[15] = 1.0f;
        m[10] = 1.0f;
        m[5] = 1.0f;
        m[0] = 1.0f;
    }
};

}  // namespace

struct UnkObject218_b70a1 {
    void* vtable;
    u8 base_0x8[0x18];
    u64 field_0x20[7];
    base::UnkVector<void*> v0;  // 0x58
    u64 field_0x80;
    UnkIdentity44_218_b70a1 m;  // 0x88
    u64 field_0xc8;
    u32 field_0xd0;
    UnkPtrArrayHolder_b70a1 h0;  // 0xd8
    UnkPtrArrayHolder_b70a1 h1;  // 0x120
    UnkPtrArrayHolder_b70a1 h2;  // 0x168
    u8 field_0x1b0[0x2801];
    u32 field_0x29b4[7];
};
static_assert(__builtin_offsetof(UnkObject218_b70a1, h0) == 0xd8, "layout");
static_assert(__builtin_offsetof(UnkObject218_b70a1, field_0x29b4) == 0x29b4, "layout");
static_assert(sizeof(UnkObject218_b70a1) == 0x29d0, "layout");

void fn_7100198aac(void* p) {
    UnkObject218_b70a1* self = static_cast<UnkObject218_b70a1*>(p);
    fn_710019e76c(self);
    self->vtable = reinterpret_cast<char*>(g_unk_7101a1e0a8) + 0x10;
    for (u64& w : self->field_0x20)
        w = 0;
    new (&self->v0) base::UnkVector<void*>(0xf);
    self->field_0x80 = 0;
    new (&self->m) UnkIdentity44_218_b70a1();
    self->field_0xc8 = 0;
    self->field_0xd0 = 0;
    new (&self->h0) UnkPtrArrayHolder_b70a1(false);
    new (&self->h1) UnkPtrArrayHolder_b70a1(true);
    new (&self->h2) UnkPtrArrayHolder_b70a1(true);
    u8* block = self->field_0x1b0;
    u32* tail = self->field_0x29b4;
    __builtin_memset(block, 0, sizeof(self->field_0x1b0));
    for (u64 i = 0; i < 7; ++i)
        tail[i] = 0;
}
