#include "base/UnkCloneRecords.h"
#include "base/UnkNamedRegistry.h"
#include "base/UnkTimedCurve.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBlackboard.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "game/UnkXformNode.h"
#include <new>

#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;

#define HIDDEN __attribute__((visibility("hidden")))
typedef u32 __attribute__((may_alias)) a_u32;

struct UnkMinimapOwner;
struct UnkMapData;
struct UnkBox;
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkFlagged;
void fn_7100e796c4(void* target, const UnkBox* box, const UnkBox* bounds);
const base::global::CStrId* fn_7100de9bf8(UnkGameManager* self);
struct UnkShape;
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};

struct UnkMinimapRect {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
    UnkMinimapRect() {}
    UnkMinimapRect(f32 x0, f32 y0, f32 x1, f32 y1) : min_x(x0), min_y(y0), max_x(x1), max_y(y1) {}
    UnkMinimapRect(const UnkMinimapRect& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
        ((a_u32*)this)[3] = ((const a_u32*)&o)[3];
    }
    UnkMinimapRect& operator=(const UnkMinimapRect& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
        ((a_u32*)this)[3] = ((const a_u32*)&o)[3];
        return *this;
    }
};

// The minimap owner's layout as these functions use it.
struct UnkMmOwner_b0199 {
    u8 unk_0x0[0x9c];
    f32 margin;  // 0x9c
    u8 unk_0xa0[0x70];
    CStrId field_0x110;
    u8 unk_0x118[0xd63d30 - 0x118];
    UnkMinimapRect view;  // 0xd63d30
    u8 unk_0xd63d40[0x10];
    f32 offset[2];        // 0xd63d50
    u8 unk_0xd63d58[0x14];
    f32 cam[9];           // 0xd63d6c
};

const CStrId* fn_7100de9bf8(UnkGameManager* self);
UnkShape* fn_7100976588(UnkFlagged* flagged);
void fn_7100e94254(UnkMinimapOwner* self);
extern const f32 g_unk_710164be30;

struct UnkMmMap_b0199 {
    u8 unk_0x0[0x68];
    f32 origin[2];
};
struct UnkMmCamOwner2_b0199 {
    u8 unk_0x0[0x68];
    UnkMmMap_b0199* map;  // 0x68
};

void fn_7100e906bc(UnkMmOwner_b0199* self) {
    f32* p = (f32*)((u8*)self + 0xd63d6c);
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    bool follow = false;
    if (player) {
        const CStrId* current = fn_7100de9bf8(g_unk_7101d47310);
        follow = self->field_0x110.data == current->data;
    }
    if (follow) {
        UnkShape* shape = fn_7100976588(*(UnkFlagged**)((u8*)player + 0x1f8));
        if (*((u8*)shape + 0x60))
            reinterpret_cast<void (*)(UnkShape*)>((*(void***)shape)[0x98 / 8])(shape);
        const f32* b = (const f32*)((u8*)shape + 0x40);
        f32 x0 = b[0], y0 = b[1], x1 = b[2], y1 = b[3];
        p[5] = 600.0f;
        p[3] = (x0 + x1) * 0.5f;
        p[4] = (y0 + y1) * 0.5f;
    }
    UnkMmMap_b0199* map = ((UnkMmCamOwner2_b0199*)self)->map;
    if (map) {
        const f32* pe = &g_unk_710164be30;
        f32 ox = map->origin[0];
        f32 cx = p[3], cy = p[4];
        f32 dx = cx - ox;
        f32 e = *pe;
        f32 e2 = e + e;
        p[0] = e2 * __builtin_floorf(dx / e2);
        p[0] = (e + map->origin[0]) + p[0];
        f32 dy = cy - map->origin[1];
        p[1] = e2 * __builtin_floorf(dy / e2);
        p[1] = (e + map->origin[1]) + p[1];
    }
    fn_7100e94254((UnkMinimapOwner*)self);
}

bool fn_7100deac04(UnkGameManager* self);
void fn_7100e9d7fc(UnkMinimapOwner* self);

struct UnkMmArg2_b0199 {
    u8 unk_0x0[0x20];
    bool field_0x20;
};

bool fn_7100e9d6f8(UnkMmOwner_b0199* self, UnkMmArg2_b0199* arg) {
    if (fn_7100deac04(g_unk_7101d47310))
        return false;
    if (arg->field_0x20) {
        const f32* p = (const f32*)((u8*)self + 0xd63d6c);
        fn_7100e906bc(self);
        f32 d0 = p[6] - p[0];
        f32 d1 = p[7] - p[1];
        f32 dist = d0 * d0 + d1 * d1;
        f32 d2 = p[8] - p[2];
        dist += d2 * d2;
        if (dist < 160000.0f) {
            f32 m = self->margin;
            f32 lo_x = p[6] - m;
            f32 lo_y = p[7] - m;
            f32 lo2_x = p[0] - m;
            f32 lo2_y = p[1] - m;
            f32 hi_x = p[6] + m;
            f32 hi_y = p[7] + m;
            f32 hi2_x = p[0] + m;
            f32 hi2_y = p[1] + m;
            UnkMinimapRect box(lo_x < lo2_x ? lo_x : lo2_x, lo_y < lo2_y ? lo_y : lo2_y,
                               hi_x > hi2_x ? hi_x : hi2_x, hi_y > hi2_y ? hi_y : hi2_y);
            fn_7100e796c4((u8*)self + 0x118, (const UnkBox*)&box, (const UnkBox*)&box);
        }
    }
    fn_7100e9d7fc((UnkMinimapOwner*)self);
    return false;
}

