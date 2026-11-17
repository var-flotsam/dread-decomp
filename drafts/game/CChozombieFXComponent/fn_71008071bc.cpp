// CChozombieFXComponent's unit (tools/tu_map.py, unit 1270): creates the model of the component's path
// (+0x48) for each of its ten effect entries (+0x50: a count and the entries, which grow to the index
// asked for), puts it on layer 0 and switches it off. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CSpittailMagmaBallMovementComponent.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU127.h"
#include "types.h"

using base::global::CFilePathStrId;

extern const base::math::CVector2D g_unk_7101647780;

struct UnkModel;

// The model's scene node, at +8.
struct UnkModelParts_l8071bc {
    u8 pad_0x0[0x8];
    UnkSceneNode node;
};

// One effect entry; the original lays these out 4-aligned after the count.
struct __attribute__((packed, aligned(4))) UnkChozombieFXEntry_l8071bc {
    UnkModel* model;                    // 0x0
    base::math::CVector3D field_0x8;    // 0x8
    base::math::CVector3D field_0x14;   // 0x14
    base::math::CVector2D field_0x20;   // 0x20
    u32 field_0x28;                     // 0x28
    u8 pad_0x2c[0x4];
    u64 field_0x30;                     // 0x30
    bool field_0x38;                    // 0x38
    u8 pad_0x39[0x7];

    UnkChozombieFXEntry_l8071bc()
        : model(nullptr),
          field_0x8{g_unk_710164779c.x, g_unk_710164779c.y, g_unk_710164779c.z},
          field_0x14{g_unk_710164779c.x, g_unk_710164779c.y, g_unk_710164779c.z},
          field_0x20{g_unk_7101647780.x, g_unk_7101647780.y}, field_0x28(0), field_0x30(0), field_0x38(true) {}
};
static_assert(sizeof(UnkChozombieFXEntry_l8071bc) == 0x40, "UnkChozombieFXEntry_l8071bc layout");

struct UnkChozombieFXEntries_l8071bc {
    u32 count;
    UnkChozombieFXEntry_l8071bc data[10];

    // The entry at `index`, constructing those up to it first.
    UnkChozombieFXEntry_l8071bc& At(u64 index) {
        if (index >= count) {
            u64 i = count;
            for (; i <= index; ++i)
                new (&data[i]) UnkChozombieFXEntry_l8071bc();
            count = static_cast<u32>(i);
        }
        return data[index];
    }
};

struct UnkChozombieFX_l8071bc {
    u8 pad_0x0[0x48];
    CFilePathStrId path;                      // 0x48
    UnkChozombieFXEntries_l8071bc entries;    // 0x50
};
static_assert(__builtin_offsetof(UnkChozombieFX_l8071bc, entries) == 0x50, "UnkChozombieFX_l8071bc layout");

static inline UnkSceneNode* Node_l8071bc(UnkModel* model) {
    return model ? &reinterpret_cast<UnkModelParts_l8071bc*>(model)->node : nullptr;
}

void fn_71008071bc(UnkChozombieFX_l8071bc* self) {
    for (u64 i = 0; i < 10; ++i) {
        UnkRenderManager* render = g_unk_7101d47300;
        UnkModel*& model = self->entries.At(i).model;
        UnkModel* created = nullptr;
        fn_71006cae2c(render->field_0x218, self->path, &created, &g_unk_7101d470e0);
        fn_7100219ae0(render, Node_l8071bc(created), 0);
        model = created;
        fn_71001436a0(self->entries.At(i).model, 0);
        fn_710014364c(self->entries.At(i).model, 0);
    }
}
