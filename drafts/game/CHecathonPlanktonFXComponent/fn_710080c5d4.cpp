// CHecathonPlanktonFXComponent (tools/tu_map.py unit 1271): starts the effect. Every one of the ten
// emitters (created when missing) is switched on, the component is marked active and, on the first
// start of a pending fade, records `value` and the fade step; `value` is stored last.
// Names and layouts are placeholders.
#include <new>

#include "game/CHecathonPlanktonFXComponent.h"
#include "types.h"

struct UnkModel;

// A vector with a fixed capacity, its elements stored in place after the count.
template <class T, u32 N>
struct UnkFixedVector_l808234 {
    u32 count;
    u8 storage[sizeof(T) * N];

    UnkFixedVector_l808234() : count(0) {}

    T* items() { return reinterpret_cast<T*>(storage); }

    // The element at `i`, default-constructing the ones up to it when the vector is shorter.
    T& At(u32 i) {
        u32 n = i + 1;
        if (n > count) {
            for (u32 j = count; j < n; ++j)
                new (&items()[j]) T();
            count = n;
        }
        return items()[i];
    }

    void Clear() {
        for (u32 i = 0; i < count; ++i)
            items()[i].~T();
        count = 0;
    }

    ~UnkFixedVector_l808234() { Clear(); }
};

struct UnkPlanktonPoint_l808234 {
    u8 data[0xc];
};

struct UnkPlanktonEmitter_l808234 {
    u64 field_0x0 = 0;
    u64 field_0x8 = 0;
    f32 field_0x10 = -1.0f;
    u32 field_0x14;  // unset in the original
    UnkModel* model = nullptr;  // 0x18
    u64 field_0x20 = 0;
    UnkFixedVector_l808234<UnkPlanktonPoint_l808234, 34> points;  // 0x28
    u32 field_0x1c4 = 0;
    bool field_0x1c8 = false;
    bool field_0x1c9 = false;
    bool field_0x1ca = false;

    UnkPlanktonEmitter_l808234() {}
};
static_assert(sizeof(UnkPlanktonEmitter_l808234) == 0x1d0, "emitter size");

struct UnkPlanktonFX_A3_b71 {
    u8 pad_0x0[0x50];
    UnkFixedVector_l808234<UnkPlanktonEmitter_l808234, 10> emitters;  // 0x50
    u8 pad_0x1274[0x10];
    f32 field_0x1284;
    bool active;  // 0x1288
    bool field_0x1289;
    bool field_0x128a;
    f32 field_0x128c;
    f32 field_0x1290;
};
static_assert(__builtin_offsetof(UnkPlanktonFX_A3_b71, field_0x1290) == 0x1290, "layout");

void fn_710080c5d4(UnkPlanktonFX_A3_b71* self, f32 value) {
    for (u32 i = 0; i < 10; ++i)
        self->emitters.At(i).field_0x1c9 = true;
    self->active = true;
    if (!self->field_0x1289 && self->field_0x128a) {
        self->field_0x1289 = true;
        self->field_0x1290 = value;
        self->field_0x1284 = 0.01f;
    }
    self->field_0x128c = value;
}
