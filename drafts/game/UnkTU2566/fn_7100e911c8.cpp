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

UnkGameManagerTarget* fn_7100decab0(UnkGameManager*);
UnkGameManagerTarget* fn_7100decab0(UnkGameManager* self);
void fn_7100e796c4(void* target, const UnkBox* box, const UnkBox* bounds);
bool fn_710113d83c(void*);
UnkMinimapRect fn_7100e94e10();

void fn_7100e911c8(UnkMmOwner_b0199* self) {
    UnkGameManagerTarget* target = fn_7100decab0(g_unk_7101d47310);
    if (!target || !target->field_0x248)
        return;
    UnkCameraCtrl* cam = fn_71010e2080(target->field_0x248);
    if (!cam || !*((u8*)cam + 0x34))
        return;
    if (!reinterpret_cast<bool (*)(UnkCameraCtrl*)>(cam->vtable[0x110 / 8])(cam))
        return;
    f32 margin = self->margin;
    const f32* p = (const f32*)((u8*)self + 0xd63d6c);
    UnkMinimapRect box(p[0] - margin, p[1] - margin, margin + p[0], margin + p[1]);
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    if (player) {
        void* x = *(void**)((u8*)player + 0x260);
        if (x && fn_710113d83c(x))
            box = UnkMinimapRect(0.0f, 0.0f, 0.0f, 0.0f);
    }
    UnkMinimapRect bounds = fn_7100e94e10();
    fn_7100e796c4((u8*)self + 0x118, (const UnkBox*)&box, (const UnkBox*)&bounds);
}

