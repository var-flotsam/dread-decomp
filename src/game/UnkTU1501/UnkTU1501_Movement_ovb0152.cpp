// Unit 1501 (CSamusMovement, tools/tu_map.py: 0x71009dc3bc-0x71009f16a8), a second batch of its functions
// (the first is src/game/CSamusMovement.cpp): the track-driven air friction (fn_71009e73cc, with the run
// speed fn_71009e754c it scales), track and state checks (fn_71009e5114, fn_71009e69c4, fn_71009e7f20),
// the animation requests (fn_71009e7a5c, fn_71009e7b78, fn_71009e7d9c, fn_71009e972c, fn_71009ec94c), the
// hit flags (fn_71009ecd98) and the leg and input updates. Names and layouts are placeholders.
#include <algorithm>

#include "base/UnkPooledStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAbilitySpeedBooster.h"
#include "game/CAnimationComponent.h"
#include "game/CBreakableTileGroup.h"
#include "game/CCharacterMovement.h"
#include "game/CGunComponent.h"
#include "game/CHyperBeamGun.h"
#include "game/CInventoryComponent.h"
#include "game/CMovementComponent.h"
#include "game/CPlayerInfo.h"
#include "game/CSamusGrappleMovementState.h"
#include "game/CSamusGunComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkHitFlags_b152.h"
#include "game/UnkItemProperties.h"
#include "game/UnkMatrix44_7100078c9c.h"
#include "game/UnkMode_b152.h"
#include "game/UnkMove_b152.h"
#include "game/UnkProbeResult.h"
#include "game/UnkSamusSettings.h"
#include "game/UnkStaticZero_b152.h"
#include "game/UnkTU1455.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2689.h"
#include "game/UnkTU363.h"
#include "game/UnkTU39.h"
#include "game/UnkTU54.h"
#include "game/UnkTrackLink_b152.h"
#include "game/UnkTrackRec_b152.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CCapsuleShape2D.h"
#include "game/logic/collision/UnkBvhTreeSweep.h"
#include "types.h"
#include "game/UnkAbility.h"

using base::global::CStrId;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkMode_b152;
struct UnkRunInfo_b152;
struct UnkLeg_b152;
struct UnkPlayer;
struct UnkAbility;
struct UnkInventory;
struct UnkSamusMovement;
struct UnkAbilitySet;
struct UnkStruct_710117af10;
struct UnkCls_710117af10;
struct UnkStruct_7101176ea8;
struct UnkRequest_7101176ea8;

// Calls virtual slot off of self.
template <class R, class... A>
static inline R CallVf(void* self, u32 off, A... args) {
    return reinterpret_cast<R (*)(void*, A...)>((*reinterpret_cast<void***>(self))[off / 8])(self, args...);
}

struct UnkLeg_b152 {
    void** vtable;
};

struct UnkMsg_b152 {
    s16 id;
    u8 field_0x2;
    u8 pad_0x3[9];
    f32 field_0xc;
};

struct UnkReq_b152 {
    u64 key;
    s64 field_0x8;
    u64 field_0x10;
};

struct UnkInput_b152 {
    u8 pad_0x0[0x58];
    u32 count;
    u8 data[1];
};

struct UnkEntry48_b152 {
    UnkRefObject* field_0x0;
    UnkRefObject* field_0x8;
    u8 pad_0x10[0x38];
} __attribute__((packed));

struct UnkSpanArg_b152 {
    const void* data;
    u64 count;
};

struct UnkOwner2e0_b152 {
    u8 pad_0x0[0x2e0];
    UnkAbilitySet* field_0x2e0;
};

struct UnkVec2_b152 {
    f32 x, y;
};

struct UnkHitNode_b152 {
    u8 pad_0x0[0x10];
    UnkHitNode_b152* field_0x10;
};
struct UnkHitInfo_b152 {
    UnkHitNode_b152* field_0x0;
    u8 pad_0x8[0xc];
    u8 field_0x14[1];
};

// The request that fn_710098ae04 takes (as src/game/CSamusGroundMovementState/CParams/CParams_Default_ovb0138.cpp):
// the reflected params base holding an unset track request, then a derived part.
struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b958;
extern void* g_unk_7101a86900[];
struct UnkAirTarget_B71a6;
struct UnkAirRequest_B71a6;

struct UnkTrackTime_b152 {
    u64 key;
    u64 time;
};
struct UnkTrackBlend_b152 {
    u32 a, b, c;
};

struct UnkTrackParams_b152 {
    s64 id;
    UnkTrackTime_b152 time;
    UnkTrackBlend_b152 blend;
    u32 flags;
    u32 mask;

    __attribute__((always_inline)) UnkTrackParams_b152(UnkTrackTime_b152 time_, UnkTrackBlend_b152 blend_, u32 mask_)
        : id(-1), time(time_), blend(blend_), flags(0), mask(mask_) {}
    __attribute__((always_inline)) UnkTrackParams_b152()
        : UnkTrackParams_b152(reinterpret_cast<const UnkTrackTime_b152&>(g_unk_710164b958),
                              reinterpret_cast<const UnkTrackBlend_b152&>(g_unk_7172769740),
                              base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>()) {}
};

struct UnkParamsVtable_b152 {
    void* vtable;
    __attribute__((always_inline)) UnkParamsVtable_b152() { vtable = g_unk_7101a22cb8 + 2; }
};

template <void** VTABLE>
struct UnkRequest_b152 : UnkParamsVtable_b152 {
    UnkTrackParams_b152 params;  // 0x8
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;

    __attribute__((always_inline)) UnkRequest_b152() {
        field_0x38 = 3;
        __builtin_memset(&field_0x3c, 0, 8);
        field_0x44 = 0;
        vtable = VTABLE + 2;
    }
};

struct UnkTrackResult_b152 {
    void** vtable;
    void vf_0x18(UnkTrackParams_b152* params) {
        reinterpret_cast<void (*)(UnkTrackResult_b152*, UnkTrackParams_b152*)>(vtable[0x18 / 8])(this, params);
    }
};

char* fn_71009e5330(UnkMove_b152* self, char* a1, char* a2, char* a3, char* a4, char* a5);
f32 fn_71007965a8(UnkRunInfo_b152* info);
f32 fn_71007965c0(UnkRunInfo_b152* info);
void fn_7100976940(UnkMove_b152* self);
void fn_71009767f0(UnkMove_b152* self);
base::UnkPooledString* fn_71009e7870(UnkMove_b152* self);
bool fn_7100df45ec(UnkGameManager* self);
void fn_71002d6074(UnkLeg_b152* leg, bool flag, f32 weight);
void fn_71009f115c();
void fn_71010dc54c(UnkMove_b152* self, UnkSpanArg_b152* span);
void fn_71002fdf5c(void* obj, void* arg);
void fn_71010d956c(UnkMove_b152* self);
bool fn_710085aa5c(void* gun, void* tile, UnkHitInfo_b152* hit);
struct UnkQueryObj;
// Out of line: fn_71009e73cc calls it.

struct UnkComponent_710035131c;
struct UnkGunComponent;
struct UnkGun_ovb0091;
void fn_7100846e64(void* beam, u32 mode);
void fn_710083ed38(UnkGun_ovb0091* self);
bool fn_710035343c(void* obj);
void fn_710034f8d8(void* obj);

static inline bool SameId_b152(const CStrId& a, const CStrId& b) {
    return a.data == b.data;
}

struct UnkTree;
struct UnkShape;
struct UnkMtx44;
struct Vec2;

extern base::global::CStrId g_unk_7172740fe0 __attribute__((visibility("hidden")));

struct UnkVec4_b152 {
    UnkVec2_b152 lo, hi;
};

// The capsule shape built from four floats.
struct UnkCapsule_b152 {
    u8 storage[0xc0];
    UnkCapsule_b152(const UnkVec4_b152* v) asm("fn_7100d5ffb4");
    ~UnkCapsule_b152() { fn_7100d6006c(this); }
};

struct UnkColliderNode_b152 {
    void** vtable;
    u8 pad_0x8[0xa0];
    f32 field_0xa8;
    f32 field_0xac;
    u8 pad_0xb0[4];
    f32 field_0xb4;
};
struct UnkColliderObj_b152 {
    u8 pad_0x0[0x158];
    UnkColliderNode_b152* field_0x158;
};

static inline const UnkQualifiedId& GetQualifiedId_b152(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), base::global::CStrId::Empty());
    return qid;
}

static inline void ReleaseRef_b152(UnkRefObject* p) {
    if (p) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
}

inline const UnkStaticZero_b152& StaticZero_b152() {
    static UnkStaticZero_b152 s_value asm("g_unk_71726aec10");
    return s_value;
}

// `arg` (x1) reaches fn_7100978f58 untouched in the original.
void fn_71009e11c0(UnkMove_b152* self, const void* arg) {
    CCharacterMovement_SetOwnerPositionDroppedToGround(reinterpret_cast<char*>(self), arg);
    UnkProbeResult result;
    result.field_0x0 = 0;
    result.field_0x8 = 0;
    UnkActor* actor = self->actor;
    f32 x = actor->field_0x44, y = actor->field_0x54, z = actor->field_0x64;
    CVector3D pos = {x, y, z};
    f32 radius = *reinterpret_cast<f32*>(static_cast<char*>(actor->field_0x228) + 0x60);
    if (!fn_71010d15d0(reinterpret_cast<char*>(self), &pos, &result, radius))
        return;
    if (self->field_0xf9)
        CallVf<void>(self, 0x318);
    if (self->field_0x3a8 != 1) {
        self->field_0x3a8 = 1;
        CallVf<void>(self, 0x2b8);
    }
}

void ApplySamusForwardBackwardScaleOrReset(UnkMove_b152* self, f32* values) {
    if (self->field_0x5258 != 1.0f) {
        static CStrId s_forward("forward", true);
        static CStrId s_backward("backward", true);
        UnkStruct_710117af10* anim = static_cast<UnkStruct_710117af10*>(self->actor->field_0x220);
        if (anim && (CAnimationComponent_IsPlayingActionNameContaining(anim, reinterpret_cast<UnkCls_710117af10**>(&s_forward), false) ||
                     CAnimationComponent_IsPlayingActionNameContaining(static_cast<UnkStruct_710117af10*>(self->actor->field_0x220),
                                   reinterpret_cast<UnkCls_710117af10**>(&s_backward), false))) {
            values[0x6c / 4] = self->field_0x5258 * values[0x6c / 4];
            values[0x60 / 4] = self->field_0x5258 * values[0x60 / 4];
        } else {
            self->field_0x5258 = 1.0f;
        }
    }
}

f32 fn_71009e5114(UnkMove_b152* self) {
    UnkTrackQuery query = {fn_7100d30c28(), nullptr, -1.0f};
    f32 a;
    f32 b = 0.0f;
    if (self->actor->field_0x220)
        fn_710117b08c(static_cast<UnkAnimationComponent*>(self->actor->field_0x220), &query);
    if (query.result) {
        UnkTrackRec_b152* rec = reinterpret_cast<UnkTrackRec_b152*>(query.result);
        a = rec->field_0x18;
        b = rec->field_0x1c;
    } else {
        a = 3700.0f;
    }
    f32 result = self->field_0x200 < b ? 3700.0f : a;
    void* cls = self->actor->field_0x220;
    if (cls && fn_710117b348(static_cast<UnkAnimationComponent*>(cls), fn_7100d001f8()))
        result = *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_717273ba88) + 0xa8) * 3700.0f;
    return result;
}

void fn_71009e51dc(UnkMove_b152* self, void* a1, void* a2, UnkInput_b152* input) {
    UnkSpanArg_b152 span = {input->data, input->count};
    fn_71010dc54c(self, &span);
    span = {input->data, input->count};
    CallVf<void>(self, 0x408, &span, a2, input);
    if (void* obj = self->actor->field_0x2e0)
        fn_71002fdf5c(obj, input);
    CallVf<void>(self->mode, 0x90, a1, a2, input);
    UnkEntry48_b152* entries = reinterpret_cast<UnkEntry48_b152*>(reinterpret_cast<u8*>(self) + 0x23dc);
    for (u64 i = 0; i < self->field_0x23d8; ++i) {
        ReleaseRef_b152(entries[i].field_0x8);
        ReleaseRef_b152(entries[i].field_0x0);
    }
    self->field_0x23d8 = 0;
}

char* fn_71009e5314(UnkMove_b152* self, char* a1, char* a2, char* a3, char* a4, char* a5) {
    if (UnkMode_b152* mode = self->mode)
        return CallVf<char*>(mode, 0x88, a1, a2, a3, a4, a5);
    return fn_71009e5330(self, a1, a2, a3, a4, a5);
}

bool fn_71009e69c4(UnkMove_b152* self) {
    if (self->field_0x3a8)
        return false;
    if (!self->field_0xf9)
        return false;
    void* cls = self->actor->field_0x220;
    if (cls && fn_710117b348(static_cast<UnkAnimationComponent*>(cls), fn_7100d029c8()))
        return false;
    if (CallVf<bool>(self, 0x3b8))
        return false;
    if (self->field_0x2314 > 0.0f)
        return false;
    UnkGameManager** manager = &g_unk_7101d47310;
    if (fn_7100f0ca8c((*manager)->field_0x38))
        return false;
    if (fn_710117b348(static_cast<UnkAnimationComponent*>(self->actor->field_0x220), fn_7100d0ccb8()))
        return false;
    cls = self->actor->field_0x220;
    if (cls && fn_710117b348(static_cast<UnkAnimationComponent*>(cls), fn_7100d24338()))
        return false;
    if (UnkAbilitySet* abilities = reinterpret_cast<UnkOwner2e0_b152*>(self->actor)->field_0x2e0) {
        if (UnkAbility* ability = fn_71003013ec(abilities, fn_7100308c74())) {
            if (fn_710009d50c(ability->vf_0x0(), fn_7100306914())) {
                if (fn_7100309568(ability) - 3 < 4)
                    return false;
            }
        }
    }
    return !fn_7100df5a4c(*manager);
}

bool GetMovementSpeedLimitAndDecayRate(UnkMove_b152* self, UnkVec2_b152* out) {
    out->x = 0.0f;
    out->y = 10.0f;
    UnkTrackQuery query = {fn_7100d2caa8(), nullptr, -1.0f};
    if (void* cls = self->actor->field_0x220)
        fn_710117b08c(static_cast<UnkAnimationComponent*>(cls), &query);
    if (query.result) {
        out->y = reinterpret_cast<UnkTrackRec_b152*>(query.result)->field_0x18;
        out->x = fn_71009e754c(self) * reinterpret_cast<UnkTrackRec_b152*>(query.result)->field_0x1c;
        return true;
    }
    void* cls = self->actor->field_0x220;
    if (!cls)
        return true;
    if (fn_710117b348(static_cast<UnkAnimationComponent*>(cls), fn_7100d272e8())) {
        out->y = 1.0f;
        out->x = fn_71009e754c(self);
        return true;
    }
    cls = self->actor->field_0x220;
    if (!cls)
        return true;
    if (fn_710117b348(static_cast<UnkAnimationComponent*>(cls), fn_7100d001f8())) {
        u32 bits = 0x7f7fffff;
        __builtin_memcpy(out, &bits, sizeof(bits));
        out->y = 0.0f;
        return true;
    }
    cls = self->actor->field_0x220;
    if (!cls)
        return true;
    if (CAnimationComponent_IsPlayingActionNameContaining(static_cast<UnkStruct_710117af10*>(cls),
                      reinterpret_cast<UnkCls_710117af10**>(&GameManagerIdTable().field_0x13f8), false)) {
        out->y = 1.0f;
        out->x = fn_71009e754c(self);
    }
    return true;
}

f32 fn_71009e754c(UnkMove_b152* self) {
    f32 value;
    void* cls = self->actor->field_0x220;
    if (cls && fn_710117b348(static_cast<UnkAnimationComponent*>(cls), fn_7100d272e8()))
        value = fn_71007965a8(self->info);
    else if (self->field_0x7e4)
        value = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(self->info) + 0xe4);
    else
        value = fn_71007965c0(self->info);
    if (UnkAbility* ability = fn_7100df5928(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310))) {
        if (fn_710033cf20(ability))
            value *= *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_71726b0fc0) + 0x30);
    }
    return value;
}

void fn_71009e7a5c(UnkMove_b152* self, UnkReq_b152* req) {
    fn_71009767f0(self);
    if (g_unk_7172740de0.field_0x70 > 0.0f) {
        if (req->key == 0xd77b18a12b71b1f8ull) {
            UnkMsg_b152 msg;
            fn_71002878dc(&msg);
            bool flag = false;
            CallVf<void>(self, 0x2e8, &msg, &flag, 0ull, 0ull);
            if (msg.id != -1 && msg.field_0x2 != 0xff && !flag && msg.field_0xc < g_unk_7172740de0.field_0x70)
                req->key = 0x19758dba5538d219ull;
            fn_7100287954(&msg);
        }
        base::UnkPooledString* id = fn_71009e7870(self);
        if (id->data != &g_unk_7101d47108->empty) {
            req->key = id->data->Key();
            req->field_0x8 = -1;
            req->field_0x10 = 0x13df5c28full;
        }
    }
}

void fn_71009e7b78(UnkMove_b152* self, UnkReq_b152* req) {
    fn_7100976940(self);
    if (g_unk_7172740de0.field_0x74 > 0.0f && req->key == 0x58b21e9778e2956dull) {
        UnkMsg_b152 msg;
        fn_71002878dc(&msg);
        bool flag = false;
        CallVf<void>(self, 0x2e8, &msg, &flag, 0ull, 0ull);
        if (msg.id != -1 && msg.field_0x2 != 0xff && !flag && msg.field_0xc < g_unk_7172740de0.field_0x74)
            req->key = 0xc5709d9896d698e0ull;
        fn_7100287954(&msg);
    }
}

void fn_71009e7d9c(UnkMove_b152* self) {
    fn_71010d956c(self);
    UnkAnimationComponent* anim = static_cast<UnkAnimationComponent*>(self->actor->field_0x220);
    if (!anim)
        return;
    UnkTrackQuery query = {fn_7100c5797c(), nullptr, -1.0f};
    if (!fn_710117b08c(anim, &query))
        return;
    UnkTrackParams_b152 params;
    reinterpret_cast<UnkTrackResult_b152*>(query.result)->vf_0x18(&params);
    CAnimationComponent_SendActionRequestRecordToTrack(reinterpret_cast<UnkStruct_7101176ea8*>(anim), reinterpret_cast<const UnkRequest_7101176ea8*>(&params),
                  0xf4061ff25a2163d6ull);
}

bool fn_71009e7f20(UnkMove_b152* self) {
    UnkInventory* inventory = self->actor->field_0x270;
    if (!fn_71011428bc(inventory, GetQualifiedId_b152(g_unk_717275a8d8)))
        return false;
    if (self->field_0x51a8 > 0.0f)
        return false;
    if (!fn_7100df45ec(g_unk_7101d47310))
        return false;
    void* cls = self->actor->field_0x220;
    if (!cls)
        return true;
    if (fn_710117b348(static_cast<UnkAnimationComponent*>(cls), fn_7100d0f728()))
        return false;
    cls = self->actor->field_0x220;
    if (!cls)
        return true;
    return !fn_710117b348(static_cast<UnkAnimationComponent*>(cls), fn_7100d089f8());
}

bool fn_71009e8218(UnkMove_b152* self, const UnkRequest_7101176ea8* request, bool flag) {
    if (flag) {
        if (void* gun = self->actor->field_0x2a8)
            fn_710083ed38(static_cast<UnkGun_ovb0091*>(gun));
    }
    if (void* gun = self->actor->field_0x2a8) {
        const base::global::CStrId* current = fn_71008407fc(static_cast<UnkGunComponent*>(gun));
        if (SameId_b152(*current, GameManagerIdTable().field_0x1848)) {
            UnkReflObject* beam =
                static_cast<UnkReflObject*>(fn_710083cd30(static_cast<UnkComponent_710035131c*>(gun)));
            if (beam && !fn_710009d50c(beam->vf_0x0(), fn_71008424e8()))
                beam = nullptr;
            if (fn_7100845fac(beam))
                fn_7100846e64(beam, 0x10);
        }
    }
    void* object = self->actor->field_0x2b0;
    if (fn_710035343c(object)) {
        UnkXform* xform = reinterpret_cast<UnkXform*>(self->actor->field_0x38);
        UnkVec3f angle = xform->GetAngle();
        UnkVec3f flipped;
        flipped.x = angle.x;
        flipped.y = -angle.y;
        flipped.z = angle.z;
        CallVf<void>(self->actor, 0xd8, &flipped, true);
    }
    void* cls = self->actor->field_0x220;
    if (!cls)
        return false;
    if (!CAnimationComponent_SendActionRequestRecordToTrack(static_cast<UnkStruct_7101176ea8*>(cls), request, 0xf4061ff25a2163d6ull))
        return false;
    fn_710034f8d8(object);
    return true;
}

void fn_71009e972c(UnkMove_b152* self) {
    UnkRequest_b152<g_unk_7101a22d18> req;
    fn_710098ae04(reinterpret_cast<UnkAirTarget_B71a6*>(self->field_0x2cf8), reinterpret_cast<UnkAirRequest_B71a6*>(&req));
}

void fn_71009e9890(UnkMove_b152* self) {
    fn_71009796dc(self);
    void* value = CallVf<void*>(self, 0x2a8);
    CallVf<void>(self->actor->field_0x2a0, 0x178, value, true);
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
    CAbilityGhostAura_AssignWeakHandleToRef(track, reinterpret_cast<UnkWeakRef*>(&self->link.handle));
    fn_710117b3e8(self->link.handle ? self->link.handle->target : nullptr, link);
}

bool IsCapsuleSweepAheadInFacingDirectionClear(UnkMove_b152* self) {
    if (!self->field_0x3a8)
        return false;
    UnkCollisionContainer* container = self->actor->field_0x2a0;
    UnkColliderObj_b152* object = reinterpret_cast<UnkColliderObj_b152*>(fn_71008640a4(container, &g_unk_7172740fe0));
    UnkColliderNode_b152* candidate = object->field_0x158;
    UnkColliderNode_b152* node = nullptr;
    if (candidate) {
        if (CallVf<s32>(candidate, 0x18) == 8)
            node = candidate;
    }
    f32 top = node->field_0xb4;
    UnkVec4_b152 box = {{node->field_0xa8, node->field_0xac}, {0.0f, top}};
    UnkCapsule_b152 shape(&box);
    f32 y = box.hi.y + 5.0f;
    f32 zero = 0.0f;
    f32 scaled = y * zero;
    UnkVec3f pos(self->field_0x13c + scaled, y + self->field_0x140, scaled + self->field_0x144);
    f32 delta[2];
    {
        UnkActor* actor = self->actor;
        UnkXform* xform = reinterpret_cast<UnkXform*>(actor->field_0x38);
        UnkVec3f angle = xform->GetAngle();
        f32 sign = (f32)((angle.y > 0.0f) - (angle.y < 0.0f));
        delta[0] = sign * 80.0f;
        delta[1] = sign * zero;
    }
    void* tracker = g_unk_7101d47310->field_0x608->field_0x820;
    u32 mask = self->field_0x3cc;
    UnkMatrix44_7100078c9c mtx = BuildTranslationMatrixFromVec3(pos);
    bool hit = SweepShapeAgainstCollisionTree(static_cast<UnkTree*>(tracker), mask, reinterpret_cast<UnkShape*>(&shape),
                             reinterpret_cast<const UnkMtx44*>(&mtx), reinterpret_cast<const Vec2*>(delta), nullptr, false);
    return !hit;
}

void fn_71009ec7b4(UnkMove_b152* self) {
    UnkReflObject* gun = static_cast<UnkReflObject*>(
        self->actor->field_0x2a8);
    if (gun && fn_710009d50c(gun->vf_0x0(), fn_7100857564()))
        fn_710085b76c(gun, true);
    UnkAirTarget_B71a6* target = reinterpret_cast<UnkAirTarget_B71a6*>(self->field_0x2cf8);
    CallVf<void>(self->mode, 0x60);
    UnkRequest_b152<g_unk_7101a86900> req;
    fn_710098ae04(target, reinterpret_cast<UnkAirRequest_B71a6*>(&req));
}

void fn_71009ec94c(UnkMove_b152* self) {
    UnkRequest_b152<g_unk_7101a22d18> req;
    fn_710098ae04(reinterpret_cast<UnkAirTarget_B71a6*>(self->field_0x2cf8), reinterpret_cast<UnkAirRequest_B71a6*>(&req));
}

UnkHitFlags_b152 fn_71009ecd98(UnkMove_b152* self, UnkHitInfo_b152* hit) {
    u32 flags = StaticZero_b152().value;
    UnkHitFlags_b152 result(flags);
    base::reflection::UnkReflObject* tile =
        reinterpret_cast<base::reflection::UnkReflObject*>(hit->field_0x0->field_0x10->field_0x10);
    if (tile && fn_710009d50c(tile->vf_0x0(), fn_7100ca0484())) {
        UnkReflObject* gun = static_cast<UnkReflObject*>(
            self->actor->field_0x2a8);
        if (gun && fn_710009d50c(gun->vf_0x0(), fn_7100857564()) && fn_710085aa5c(gun, tile, hit)) {
            flags |= 1;
            result.bits = flags;
        }
        u64 unused = 0;
        if (fn_71010d9638(self, hit, &unused) && !fn_7100df5874(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310))) {
            flags |= 1;
            result.bits = flags;
        }
        if (UnkQueryObj* object = fn_7100ca2048(tile, hit->field_0x14, 1)) {
            UnkHitFlags_b152 more = fn_7100301c0c(self->actor->field_0x2e0, object, hit);
            result.bits = more.bits | flags;
        }
    }
    return result;
}

void SetSamusLegOverrideKeyActive(UnkSamusMovement* movement, const u64* key, bool active, bool flag) {
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
