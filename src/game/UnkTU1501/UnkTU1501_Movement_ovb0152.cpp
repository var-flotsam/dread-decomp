// Unit 1501 (CSamusMovement, tools/tu_map.py: 0x71009dc3bc-0x71009f16a8), a second batch of its functions
// (the first is src/game/CSamusMovement.cpp): the track-driven air friction (fn_71009e73cc, with the run
// speed fn_71009e754c it scales), track and state checks (fn_71009e5114, fn_71009e69c4, fn_71009e7f20),
// the animation requests (fn_71009e7a5c, fn_71009e7b78, fn_71009e7d9c, fn_71009e972c, fn_71009ec94c), the
// hit flags (fn_71009ecd98) and the leg and input updates. Names and layouts are placeholders.
#include <algorithm>

#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkItemProperties.h"
#include "game/UnkXformNode.h"
#include "types.h"

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

// A mode of the movement (the pointers at +0x2d1c): its kind and, for kind 6, a position at +0x19c.
struct UnkMode_b152 {
    void** vtable;
    u8 pad_0x8[0x8];
    s32 kind;  // 0x10
    u8 pad_0x14[0x188];
    UnkVec3f field_0x19c;
};

// What a CCharClass*Track record holds past its header: two floats.
struct UnkTrackRec_b152 {
    u8 pad_0x0[0x18];
    f32 field_0x18;
    f32 field_0x1c;
};

// A link into the weak handle list of a track: the context and callback it is given, and the handle it holds.
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

struct UnkProbeResult {
    u64 field_0x0;
    u32 field_0x8;
};

struct UnkSamusSettings {
    u8 pad_0x0[0x70];
    f32 field_0x70;
    f32 field_0x74;
    bool field_0x78;
};
extern UnkSamusSettings g_unk_7172740de0;
extern base::reflection::UnkTunable g_unk_71726b0fc0;
extern base::reflection::UnkTunable g_unk_717273ba88;

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

struct UnkIdHolder_b152 {
    base::global::UnkStringData* data;
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

struct UnkAbility {
    virtual const base::reflection::CClass* vf_0x0();
};

struct UnkVec2_b152 {
    f32 x, y;
};

struct UnkHitFlags_b152 {
    u32 bits;
    UnkHitFlags_b152(u32 b) : bits(b) {}
    UnkHitFlags_b152(const UnkHitFlags_b152& o) : bits(o.bits) {}
};
struct UnkHitNode_b152 {
    u8 pad_0x0[0x10];
    UnkHitNode_b152* field_0x10;
};
struct UnkHitInfo_b152 {
    UnkHitNode_b152* field_0x0;
};

// The id table (fn_7100031894 builds it; as src/game/CSamusMagnetGloveMovementState/CParams/CParams_Dir_ovb0082.cpp).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x13f8];
    CStrId field_0x13f8;
    u8 pad_0x1400[0x1848 - 0x1400];
    CStrId field_0x1848;
    u8 pad_0x1850[0x2578 - 0x1850];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

// The request that fn_710098ae04 takes (as src/game/CSamusGroundMovementState/CParams/CParams_Default_ovb0138.cpp):
// the reflected params base holding an unset track request, then a derived part.
struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b958;
extern UnkDesc_SpitSend g_unk_7172769740;
extern void* g_unk_7101a22cb8[];
extern void* g_unk_7101a22d18[];
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
UnkAbility* fn_7100df5928(UnkPlayer* self);
bool fn_710033cf20(UnkAbility* ability);
f32 fn_71007965a8(UnkRunInfo_b152* info);
f32 fn_71007965c0(UnkRunInfo_b152* info);
void fn_7100978f58(char* self, const void* arg);  // reads three words at arg
bool fn_71010d15d0(char* self, const CVector3D* pos, UnkProbeResult* result, f32 radius);
void fn_71002878dc(UnkMsg_b152* msg);
void fn_7100287954(UnkMsg_b152* msg);
void fn_7100976940(UnkMove_b152* self);
void fn_71009767f0(UnkMove_b152* self);
UnkIdHolder_b152* fn_71009e7870(UnkMove_b152* self);
bool fn_71011428bc(UnkInventory* inventory, const UnkQualifiedId& item);
bool fn_7100df45ec(UnkGameManager* self);
void fn_71002d6074(UnkLeg_b152* leg, bool flag, f32 weight);
bool fn_710002fb70(void* timer);
void fn_71009796dc(UnkMove_b152* self);
void fn_71009f115c();
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
bool fn_710117af10(UnkStruct_710117af10* self, UnkCls_710117af10** name, bool skipBusy);
UnkAbility* fn_71003013ec(UnkAbilitySet* set, u64 id);
u64 fn_7100308c74();
u32 fn_7100309568(UnkAbility* ability);
bool fn_7100f0ca8c(void* member);
bool fn_7100df5a4c(UnkGameManager* self);
void fn_71010dc54c(UnkMove_b152* self, UnkSpanArg_b152* span);
void fn_71002fdf5c(void* obj, void* arg);
void fn_710098ae04(UnkAirTarget_B71a6* target, UnkAirRequest_B71a6* req);
bool fn_7101176ea8(UnkStruct_7101176ea8* self, const UnkRequest_7101176ea8* req, u64 key);
void fn_71010d956c(UnkMove_b152* self);
bool fn_710085aa5c(void* gun, void* tile, UnkHitInfo_b152* hit);
bool fn_71010d9638(UnkMove_b152* self, UnkHitInfo_b152* hit, u64* out);
bool fn_7100df5874(UnkPlayer* self);
struct UnkQueryObj;
UnkQueryObj* fn_7100ca2048(void*, void*, u32);
UnkHitFlags_b152 fn_7100301c0c(void* set, void* object, void* hit);
void fn_710085b76c(UnkReflObject* gun, bool value);
// Out of line: fn_71009e73cc calls it.
__attribute__((noinline)) f32 fn_71009e754c(UnkMove_b152* self);

struct UnkComponent;
struct UnkGunComponent;
struct UnkGun_ovb0091;
const CStrId* fn_71008407fc(UnkGunComponent* self);
void* fn_710083cd30(UnkComponent* component);
bool fn_7100845fac(void* beam);
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
struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

UnkMatrix44_7100078c9c fn_7100078cd8(const UnkVec3f& t);
bool fn_7100d5babc(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);
void fn_7100d6006c(void* self);
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
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

static inline void ReleaseRef_b152(UnkRefObject* p) {
    if (p) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
}

struct UnkStaticZero_b152 {
    u32 value;
    UnkStaticZero_b152() : value(0) {}
};
inline const UnkStaticZero_b152& StaticZero_b152() {
    static UnkStaticZero_b152 s_value asm("g_unk_71726aec10");
    return s_value;
}

// `arg` (x1) reaches fn_7100978f58 untouched in the original.
void fn_71009e11c0(UnkMove_b152* self, const void* arg) {
    fn_7100978f58(reinterpret_cast<char*>(self), arg);
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

void fn_71009e3188(UnkMove_b152* self, f32* values) {
    if (self->field_0x5258 != 1.0f) {
        static CStrId s_forward("forward", true);
        static CStrId s_backward("backward", true);
        UnkStruct_710117af10* anim = static_cast<UnkStruct_710117af10*>(self->actor->field_0x220);
        if (anim && (fn_710117af10(anim, reinterpret_cast<UnkCls_710117af10**>(&s_forward), false) ||
                     fn_710117af10(static_cast<UnkStruct_710117af10*>(self->actor->field_0x220),
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
    if (void* obj = *reinterpret_cast<void**>(reinterpret_cast<u8*>(self->actor) + 0x2e0))
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
    if (fn_7100f0ca8c(reinterpret_cast<char*>(*manager) + 0x38))
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

bool fn_71009e73cc(UnkMove_b152* self, UnkVec2_b152* out) {
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
    if (fn_710117af10(static_cast<UnkStruct_710117af10*>(cls),
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
        UnkIdHolder_b152* id = fn_71009e7870(self);
        if (id->data != &g_unk_7101d47108->empty) {
            req->key = *reinterpret_cast<u64*>(reinterpret_cast<u8*>(id->data) + 0x30);
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
    fn_7101176ea8(reinterpret_cast<UnkStruct_7101176ea8*>(anim), reinterpret_cast<const UnkRequest_7101176ea8*>(&params),
                  0xf4061ff25a2163d6ull);
}

bool fn_71009e7f20(UnkMove_b152* self) {
    UnkInventory* inventory = *reinterpret_cast<UnkInventory**>(reinterpret_cast<u8*>(self->actor) + 0x270);
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
        if (void* gun = *reinterpret_cast<void**>(reinterpret_cast<u8*>(self->actor) + 0x2a8))
            fn_710083ed38(static_cast<UnkGun_ovb0091*>(gun));
    }
    if (void* gun = *reinterpret_cast<void**>(reinterpret_cast<u8*>(self->actor) + 0x2a8)) {
        const base::global::CStrId* current = fn_71008407fc(static_cast<UnkGunComponent*>(gun));
        if (SameId_b152(*current, GameManagerIdTable().field_0x1848)) {
            UnkReflObject* beam =
                static_cast<UnkReflObject*>(fn_710083cd30(static_cast<UnkComponent*>(gun)));
            if (beam && !fn_710009d50c(beam->vf_0x0(), fn_71008424e8()))
                beam = nullptr;
            if (fn_7100845fac(beam))
                fn_7100846e64(beam, 0x10);
        }
    }
    void* object = *reinterpret_cast<void**>(reinterpret_cast<u8*>(self->actor) + 0x2b0);
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
    if (!fn_7101176ea8(static_cast<UnkStruct_7101176ea8*>(cls), request, 0xf4061ff25a2163d6ull))
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

bool fn_71009ea664(UnkMove_b152* self) {
    if (!self->field_0x3a8)
        return false;
    UnkCollisionContainer* container = *reinterpret_cast<UnkCollisionContainer**>(reinterpret_cast<u8*>(self->actor) + 0x2a0);
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
    UnkMatrix44_7100078c9c mtx = fn_7100078cd8(pos);
    bool hit = fn_7100d5babc(static_cast<UnkTree*>(tracker), mask, reinterpret_cast<UnkShape*>(&shape),
                             reinterpret_cast<const UnkMtx44*>(&mtx), reinterpret_cast<const Vec2*>(delta), nullptr, false);
    return !hit;
}

void fn_71009ec7b4(UnkMove_b152* self) {
    UnkReflObject* gun = static_cast<UnkReflObject*>(
        *reinterpret_cast<void**>(reinterpret_cast<u8*>(self->actor) + 0x2a8));
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
            *reinterpret_cast<void**>(reinterpret_cast<u8*>(self->actor) + 0x2a8));
        if (gun && fn_710009d50c(gun->vf_0x0(), fn_7100857564()) && fn_710085aa5c(gun, tile, hit)) {
            flags |= 1;
            result.bits = flags;
        }
        u64 unused = 0;
        if (fn_71010d9638(self, hit, &unused) && !fn_7100df5874(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310))) {
            flags |= 1;
            result.bits = flags;
        }
        if (UnkQueryObj* object = fn_7100ca2048(tile, reinterpret_cast<u8*>(hit) + 0x14, 1)) {
            UnkHitFlags_b152 more = fn_7100301c0c(*reinterpret_cast<void**>(reinterpret_cast<u8*>(self->actor) + 0x2e0), object, hit);
            result.bits = more.bits | flags;
        }
    }
    return result;
}

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
