// CChozombieFXComponent's unit (tools/tu_map.py, unit 1270): hides the effect. Each of the ten trail slots'
// model is taken off the render manager and its scene node released, the slots are cleared, then the base
// component's handler runs. Names and layouts are placeholders.
#include <new>

#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CComponent.h"
#include "game/UnkFixedVector_l808234.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkModel;


struct UnkChozombieTrail_l8073c0 {
    UnkModel* model;  // 0x0
    CVector3D field_0x8;
    CVector3D field_0x14;
    CVector2D field_0x20;
    u32 field_0x28;
    u32 field_0x2c;  // unset in the original
    u64 field_0x30;
    bool field_0x38;

    UnkChozombieTrail_l8073c0()
        : model(nullptr),
          field_0x8{g_unk_710164779c.x, g_unk_710164779c.y, g_unk_710164779c.z},
          field_0x14{field_0x8.x, field_0x8.y, field_0x8.z},
          field_0x20{g_unk_7101647780.x, g_unk_7101647780.y},
          field_0x28(0),
          field_0x30(0),
          field_0x38(true) {}
};
static_assert(sizeof(UnkChozombieTrail_l8073c0) == 0x40, "trail size");

struct UnkChozombieFX_l8073c0 {
    u8 pad_0x0[0x50];
    UnkFixedVector_l808234<UnkChozombieTrail_l8073c0, 10> trails;  // 0x50
};

// The model's scene node (its base at +0x8).
static inline UnkItemBase_71011a91c4* Node(UnkModel* model) {
    return model ? reinterpret_cast<UnkItemBase_71011a91c4*>(reinterpret_cast<char*>(model) + 8) : nullptr;
}

void fn_71008073c0(UnkChozombieFX_l8073c0* self) {
    for (u32 i = 0; i < 10; ++i) {
        fn_7100219e18(g_unk_7101d47300, self->trails.At(i).model, false);
        fn_71002199d8(g_unk_7101d47300, Node(self->trails.At(i).model), 0);
    }
    self->trails.Clear();
    fn_71010c422c(reinterpret_cast<UnkMorphBallLauncher_b57*>(self));
}
