// tools/tu_map.py unit 211 (no class hint): a constructor over the base built by fn_710019e76c
// (fn_7100192994): sets the vtable g_unk_7101a1dc70, an array (step 16, flags 0x1a) reserved to 16 through
// fn_71000231f4, a pair of 1/30 rates, an identity 4x4 matrix at +0x88, a cleared 0x144-byte block, an
// allocator/flags pair and an array (flags 0xe) resized to 11 zeroed pointers, then three flags.
// Names and layouts are placeholders.
#include <new>

#include "UnkPtrArrayHolder_b70.h"
#include "base/UnkVector.h"
#include "game/UnkTU226.h"
#include "game/UnkTU27.h"
#include "types.h"

extern void* g_unk_7101a1dc70[];

namespace {

struct UnkIdentity44_211_b70a1 {
    f32 m[16];

    UnkIdentity44_211_b70a1() {
        __builtin_memset(m, 0, sizeof(m));
        m[15] = 1.0f;
        m[10] = 1.0f;
        m[5] = 1.0f;
        m[0] = 1.0f;
    }
};

}  // namespace

struct UnkObject211_b70a1 {
    void* vtable;
    u8 base_0x8[0x18];
    u64 field_0x20;
    u64 field_0x28;
    u32 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    base::UnkVector<u8> v0;  // 0x48
    u64 field_0x70;
    u64 field_0x78;
    f32 field_0x80;
    f32 field_0x84;
    UnkIdentity44_211_b70a1 m;  // 0x88
    u8 field_0xc8[0x144];
    UnkPtrArrayHolder_b70a1 h;  // 0x210
    bool field_0x258;
    bool field_0x259;
    bool field_0x25a;
};
static_assert(__builtin_offsetof(UnkObject211_b70a1, m) == 0x88, "layout");
static_assert(__builtin_offsetof(UnkObject211_b70a1, h) == 0x210, "layout");
static_assert(__builtin_offsetof(UnkObject211_b70a1, field_0x25a) == 0x25a, "layout");

void fn_7100192994(void* p) {
    UnkObject211_b70a1* self = static_cast<UnkObject211_b70a1*>(p);
    fn_710019e76c(self);
    self->vtable = reinterpret_cast<char*>(g_unk_7101a1dc70) + 0x10;
    self->field_0x20 = 0;
    self->field_0x28 = 0;
    self->field_0x30 = 0;
    self->field_0x38 = 0;
    self->field_0x40 = 0;
    self->v0.data = nullptr;
    self->v0.count = 0;
    self->v0.capacity = 0;
    self->v0.step = 0x10;
    self->v0.field_0x14 = true;
    self->v0.flags = 0x1a;
    self->v0.allocator = fn_7100083990();
    fn_71000231f4(&self->v0, 0x10);
    self->field_0x70 = 0;
    self->field_0x78 = 0;
    self->field_0x80 = 1.0f / 30.0f;
    self->field_0x84 = 1.0f / 30.0f;
    new (&self->m) UnkIdentity44_211_b70a1();
    UnkPtrArrayHolder_b70a1* h = &self->h;
    __builtin_memset(self->field_0xc8, 0, sizeof(self->field_0xc8));
    new (h) UnkPtrArrayHolder_b70a1(true);
    self->field_0x258 = true;
    self->field_0x259 = false;
    self->field_0x25a = true;
}
