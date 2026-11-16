// Unit 2566 (CMinimapManager::CTunableMinimapManager): hands the minimap's tuned colours to the
// render passes, and the current area's colours (by the area's kind and the map's state). Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2564.h"
#include "game/UnkTU2566.h"
#include "types.h"

using base::math::CVector3D;

// The minimap tunable's colours (g_unk_71727ca410).
struct UnkMinimapColors_8fb5c {
    u8 pad_0x0[0x24];
    CVector3D field_0x24;
    CVector3D field_0x30;
    CVector3D field_0x3c;
    CVector3D field_0x48;
    CVector3D field_0x54;
    CVector3D field_0x60;
    CVector3D field_0x6c;
    CVector3D field_0x78;
    u8 pad_0x84[0x24];
    CVector3D field_0xa8;
    CVector3D field_0xb4;
    CVector3D field_0xc0;
};
static_assert(__builtin_offsetof(UnkMinimapColors_8fb5c, field_0xc0) == 0xc0, "UnkMinimapColors_8fb5c layout");

// A colour with its alpha.
struct UnkColor_8fb5c {
    CVector3D rgb;
    f32 a;
};

struct UnkGameFlags_8fb5c {
    u8 pad_0x0[0x2441];
    bool field_0x2441;
};

// The map's state flags.
struct UnkMinimapState_8fb5c {
    bool field_0x0;
    bool field_0x1;
    bool field_0x2;
    u8 pad_0x3[0x8ad];
    f32 field_0x8b0;
};

struct UnkMinimapWorld_8fb5c {
    u8 pad_0x0[0x110];
    base::global::CStrId field_0x110;
    u8 pad_0x118[0xd63c50];
    UnkMinimapState_8fb5c state;
};
static_assert(__builtin_offsetof(UnkMinimapWorld_8fb5c, state) == 0xd63d68, "UnkMinimapWorld_8fb5c layout");

static inline const UnkMinimapColors_8fb5c& Colors() {
    return reinterpret_cast<const UnkMinimapColors_8fb5c&>(g_unk_71727ca410);
}

// Hands a vector to the render passes' parameter hash.
inline void SetVec3_8fb5c(u64 hash, CVector3D v) { fn_7100219294(g_unk_7101d47300, hash, &v.x, 6); }

// Hands an opaque colour to the render passes' parameter hash.
inline void SetColor_8fb5c(u64 hash, const CVector3D& rgb) {
    UnkColor_8fb5c c = {rgb, 1.0f};
    fn_7100219294(g_unk_7101d47300, hash, &c.rgb.x, 6);
}

void fn_7100e8fb5c(UnkMinimapWorld_8fb5c* self, bool colors, bool area) {
    if (colors) {
        const UnkMinimapColors_8fb5c& tunable = Colors();
        SetColor_8fb5c(0x44213fb099a861a3, tunable.field_0x48);
        SetColor_8fb5c(0x7f23f21cedda3536, tunable.field_0x54);
        SetVec3_8fb5c(0xb94590d0d0d7a38e, tunable.field_0x3c);
        SetVec3_8fb5c(0x2333d73f2bb4f379, tunable.field_0x60);
        SetVec3_8fb5c(0x48620bbd87ab24c0, tunable.field_0x6c);
        SetVec3_8fb5c(0xe134bd7d84d91c3b, tunable.field_0x78);
    }
    if (area) {
        UnkMinimapState_8fb5c* state = &self->state;
        const UnkMinimapColors_8fb5c& tunable = Colors();
        bool special = reinterpret_cast<UnkGameFlags_8fb5c*>(g_unk_7101d47310)->field_0x2441 &&
                       (self->field_0x110.data == g_unk_71727c7fe8.data ||
                        self->field_0x110.data == g_unk_71727c7ff0.data ||
                        self->field_0x110.data == g_unk_71727c8000.data);
        SetVec3_8fb5c(0x8326972bd3d03767, special ? tunable.field_0x30 : tunable.field_0x24);
        CVector3D color = state->field_0x2 ? tunable.field_0xb4 : tunable.field_0xa8;
        SetVec3_8fb5c(0x64a1192276c42eb8, {color.x * 0.5f, color.y * 0.5f, color.z * 0.5f});
        SetVec3_8fb5c(0x39e8a1b1791ff383, special || state->field_0x1
                                              ? tunable.field_0xc0
                                              : CVector3D{color.x * 3.0f, color.y * 3.0f, color.z * 3.0f});
        f32 params[3] = {1.0f, state->field_0x0 ? 1.0f : 0.0f, state->field_0x8b0};
        fn_7100219294(g_unk_7101d47300, 0x02187b0175c717fb, params, 6);
    }
}