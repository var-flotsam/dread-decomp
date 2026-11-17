// CHecathonPlanktonFXComponent (tools/tu_map.py unit 1271): initialisation. Keeps the object at +0x10
// when it is a CCharClassHecathonPlanktonFXComponent, takes its path, loads the component's own model
// once and creates the ten emitters' models, each shown with a white colour. Returns the base
// method's result. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CSpittailMagmaBallMovementComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSlots.h"
#include "game/UnkTU127.h"
#include "game/UnkTU253.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::math::CVector4D;
using base::reflection::UnkReflObject;

struct UnkModel;

void fn_71001494f8(void* slot, u32 kind, u32 mode, CFilePathStrId* path, u32 flags);

// The component's own model's path (a static of this unit).
__attribute__((visibility("hidden"))) extern CFilePathStrId g_unk_7172722260;

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

struct UnkCharClassPlanktonFX_l80ca2c {
    u8 pad_0x0[0x58];
    CFilePathStrId path;  // 0x58
};

// The members of a model this unit reads.
struct UnkPlanktonModelView_l80ca2c {
    u8 pad_0x0[0x138];
    UnkModelData* data;  // 0x138
};

struct UnkPlanktonFX_l80ca2c {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkReflObject* char_class;  // 0x10
    u8 pad_0x18[0x30];
    CFilePathStrId path_0x48;
    UnkFixedVector_l808234<UnkPlanktonEmitter_l808234, 10> emitters;  // 0x50
    u8 pad_0x1274[0x4c];
    UnkModel* model;  // 0x12c0
};
static_assert(__builtin_offsetof(UnkPlanktonFX_l80ca2c, model) == 0x12c0, "layout");

static inline UnkSceneNode* Node_l80ca2c(UnkModel* model) {
    return model ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<char*>(model) + 8) : nullptr;
}

static inline UnkModelData* Data_l80ca2c(UnkModel* model) {
    return reinterpret_cast<UnkPlanktonModelView_l80ca2c*>(model)->data;
}

bool fn_710080ca2c(UnkPlanktonFX_l80ca2c* self, void* arg) {
    bool result = fn_71010c41bc(reinterpret_cast<UnkBaseComponent*>(self), arg);
    UnkReflObject* object = self->char_class;
    UnkReflObject* cast = object && fn_710009d50c(object->vf_0x0(), fn_7100805e14()) ? object : nullptr;
    self->char_class = cast;
    self->path_0x48 = CFilePathStrId(reinterpret_cast<UnkCharClassPlanktonFX_l80ca2c*>(cast)->path);
    if (!self->model) {
        CFilePathStrId path(g_unk_7172722260);
        fn_71001494f8(&self->model, 0xc8, 1, &path, 4);
    }
    for (u32 i = 0; i < 10; ++i) {
        UnkRenderManager* manager = g_unk_7101d47300;
        UnkPlanktonEmitter_l808234& emitter = self->emitters.At(i);
        {
            UnkModel* model = nullptr;
            fn_71006cae2c(manager->field_0x218, self->path_0x48, &model, &g_unk_7101d470e0);
            fn_7100219ae0(manager, Node_l80ca2c(model), 0);
            emitter.model = model;
        }
        fn_71001436a0(self->emitters.At(i).model, 0);
        fn_71001c0968(Data_l80ca2c(self->emitters.At(i).model), true);
        CVector4D colour = {1.0f, 1.0f, 1.0f, 1.0f};
        fn_71001c287c(Data_l80ca2c(self->emitters.At(i).model), 0, 0, &colour, 1);
        colour.w = 0.0f;
        fn_71001c2764(reinterpret_cast<UnkSlots*>(Data_l80ca2c(self->emitters.At(i).model)), 0, 0, &colour, 1,
                      true);
    }
    return result;
}
