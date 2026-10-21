// Fills a sound play description from a sound definition: copies the ids and settings, picks a
// random file from the given variation list (falling back to the first list, or to the default
// path), and randomises volume, pitch and pan within the definition's ranges and given scales.
#include "base/global/CStrId.h"
#include "types.h"

static inline f32 Min(f32 a, f32 b) { return b < a ? b : a; }

// A file-path string id (pool g_unk_7101d47200) with CStrId's copy-assignment.
struct UnkPathId {
    base::global::UnkStringData* data;

    UnkPathId& operator=(const UnkPathId& other) {
        if (data != other.data) {
            base::global::UnkStringData* d = other.data;
            base::global::UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
};

extern UnkPathId g_unk_7101d470e8;

extern "C" int rand();

struct UnkIdPair {
    u32 a;
    u32 b;
};

struct UnkPathList_0x50 {
    UnkPathId* data;
    u32 count;
    u8 pad_0xc[0x44];
};

struct UnkSoundDef {
    u8 pad_0x0[0x58];
    base::global::CStrId field_0x58;
    u8 pad_0x60[0x8];
    UnkPathList_0x50 field_0x68[16];
    UnkIdPair field_0x568;
    u8 pad_0x570[0x4];
    u32 field_0x574;
    u8 pad_0x578[0x8];
    u32 field_0x580;
    u32 field_0x584;
    f32 field_0x588;
    f32 field_0x58c;
    f32 field_0x590;
    f32 field_0x594;
    f32 field_0x598;
    f32 field_0x59c;
    u32 field_0x5a0;
    u32 field_0x5a4;
    f32 field_0x5a8;
    u32 field_0x5ac;
    u32 field_0x5b0;
    u32 field_0x5b4;
    u32 field_0x5b8;
    u32 field_0x5bc;
    u8 pad_0x5c0[0x4];
    u8 field_0x5c4;
};

struct UnkSoundDesc {
    UnkIdPair field_0x0;
    UnkIdPair field_0x8;
    u8 pad_0x10[0x8];
    base::global::CStrId field_0x18;
    UnkPathId field_0x20;
    u8 pad_0x28[0x10];
    f32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u8 pad_0x54[0x4];
    u32 field_0x58;
    u32 field_0x5c;
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u8 field_0x6c;
    bool field_0x6d;
};

static inline f32 RandVary(const f32& base, const f32& range) {
    f32 q = (f32)(rand() % 200 - 100) / 200.0f;
    return base + range * q;
}
static inline f32 Clamp(f32 v, f32 lo, f32 hi) { return v <= lo ? lo : Min(v, hi); }

void fn_7101037bc0(UnkSoundDef* self, UnkSoundDesc* out, f32* scales, u32 index) {
    const UnkIdPair& ids = self->field_0x568;
    UnkIdPair& valid = out->field_0x0;
    valid.a = ids.a < 7 ? ids.a : 0x7fffffff;
    valid.b = ids.b < 10 ? ids.b : 0x7fffffff;
    UnkIdPair& raw = out->field_0x8;
    raw.a = ids.a;
    raw.b = ids.b;
    out->field_0x18 = self->field_0x58;

    UnkPathId* path = &g_unk_7101d470e8;
    if (index != 0xffffffff && !(self->field_0x5a8 > 0.0f && rand() % 100 < (s32)self->field_0x5a8)) {
        UnkPathList_0x50& list = self->field_0x68[index];
        if (list.count != 0) {
            u32 i = rand() % list.count;
            path = &list.data[i];
        } else if (index != 0 && self->field_0x68[0].count != 0) {
            u32 i = rand() % self->field_0x68[0].count;
            path = &self->field_0x68[0].data[i];
        }
    }
    out->field_0x20 = *path;

    bool scaled = false;
    f32 s0 = -1.0f;
    if (scales) {
        s0 = scales[0];
        scaled = true;
    }
    f32 v = s0 > 0.0f ? s0 * self->field_0x588 : self->field_0x588;
    out->field_0x38 = Clamp(RandVary(v, self->field_0x58c), 0.0f, 2.0f);
    out->field_0x3c = self->field_0x584;
    out->field_0x40 = self->field_0x580;

    f32 s1 = -1.0f;
    if (scaled)
        s1 = scales[1];
    f32 p = s1 > 0.0f ? s1 * self->field_0x590 : self->field_0x590;
    out->field_0x44 = Clamp(RandVary(p, self->field_0x594), 0.0f, 2.0f);

    out->field_0x48 = Clamp(RandVary(self->field_0x598, self->field_0x59c), -1.0f, 1.0f);
    out->field_0x4c = self->field_0x5a0;
    out->field_0x50 = self->field_0x5a4;
    out->field_0x5c = self->field_0x5b0;
    out->field_0x60 = self->field_0x5b4;
    out->field_0x64 = self->field_0x5b8;
    out->field_0x68 = self->field_0x5bc;
    out->field_0x6c = self->field_0x5c4;
    out->field_0x6d = self->field_0x574 == 1;
    out->field_0x58 = self->field_0x5ac;
}
