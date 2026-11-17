// CHecathonPlanktonFXComponent (tools/tu_map.py unit 1271): the destructor. It drops the model at
// +0x12c0, takes every emitter's model off the render manager, clears the emitters, frees the list
// at +0x1298 and releases the path id before the base destructor. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "game/CHecathonPlanktonFXComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"
#include "types.h"

using base::global::CFilePathStrId;

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

struct UnkPlanktonFX_l808234 {
    void** vtable;
    u8 pad_0x8[0x40];
    CFilePathStrId path_0x48;
    UnkFixedVector_l808234<UnkPlanktonEmitter_l808234, 10> emitters;  // 0x50
    u8 pad_0x1274[0x20];
    f32 intensity;  // 0x1294
    base::global::CRntVector<u64> list_0x1298;
    UnkModel* model;  // 0x12c0
};
static_assert(__builtin_offsetof(UnkPlanktonFX_l808234, intensity) == 0x1294, "layout");
static_assert(__builtin_offsetof(UnkPlanktonFX_l808234, model) == 0x12c0, "layout");

static inline UnkItemBase_71011a91c4* Item_l808234(UnkModel* model) {
    return model ? reinterpret_cast<UnkItemBase_71011a91c4*>(reinterpret_cast<char*>(model) + 8) : nullptr;
}

void fn_7100808234(UnkPlanktonFX_l808234* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101a74d08 + 0x10);
    if (self->model) {
        fn_71002199d8(g_unk_7101d47300, Item_l808234(self->model), 0);
        self->model = nullptr;
    }
    for (u32 i = 0; i < self->emitters.count; ++i) {
        fn_7100219e18(g_unk_7101d47300, self->emitters.At(i).model, false);
        fn_71002199d8(g_unk_7101d47300, Item_l808234(self->emitters.At(i).model), 0);
        self->emitters.At(i).points.Clear();
    }
    self->emitters.Clear();
    self->list_0x1298.Free();
    self->emitters.~UnkFixedVector_l808234();
    self->path_0x48.~CFilePathStrId();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
