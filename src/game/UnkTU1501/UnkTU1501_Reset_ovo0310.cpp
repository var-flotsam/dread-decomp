// CSamusMovement's unit (unit 1501): the movement reset on entering/leaving morph ball, the per-frame
// state clear, and the floor-slide check. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2689.h"
#include "game/UnkTU437.h"
#include <math.h>
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkMode_b152;
struct UnkRunInfo_b152;
struct UnkLeg_b152;

template <class R, class... A>
static inline R CallVf(void* self, u32 off, A... args) {
    return reinterpret_cast<R (*)(void*, A...)>((*reinterpret_cast<void***>(self))[off / 8])(self, args...);
}

struct UnkMode_b152 {
    void** vtable;
    u8 pad_0x8[0x8];
    s32 kind;  // 0x10
    u8 pad_0x14[0x188];
    UnkVec3f field_0x19c;
};

struct UnkTrackRec_b152 {
    u8 pad_0x0[0x18];
    f32 field_0x18;
    f32 field_0x1c;
};

// A link into a track's weak handle list: the context and callback it is given, and the handle it holds.
struct UnkTrackLink_b152 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
    UnkWeakHandle* handle;
};

struct UnkMove_b152 {
    void** vtable;  // 0x0
    u8 pad_0x8[0x28];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x44];
    f32 field_0x7c;  // 0x7c
    u8 pad_0x80[0x79];
    bool field_0xf9;  // 0xf9
    u8 pad_0xfa[0x2e];
    UnkWeakHandle* field_0x128;  // 0x128
    u8 pad_0x130[0xc];
    f32 field_0x13c;  // 0x13c
    f32 field_0x140;  // 0x140
    f32 field_0x144;  // 0x144
    u8 pad_0x148[0x90];
    f32 field_0x1d8;  // 0x1d8
    u8 pad_0x1dc[0x24];
    f32 field_0x200;  // 0x200
    u8 pad_0x204[0x1a4];
    bool field_0x3a8;  // 0x3a8
    u8 pad_0x3a9[0x23];
    u32 field_0x3cc;  // 0x3cc
    u8 pad_0x3d0[0x31c];
    f32 field_0x6ec;  // 0x6ec
    f32 field_0x6f0;  // 0x6f0
    u8 pad_0x6f4[0xd4];
    u32 field_0x7c8;  // 0x7c8
    u8 pad_0x7cc[0x18];
    bool field_0x7e4;  // 0x7e4
    u8 pad_0x7e5[0x1aab];
    u32 field_0x2290;  // 0x2290
    u8 pad_0x2294[0x4];
    f32 field_0x2298;  // 0x2298
    u8 pad_0x229c[0x5e];
    u8 field_0x22fa;  // 0x22fa
    u8 pad_0x22fb[0x1];
    u32 field_0x22fc;  // 0x22fc
    u8 field_0x2300;  // 0x2300
    u8 pad_0x2301[0x7];
    u8 field_0x2308;  // 0x2308
    u8 pad_0x2309[0x7];
    u8 field_0x2310;  // 0x2310
    u8 pad_0x2311[0x3];
    f32 field_0x2314;  // 0x2314
    u32 field_0x2318;  // 0x2318
    u8 pad_0x231c[0x4];
    u32 field_0x2320;  // 0x2320
    u8 field_0x2324;  // 0x2324
    u8 field_0x2325;  // 0x2325
    u8 pad_0x2326[0x2];
    u64 field_0x2328;  // 0x2328
    u8 pad_0x2330[0xc];
    u32 field_0x233c;  // 0x233c
    u32 field_0x2340;  // 0x2340
    u8 pad_0x2344[0x94];
    u32 field_0x23d8;  // 0x23d8
    u8 pad_0x23dc[0x22c];
    u8 field_0x2608;  // 0x2608
    u8 pad_0x2609[0x6d3];
    u32 field_0x2cdc[6];  // 0x2cdc
    u8 pad_0x2cf4[0x4];
    u8 field_0x2cf8[16];  // 0x2cf8
    UnkMode_b152* mode;  // 0x2d08
    u8 pad_0x2d10[0x8];
    u32 mode_count;  // 0x2d18
    u8 pad_0x2d1c[0x2474];
    u64 field_0x5190;  // 0x5190
    u8 field_0x5198;  // 0x5198
    u8 pad_0x5199[0x3];
    u32 field_0x519c;  // 0x519c
    u32 field_0x51a0;  // 0x51a0
    u16 field_0x51a4;  // 0x51a4
    u8 pad_0x51a6[0x2];
    f32 field_0x51a8;  // 0x51a8
    u32 field_0x51ac;  // 0x51ac
    u8 field_0x51b0;  // 0x51b0
    u8 pad_0x51b1[0x1b];
    u8 field_0x51cc;  // 0x51cc
    u8 pad_0x51cd[0x7];
    u8 field_0x51d4;  // 0x51d4
    u8 pad_0x51d5[0x3];
    UnkRefObject* field_0x51d8;  // 0x51d8
    UnkRefObject* field_0x51e0;  // 0x51e0
    u8 pad_0x51e8[0x2c];
    u32 field_0x5214;  // 0x5214
    u8 pad_0x5218[0x8];
    u8 field_0x5220;  // 0x5220
    u8 pad_0x5221[0x17];
    u64 field_0x5238;  // 0x5238
    u8 pad_0x5240[0x18];
    f32 field_0x5258;  // 0x5258
    u8 pad_0x525c[0x4];
    u8 timer[0xb0];  // 0x5260
    u32 field_0x5310;  // 0x5310
    u8 pad_0x5314[0x10];
    f32 field_0x5324;  // 0x5324
    f32 field_0x5328;  // 0x5328
    u8 pad_0x532c[0x3c];
    f32 field_0x5368;  // 0x5368
    u8 pad_0x536c[0x4];
    UnkTrackLink_b152 link;  // 0x5370
    u8 pad_0x5390[0x8];
    u8 field_0x5398;  // 0x5398
    u8 pad_0x5399[0xb7];
    u64 field_0x5450;  // 0x5450
    UnkRunInfo_b152* info;  // 0x5458
    u8 pad_0x5460[0x8];
    UnkLeg_b152* leg0;  // 0x5468
    UnkLeg_b152* leg1;  // 0x5470
};


struct UnkInventory;
struct UnkMorphBallMovement_b49a4;

void fn_71010da750(UnkMove_b152* self);
void fn_710034eaec(void* obj, bool flag);

struct UnkStaticZero_b152 {
    u32 value;
    UnkStaticZero_b152() : value(0) {}
};
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
    fn_71010d7b88(reinterpret_cast<UnkMorphBallMovement_b49a4*>(self), flag);
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
    if (void* gui = *reinterpret_cast<void**>(reinterpret_cast<u8*>(self->actor) + 0x1d8)) {
        fn_710002ffb4(reinterpret_cast<UnkGuiKeyList*>(reinterpret_cast<u8*>(gui) + 0xb10),
                      reinterpret_cast<base::global::CStrId*>(&g_unk_7172740ff0));
        fn_710002ffb4(reinterpret_cast<UnkGuiKeyList*>(reinterpret_cast<u8*>(gui) + 0xbc0),
                      reinterpret_cast<base::global::CStrId*>(&g_unk_7172740ff0));
    }
    block->field_0xa0 = 2;
}


struct UnkGroundPlayer_b52;
bool fn_71010d1598(UnkGroundPlayer_b52*);
struct UnkSlideState;
bool fn_7100996bc4(UnkSlideState* self);
struct UnkAimComponent;
bool fn_710034f8dc(UnkAimComponent* self);
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
            if (fn_710034f8dc(reinterpret_cast<UnkAimComponent*>(aim)))
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
