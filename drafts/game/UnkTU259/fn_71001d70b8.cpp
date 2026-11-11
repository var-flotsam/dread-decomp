#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkPair_b0154v {
    u32 first;
    u32 second;
};

struct UnkEntry_b0154v {
    u32 count;
    UnkPair_b0154v items[16];
    u64 field_0x88;
    ~UnkEntry_b0154v() { count = 0; }
};
static_assert(sizeof(UnkEntry_b0154v) == 0x90, "entry size");

struct UnkViewport_b0154v {
    s32 x, y, width, height;
    f32 min_depth, max_depth;
};

struct UnkGfxV_b0154 {
    u8 pad_0x0[0x317a9e0];
    base::UnkVector<UnkEntry_b0154v> entries;  // 0x317a9e0
};

extern CStrId g_unk_716e595b98 __attribute__((visibility("hidden")));
extern f32 g_unk_7101a1e720[] __attribute__((visibility("hidden")));

u32 fn_7100132fc0();
u32 fn_7100132fd0();
void fn_710015a090(s32 a, s32 b, s32 c, s32 d);
void fn_71001d48cc(UnkGfxV_b0154* self, void* id, f32 value);
void fn_710015a3e0(u32 value);
u64 fn_710015abd0();
void fn_71001cdba4(UnkGfxV_b0154* self, UnkEntry_b0154v* entry);
void fn_7100160ab0(u32 index, s32 mode, u32 group);
void fn_7100160990(u32 index, s32 mode, u32 group);
void fn_71001d4dbc(UnkGfxV_b0154* self, void* object, UnkViewport_b0154v* viewport);
void fn_710015f770(u32 index, void* object, u32 group);
UnkEntry_b0154v* fn_7100159aa0();
void fn_710015c2d0(UnkEntry_b0154v* entry);
void fn_71001cafa0(UnkGfxV_b0154* self, s32 a, s32 b, s32 c);

void fn_71001d70b8(UnkGfxV_b0154* self, void** object_ref, u64* value_ref, bool flag, f32 a, f32 scale) {
    g_unk_7101a1e720[0] = 0.0f;
    g_unk_7101a1e720[1] = 0.0f;
    g_unk_7101a1e720[2] = a;
    g_unk_7101a1e720[3] = a;
    UnkViewport_b0154v viewport;
    u32 width = (u32)((f32)fn_7100132fc0() * scale);
    u32 height = (u32)((f32)fn_7100132fd0() * scale);
    fn_710015a090(0, 0, width, height);
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = width;
    viewport.height = height;
    viewport.min_depth = 0.0f;
    viewport.max_depth = 1.0f;
    {
        CStrId id(g_unk_716e595b98);
    }
    fn_71001d48cc(self, nullptr, 1.0f);
    void* object = *object_ref;
    fn_710015a3e0(0);
    u64 value = *value_ref;
    UnkEntry_b0154v entry;
    u64 token = fn_710015abd0();
    entry.count = 0;
    entry.field_0x88 = token;
    if (value) {
        __builtin_memcpy(&entry.items[0], &value, sizeof(value));
        entry.count = 1;
    }
    fn_71001cdba4(self, &entry);
    s32 mode = flag ? 2 : 0;
    fn_7100160ab0(0, mode, 1);
    fn_7100160990(0, mode, 1);
    fn_71001d4dbc(self, object, &viewport);
    fn_710015f770(0, nullptr, 1);
    base::UnkVector<UnkEntry_b0154v>& v = self->entries;
    u32 n = v.count - 1;
    typedef u32 __attribute__((may_alias)) UnkAliasCount;
    *reinterpret_cast<UnkAliasCount*>(&v.data[(s32)n].count) = 0;
    v.count = n;
    if (n != 0)
        fn_710015c2d0(v.data + n - 1);
    else
        fn_710015c2d0(fn_7100159aa0());
    fn_710015a3e0(1);
    g_unk_7101a1e720[0] = 0.0f;
    g_unk_7101a1e720[1] = 0.0f;
    g_unk_7101a1e720[2] = 1.0f;
    g_unk_7101a1e720[3] = 1.0f;
    fn_71001cafa0(self, 1, 1, 0);
}
