// Unit 2563: fills a minimap icon's box and string id from its object's position and facing (the id
// picked by the icon's kind and by which way it faces). Names and layouts are placeholders.
#include <new>

#include "base/UnkFreedVectorPair.h"
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2564.h"
#include "game/UnkTU55.h"
#include "game_s2/UnkItem50.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkVec3 {
    u32 a;
    u32 b;
    u32 c;
};

struct UnkMinimapIconOut {
    f32 x, y;
    f32 x1, y1, x2, y2;
    CStrId id;
};

struct UnkMinimapIconObj {
    u8 unk_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
};

struct UnkMinimapIconKind {
    u8 unk_0x0[0x124];
    u32 kind;
};

struct UnkMinimapIconSelf {
    u8 unk_0x0[0x18];
    u8* obj;  // 0x18
    u8 unk_0x20[0x468];
    UnkMinimapIconKind* kind;  // 0x488
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100e3cee0(UnkMinimapIconSelf* self, UnkMinimapIconOut* out) {
    u32 vb = *(u32*)(self->obj + 0x54);
    u32 va = *(u32*)(self->obj + 0x44);
    u32 vc = *(u32*)(self->obj + 0x64);
    UnkVec3 v = {va, vb, vc};
    fn_710007b444(out, &v);
    f32 z = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->obj + 0x38)).m[2];
    bool neg = z < 0.0f;
    f32 x = out->x;
    f32 y = out->y;
    f32 lo = neg ? -300.0f : 150.0f;
    f32 hi = neg ? -150.0f : 300.0f;
    out->x2 = x + hi;
    out->y2 = y + 300.0f;
    out->x1 = x + lo;
    out->y1 = y + 0.0f;
    const CStrId* id;
    switch (self->kind->kind) {
    case 0: id = neg ? &g_unk_71727c81e8 : &g_unk_71727c81f0; break;
    case 1: id = neg ? &g_unk_71727c81f8 : &g_unk_71727c8200; break;
    case 3: id = neg ? &g_unk_71727c8208 : &g_unk_71727c8210; break;
    case 4: id = neg ? &g_unk_71727c8218 : &g_unk_71727c8220; break;
    case 5: id = neg ? &g_unk_71727c8188 : &g_unk_71727c8190; break;
    default: return;
    }
    out->id = *id;
}
