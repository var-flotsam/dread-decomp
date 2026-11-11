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
#include <algorithm>
#include "../../../src/game_s7/UnkIter.h"
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

struct UnkSamusMovement;
struct UnkPlayer;
struct UnkAbility;
struct UnkOwner_b152;

struct UnkLeg_b152 {
    void** vtable;
};
void fn_71002d6074(UnkLeg_b152* leg, bool flag, f32 weight);
bool fn_710002fb70(void* timer);

void fn_71009eee34(UnkSamusMovement* movement, const u64* key, bool active, bool flag) {
    UnkMove_b152* self = reinterpret_cast<UnkMove_b152*>(movement);
    UnkGuiKeyList* list = reinterpret_cast<UnkGuiKeyList*>(self->timer);
    base::global::CStrId* id = reinterpret_cast<base::global::CStrId*>(const_cast<u64*>(key));
    bool before = fn_710002fb70(self->timer);
    if (active) {
        fn_710002fd10(list, id);
        CallVf<void>(self->leg0, 0x78);
        CallVf<void>(self->leg1, 0x78);
    } else {
        fn_710002ffb4(list, id);
    }
    bool after = fn_710002fb70(self->timer);
    if (before & after)
        return;
    f32 weight = flag ? 0.0f : 0.1f;
    fn_71002d6074(self->leg0, active, weight);
    fn_71002d6074(self->leg1, active, weight);
    self->field_0x5310 = 0;
}

struct UnkOwnerState_b152 {
    u8 pad_0x0[0x210];
    UnkMove_b152* field_0x210;
};
void fn_71010d9a1c(UnkMove_b152* self, UnkOwnerState_b152* arg);

struct UnkBlock5220_b152 {
    u8 field_0x0;
    u8 pad_0x1[0x147];
    f32 field_0x148;
    u8 pad_0x14c[0x2c];
    u8 field_0x178;
    u8 pad_0x179[0xb7];
    u64 field_0x230;
};

void fn_71009e9390(UnkMove_b152* self, UnkOwnerState_b152* other) {
    UnkBlock5220_b152* block = reinterpret_cast<UnkBlock5220_b152*>(reinterpret_cast<u8*>(self) + 0x5220);
    fn_71010d9a1c(self, other);
    self->field_0x2308 = 0;
    if (other) {
        f32 pending = other->field_0x210 ? 0.0f : -1.0f;
        __builtin_memcpy(&block->field_0x148, &pending, sizeof(pending));
        self->field_0x51a8 = 0.27f;
        if (UnkMove_b152* p = other->field_0x210) {
            u8 b = p->field_0x2608;
            if (b) {
                self->field_0x2324 = b;
                self->field_0x2325 = b;
            }
            if (p->field_0xf9) {
                UnkGameManager* gm = g_unk_7101d47310;
                const u8* bytes = reinterpret_cast<const u8*>(gm);
                if ((gm->field_0xc8 != 0 || gm->field_0x10c) && bytes[0xc4] && !bytes[0xc6])
                    self->field_0x2308 = 1;
            }
        }
    } else {
        block->field_0x148 = -1.0f;
    }
    block->field_0x0 = 1;
    block->field_0x178 = 0;
    block->field_0x230 = 0;
    __builtin_memset(self->field_0x2cdc, 0, sizeof(self->field_0x2cdc));
}

void fn_71009e9890(UnkMove_b152* self);
void fn_71009796dc(UnkMove_b152* self);
void fn_71009f115c();
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);

void fn_71009e9890(UnkMove_b152* self) {
    fn_71009796dc(self);
    void* value = CallVf<void*>(self, 0x2a8);
    CallVf<void>(*reinterpret_cast<void**>(reinterpret_cast<u8*>(self->actor) + 0x2a0), 0x178, value, true);
    CallVf<void>(self, 0x2b0, value);
    UnkWeakTarget* track = static_cast<UnkWeakTarget*>(self->actor->field_0x220);
    if (!track)
        return;
    UnkTrackLink_b152* link = &self->link;
    link->context = self;
    link->callback = fn_71009f115c;
    if (UnkWeakHandle* handle = link->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = link->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            link->handle = nullptr;
        }
    }
    // Keeps the movement in its own register across the handle update.
    asm("" : "+r"(self));
    fn_7100316018(track, &self->link.handle);
    fn_710117b3e8(self->link.handle ? self->link.handle->target : nullptr, link);
}
