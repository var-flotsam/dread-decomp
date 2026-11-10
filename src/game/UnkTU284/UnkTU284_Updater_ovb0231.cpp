// Unit 284 (tools/tu_map.py): two members of the updater whose part sits at +0x108
// (src/game_71001e2acc/fn_710020c780.cpp): fn_7100207628 sets the alpha of every slot of the part to a
// value (kept at +0x114), fn_71002076d4 hands the part its render resource, copies its settings and reads
// three floats from it by name hash. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkSlots.h"
#include "game/UnkSphere.h"
#include "types.h"

using base::math::CVector4D;
using base::reflection::UnkReflObject;

void fn_71001c287c(void* target, u32 a, u32 b, CVector4D* colour, u32 c);


struct UnkSlotOwner_b0231 {
    u8 pad_0x0[0x108];
    UnkSlots* part;  // 0x108
    u8 pad_0x110[4];
    f32 alpha;  // 0x114
};

void fn_7100207628(UnkSlotOwner_b0231* self, f32 alpha) {
    self->alpha = alpha;
    CVector4D colour;
    colour.x = 1.0f;
    colour.y = 1.0f;
    colour.z = 1.0f;
    colour.w = 1.0f;
    UnkSlots* part = self->part;
    s32 last = static_cast<s32>(part->info->count) - 1;
    if (last < 0)
        return;
    f32 a = alpha <= 0.0f ? 0.0f : alpha;
    for (u32 i = 0;; ++i) {
        fn_71001c287c(part, i, 0, &colour, 1);
        colour.w = a;
        fn_71001c2764(self->part, i, 0, &colour, 1, true);
        if (last == static_cast<s32>(i))
            break;
        part = self->part;
    }
}

// The updater's settings (the source's, copied member by member).
struct UnkSettings_b0231 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    bool field_0x34;
    bool field_0x35;

    UnkSettings_b0231& operator=(const UnkSettings_b0231& o) {
        field_0x0 = o.field_0x0;
        field_0x4 = o.field_0x4;
        field_0x8 = o.field_0x8;
        field_0xc = o.field_0xc;
        field_0x10 = o.field_0x10;
        field_0x14 = o.field_0x14;
        field_0x18 = o.field_0x18;
        field_0x20 = o.field_0x20;
        field_0x28 = o.field_0x28;
        field_0x2c = o.field_0x2c;
        field_0x30 = o.field_0x30;
        field_0x34 = o.field_0x34;
        field_0x35 = o.field_0x35;
        return *this;
    }
};

struct UnkSource_b0231 {
    u8 unk_0x0[0x28];
    f32 field_0x28;
    u8 unk_0x2c[0xc];
    UnkSettings_b0231 settings;  // 0x38
};

struct UnkUpdater_b0231 {
    void** vtable;
    u8 unk_0x8[0x70];
    UnkReflObject* field_0x78;
    u8 field_0x80[0x8];  // the start of an embedded UnkSphere
    f32 field_0x88;
    u8 unk_0x8c[0xc];
    UnkSettings_b0231 settings;  // 0x98
    u8 unk_0xd0[0x2c];
    bool field_0xfc;
    bool field_0xfd;
    u8 unk_0xfe[0xa];
    UnkSource_b0231* field_0x108;
    f32 field_0x110;
    f32 field_0x114;

    void vf_0x30(UnkSettings_b0231* s) {
        reinterpret_cast<void (*)(UnkUpdater_b0231*, UnkSettings_b0231*)>(vtable[6])(this, s);
    }
};

struct UnkSource_710020c780;
bool fn_71001c32bc(UnkSource_710020c780* source, const u64* key, u32 flag, f32* out);
void fn_71001c09c4(UnkSource_710020c780* source, UnkReflObject* obj);

static inline UnkSource_710020c780* Part(UnkSource_b0231* source) {
    return reinterpret_cast<UnkSource_710020c780*>(source);
}

// Hands the part its render resource (the object at +0x78 when it is one), takes its settings and
// reads the float values stored under three name hashes: the first is kept at +0x110 with whether it is
// positive at +0xfd, the second sets the sphere radius, the third says at +0xfc whether it is not zero.
// Last, the alpha of the part's first slot is kept at +0x114.
void fn_71002076d4(UnkUpdater_b0231* self) {
    UnkSource_b0231* source = self->field_0x108;
    UnkReflObject* obj = self->field_0x78;
    UnkReflObject* cast = nullptr;
    if (obj)
        cast = fn_710009d50c(obj->vf_0x0(), fn_71001b122c()) ? obj : nullptr;
    fn_71001c09c4(Part(source), cast);
    UnkSource_b0231* s = self->field_0x108;
    self->field_0x88 = s->field_0x28;
    self->settings = s->settings;
    self->vf_0x30(&self->settings);
    u64 radius_key = 0xd4fec4bd56e3c8bULL;
    u64 sphere_key = 0xdaa17bc43be5113fULL;
    u64 flag_key = 0xa2c48c1cbef004f4ULL;
    f32 first, second, third;
    CVector4D colour;
    first = 0.0f;
    if (fn_71001c32bc(Part(self->field_0x108), &radius_key, 1, &first)) {
        bool positive = first > 0.0f;
        self->field_0x110 = first;
        self->field_0xfd = positive;
    }
    second = 0.0f;
    if (fn_71001c32bc(Part(self->field_0x108), &sphere_key, 1, &second))
        fn_710021693c(reinterpret_cast<UnkSphere*>(self->field_0x80), second);
    third = 0.0f;
    if (fn_71001c32bc(Part(self->field_0x108), &flag_key, 1, &third))
        self->field_0xfc = !(third + -0.001f <= 0.0f) | !(third + 0.001f >= 0.0f);
    colour.x = 1.0f;
    colour.y = 1.0f;
    colour.z = 1.0f;
    colour.w = 1.0f;
    fn_71001c287c(self->field_0x108, 0, 0, &colour, 1);
    self->field_0x114 = colour.w;
}
