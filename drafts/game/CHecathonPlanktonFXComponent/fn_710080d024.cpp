// CHecathonPlanktonFXComponent (tools/tu_map.py unit 1271): resets the effect. Every point of the ten
// emitters gets a render instance of the emitter's model at the origin, then the flags at +0x1288, the
// four scales at +0x1274 and the word at +0x12c8 are set to their defaults. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkMatrix44.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"
#include "game/UnkTU54.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkModel;

// A vector with a fixed capacity, its elements stored in place after the count (the layout of
// the destructor's UnkFixedVector_l808234; this unit's other accessor takes a 64-bit index).
template <class T, u32 N>
struct UnkFixedVector_l80d024 {
    u32 count;
    u8 storage[sizeof(T) * N];

    UnkFixedVector_l80d024() : count(0) {}

    T* items() { return reinterpret_cast<T*>(storage); }

    // The element at `i`, default-constructing the ones up to it when the vector is shorter.
    T& At(u64 i) {
        if (i >= count) {
            for (u64 j = count; j <= i; ++j)
                new (&items()[j]) T();
            count = i + 1;
        }
        return items()[i];
    }
};

// A point of an emitter: its render instance first.
struct UnkPlanktonPoint_l80d024 {
    u64 instance = 0;
    u64 field_0x8 = 0;
    bool field_0x10 = false;

    UnkPlanktonPoint_l80d024() {}
};
static_assert(sizeof(UnkPlanktonPoint_l80d024) == 0x18, "point size");

struct UnkPlanktonEmitter_l80d024 {
    u64 field_0x0 = 0;
    u64 field_0x8 = 0;
    f32 field_0x10 = -1.0f;
    u32 field_0x14;  // unset in the original
    UnkModel* model = nullptr;  // 0x18
    u64 field_0x20 = 0;
    UnkFixedVector_l80d024<UnkPlanktonPoint_l80d024, 17> points;  // 0x28
    u32 field_0x1c4 = 0;
    bool field_0x1c8 = false;
    bool field_0x1c9 = false;
    bool field_0x1ca = false;

    UnkPlanktonEmitter_l80d024() {}
};
static_assert(sizeof(UnkPlanktonEmitter_l80d024) == 0x1d0, "emitter size");

struct UnkPlanktonFX_l80d024 {
    void** vtable;
    u8 pad_0x8[0x40];
    CFilePathStrId path_0x48;
    UnkFixedVector_l80d024<UnkPlanktonEmitter_l80d024, 10> emitters;  // 0x50
    f32 field_0x1274;
    f32 field_0x1278;
    f32 field_0x127c;
    f32 field_0x1280;
    u8 pad_0x1284[4];
    bool field_0x1288;
    bool field_0x1289;
    bool field_0x128a;
    u8 pad_0x128b[9];
    f32 intensity;  // 0x1294
    base::global::CRntVector<u64> list_0x1298;
    UnkModel* model;  // 0x12c0
    u32 field_0x12c8;
};
static_assert(__builtin_offsetof(UnkPlanktonFX_l80d024, intensity) == 0x1294, "layout");
static_assert(__builtin_offsetof(UnkPlanktonFX_l80d024, field_0x12c8) == 0x12c8, "layout");

void fn_710080d024(UnkPlanktonFX_l80d024* self) {
    const UnkVec3f& zero = reinterpret_cast<const UnkVec3f&>(g_unk_710164779c);
    for (u64 i = 0; i < 10; ++i) {
        for (u64 j = 0; j < 17; ++j) {
            UnkMatrix44 m = fn_71000791a8(zero, zero, zero);
            fn_7100219dd0(g_unk_7101d47300, reinterpret_cast<u64>(self->emitters.At(i).model), &m, 0, 1,
                          &self->emitters.At(i).points.At(j).instance, 0);
        }
    }
    self->field_0x1288 = true;
    self->field_0x1289 = false;
    self->field_0x128a = false;
    self->field_0x1274 = 0.1f;
    self->field_0x1278 = 0.1f;
    self->field_0x127c = 0.1f;
    self->field_0x1280 = 0.1f;
    self->field_0x12c8 = 0;
}
