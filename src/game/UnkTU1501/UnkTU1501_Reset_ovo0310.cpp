// CSamusMovement's unit (unit 1501): the movement reset on entering/leaving morph ball, the per-frame
// state clear, and the floor-slide check. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CMovementComponent.h"
#include "game/UnkMode_b152.h"
#include "game/UnkMove_b152.h"
#include "game/UnkStaticZero_b152.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2689.h"
#include "game/UnkTU437.h"
#include <math.h>
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTrackLink_b152.h"
#include "game/UnkTrackRec_b152.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkMode_b152;
struct UnkRunInfo_b152;
struct UnkLeg_b152;

template <class R, class... A>
static inline R CallVf(void* self, u32 off, A... args) {
    return reinterpret_cast<R (*)(void*, A...)>((*reinterpret_cast<void***>(self))[off / 8])(self, args...);
}

struct UnkInventory;
struct UnkMorphBallMovement_b49a4;

void fn_71010da750(UnkMove_b152* self);
void fn_710034eaec(void* obj, bool flag);

inline const UnkStaticZero_b152& StaticZero_b152() {
    static UnkStaticZero_b152 s_value asm("g_unk_71726b5354");
    return s_value;
}

void fn_71009e76f4(UnkMove_b152* self) {
    fn_71010da750(self);
    if (self->field_0x3a8) {
        *(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x430) = 0;
        self->field_0x5398 = 0;
    } else {
        ResetRef(&self->field_0x51d8);
        ResetRef(&self->field_0x51e0);
        self->field_0x5214 = StaticZero_b152().value;
        self->field_0x5398 = 0;
    }
    if (UnkMode_b152* mode = self->mode) {
        CallVf<void>(mode, 0xc0, self->field_0x3a8);
    }
    if (void* obj = self->actor->field_0x2b0)
        fn_710034eaec(obj, self->field_0x3a8);
}


extern u8 g_unk_7172740ff0 __attribute__((visibility("hidden")));

struct UnkBlock5198_b152 {
    u8 field_0x0;
    u8 pad_0x1[3];
    u32 field_0x4;
    u32 field_0x8;
    u16 field_0xc;
    u8 pad_0xe[0x26];
    u8 field_0x34;
    u8 pad_0x35[7];
    u8 field_0x3c;
    u8 pad_0x3d[0x4b];
    u8 field_0x88;
    u8 pad_0x89[0x17];
    u64 field_0xa0;
    u8 pad_0xa8[0x158];
    u8 field_0x200;
    u8 pad_0x201[0xb7];
    u64 field_0x2b8;
};

typedef u64 __attribute__((aligned(4))) u64a;

struct UnkBlock22fa_b152 {
    u8 field_0x0;
    u8 pad_0x1[5];
    u8 field_0x6;
    u8 pad_0x7[0xf];
    u8 field_0x16;
    u8 pad_0x17[0x13];
    u16 field_0x2a;
};

void fn_71009e9268(UnkMove_b152* self, bool flag) {
    UnkBlock5198_b152* block = reinterpret_cast<UnkBlock5198_b152*>(reinterpret_cast<u8*>(self) + 0x5198);
    UnkBlock22fa_b152* small = reinterpret_cast<UnkBlock22fa_b152*>(reinterpret_cast<u8*>(self) + 0x22fa);
    CPlayerMovement_Reset(reinterpret_cast<UnkMorphBallMovement_b49a4*>(self), flag);
    u64a* cleared = reinterpret_cast<u64a*>(self->field_0x2cdc);
    small->field_0x0 = 0;
    self->field_0x22fc = 0;
    block->field_0x2b8 = 0;
    small->field_0x16 = 0;
    self->field_0x2314 = 0.0f;
    self->field_0x2318 = 0;
    self->field_0x2320 = 0;
    small->field_0x2a = 0;
    block->field_0x3c = 0;
    self->field_0x5190 = 0;
    __builtin_memset(cleared, 0, 24);
    block->field_0x0 = 1;
    block->field_0x8 = block->field_0x4;
    block->field_0xc = 0;
    self->field_0x233c = 0;
    self->field_0x2340 = 0;
    block->field_0x200 = 0;
    self->field_0x51b0 = 0;
    self->field_0x51a8 = 0.0f;
    self->field_0x51ac = 0;
    UnkMode_b152** modes = reinterpret_cast<UnkMode_b152**>(reinterpret_cast<u8*>(self) + 0x2d1c);
    for (UnkMode_b152** it = modes; it != modes + self->mode_count; ++it)
        CallVf<void>(*it, 0xa8, flag);
    block->field_0x34 = 0;
    small->field_0x6 = 0;
    block->field_0x88 = 0;
    if (void* gui = self->actor->field_0x1d8) {
        fn_710002ffb4(reinterpret_cast<UnkGuiKeyList*>(reinterpret_cast<u8*>(gui) + 0xb10),
                      reinterpret_cast<base::global::CStrId*>(&g_unk_7172740ff0));
        fn_710002ffb4(reinterpret_cast<UnkGuiKeyList*>(reinterpret_cast<u8*>(gui) + 0xbc0),
                      reinterpret_cast<base::global::CStrId*>(&g_unk_7172740ff0));
    }
    block->field_0xa0 = 2;
}


struct UnkGroundPlayer_b52;
struct UnkSlideState;
bool fn_7100996bc4(UnkSlideState* self);
struct UnkAimComponent_34f8dc;
struct UnkPlayer;

// The aim tunables block: the slide angle range.
struct UnkAimTunable {
    u8 pad_0x0[0xa8];
    f32 field_0xa8;
    u8 pad_0xac[0xc];
    f32 field_0xb8;
};

struct UnkAimState_b0153 {
    u8 pad_0x0[0x10];
    s32 field_0x10;
};

struct UnkAimComp_b0153 {
    u8 pad_0x0[0x50];
    UnkAimState_b0153* field_0x50;
};

struct UnkSlideOwner_b0153 {
    u8 pad_0x0[0x38];
    UnkMatrix44 field_0x38;
    u8 pad_0x78[0x220 - 0x78];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x2b0 - 0x228];
    UnkAimComp_b0153* field_0x2b0;
};

struct UnkSlideMovement_b0153 {
    u8 pad_0x0[0x30];
    UnkSlideOwner_b0153* owner;
    u8 pad_0x38[0xf9 - 0x38];
    bool field_0xf9;
    u8 pad_0xfa[0x154 - 0xfa];
    f32 field_0x154;
    f32 field_0x158;
    f32 field_0x15c;
    u8 pad_0x160[0x6e8 - 0x160];
    f32 field_0x6e8;
    f32 field_0x6ec;
    f32 field_0x6f0;
    u8 pad_0x6f4[0x7e4 - 0x6f4];
    bool field_0x7e4;
    u8 pad_0x7e5[0x2d08 - 0x7e5];
    UnkSlideState* field_0x2d08;
};

struct UnkSlideStateHdr_b0153 {
    u8 pad_0x0[0x10];
    s32 field_0x10;
};

template <const base::reflection::CClass* (*Getter)()>
static inline bool InTrack(UnkSlideMovement_b0153* self) {
    UnkAnimationComponent* comp = self->owner->field_0x220;
    return comp && fn_710117b348(comp, Getter());
}

bool fn_71009ea19c(UnkSlideMovement_b0153* self) {
    bool flag = self->field_0xf9;
    UnkSlideState* state = nullptr;
    if (self->field_0x2d08 && reinterpret_cast<UnkSlideStateHdr_b0153*>(self->field_0x2d08)->field_0x10 == 3)
        state = self->field_0x2d08;
    bool result;
    if (!flag) {
        result = false;
        UnkAimComp_b0153* aim = self->owner->field_0x2b0;
        if (state && !fn_7100996bc4(state))
            goto veto;
        if (self->field_0x7e4)
            goto veto;
        if (InTrack<fn_7100d001f8>(self) || InTrack<fn_7100d029c8>(self))
            goto veto;
        if (aim) {
            if (!aim->field_0x50 || aim->field_0x50->field_0x10 != 1)
                goto veto;
            if (fn_710034f8dc(reinterpret_cast<UnkAimComponent_34f8dc*>(aim)))
                goto veto;
        }
        if (!(self->field_0x6f0 > 0.0f))
            goto veto;
        if (!self->owner)
            goto veto;
        if (UnkAnimationComponent* comp = self->owner->field_0x220) {
            if (fn_710117b348(comp, fn_7100cf1628()))
                goto veto;
            f32 angle = atan2f(self->field_0x6ec, fabsf(self->field_0x6e8));
            if (!(angle > g_unk_71726b1438.field_0xa8) && !(angle < g_unk_71726b1438.field_0xb8)) {
                const UnkMatrix44& m = fn_710002614c(&self->owner->field_0x38);
                base::math::CVector2D flat;
                f32 ux = m.m[2];
                f32 uy = m.m[6];
                f32 uz = m.m[10];
                base::math::CVector3D up = {ux, uy, uz};
                fn_710007b430(&flat, &up);
                result = !(self->field_0x6e8 * flat.x + self->field_0x6ec * flat.y < 0.0f);
            }
        }
    } else {
    veto:
        result = false;
    }
    if (!fn_71010d1598(reinterpret_cast<UnkGroundPlayer_b52*>(self)))
        return result;
    if (state && !fn_7100996bc4(state))
        return result;
    if (UnkAnimationComponent* comp = self->owner->field_0x220)
        result = !fn_710117b348(comp, fn_7100c2d0cc());
    else
        result = true;
    if (!fn_7100df5874(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310)))
        return result;
    const UnkMatrix44& m = fn_710002614c(&self->owner->field_0x38);
    f32 up_x = m.m[2];
    f32 up_y = m.m[6];
    f32 up_z = m.m[10];
    f32 dot = up_x * self->field_0x154 + up_y * self->field_0x158 + up_z * self->field_0x15c;
    result &= !(dot < 0.0f);
    return result;
}
