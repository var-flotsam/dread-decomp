// Unit 2702 (tools/tu_map.py, hint CAIComponent): small AI component helpers, run overnight batch b0264. Names and
// layouts are placeholders.
#include <new>

#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CAttackComponent.h"
#include "game/CCharacterMovement.h"
#include "game/CEnemyMovement.h"
#include "game/CFrozenComponent.h"
#include "game/CInputComponent.h"
#include "game/CMovementComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU341.h"
#include "game/UnkTU394.h"
#include "game/UnkTU53.h"
#include "game/UnkTU55.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// ---- e ----
struct UnkAIVtbl_e {
    u8 pad_0x0[0x430];
    void (*vf_0x430)(void* self);
};
struct UnkAICtx_e {
    u8 pad_0x0[0x48c];
    union {
        f32 f;
        u32 u;
    } field_0x48c;
    f32 field_0x490;
    u8 pad_0x494[8];
    bool field_0x49c;
};
struct UnkAIActor_e {
    u8 pad_0x0[0x218];
    void* field_0x218;
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0xa0];
    void* field_0x2c8;
};
struct UnkAIState_e {
    union {
        f32 f;
        u32 u;
    } timer;
    u8 pad_0x4[4];
    bool field_0x8;
    bool field_0x9;
    bool field_0xa;
    bool field_0xb;
};
struct UnkAIOwner_e {
    UnkAIVtbl_e* vtbl;
    u8 pad_0x8[0x28];
    UnkAIActor_e* actor;
    u8 pad_0x38[6];
    bool field_0x3e;
    u8 pad_0x3f[0x1610 - 0x3f];
    UnkAICtx_e* field_0x1610;
    u8 pad_0x1618[0x14674 - 0x1618 + 0x1466c - 0x1466c];
};

struct UnkFrostbiteComponent;

void fn_710110603c(UnkAIOwner_e* self) {
    UnkAIState_e* s = reinterpret_cast<UnkAIState_e*>(reinterpret_cast<u8*>(self) + 0x1466c);
    if (!self->field_0x3e) {
        if (!s->field_0x8) {
            void* comp = self->actor->field_0x2c8;
            if ((!comp || !fn_71007f9f20(comp) || fn_71007f9fd0(reinterpret_cast<UnkFrostbiteComponent*>(comp))) && fn_7100d7bea0(reinterpret_cast<UnkActor*>(self->actor), false)) {
                self->field_0x3e = true;
                self->vtbl->vf_0x430(self);
                UnkAICtx_e* ctx = self->field_0x1610;
                if (ctx->field_0x49c)
                    s->field_0x9 = true;
                s->timer.u = ctx->field_0x48c.u;
                void* c218 = self->actor->field_0x218;
                if (c218 && fn_71010cc3a4(c218) == 4) {
                    s->field_0xa = true;
                    if (UnkAIActor_e* a = self->actor) {
                        UnkAnimationComponent* c = a->field_0x220;
                        if (c && fn_710117b348(c, fn_7100bf4588()))
                            s->field_0xb = true;
                    }
                }
                return;
            }
        }
        if (!self->field_0x3e)
            return;
    }
    f32 t = s->timer.f;
    if (!(t > 0.0f))
        return;
    UnkAICtx_e* ctx = self->field_0x1610;
    f32 g = ctx->field_0x490;
    if (g > 0.0f) {
        if (t > g)
            s->timer.u = ctx->field_0x48c.u;
        else
            s->timer.f = g;
    } else {
        f32 h = ctx->field_0x48c.f;
        f32 h2 = h * 0.5f;
        if (!(t > h2)) {
            s->timer.f = h2;
            return;
        }
        // keeps the two stores as separate branches (no select)
        asm volatile("" ::: "memory");
        s->timer.f = h;
    }
}

// ---- g ----
struct UnkActor_7101105e44;
struct UnkVec3 {
    u32 a;
    u32 b;
    u32 c;
};

struct UnkAIVtbl_g {
    u8 pad_0x0[0x438];
    void (*vf_0x438)(void* self);
    u8 pad_0x440[0x18];
    void (*vf_0x458)(void* self);
};
struct UnkAICfg_g {
    u8 pad_0x0[0x22c];
    bool field_0x22c;
    u8 pad_0x22d[0x13c];
    bool field_0x369;
};
struct UnkAIGroupMember_g;
struct UnkAIGroupHolder_g {
    u8 pad_0x0[0x90];
    UnkAIGroupMember_g** items;  // 0x90
    u8 pad_0x98[0];
};
struct UnkAIGroup_g {
    u8 pad_0x0[0x90];
    void** items;  // 0x90
    u32 count;  // 0x98
};
struct UnkAIGroupSrc_g {
    u8 pad_0x0[0x10];
    void* field_0x10;
};
struct UnkAIActorB_g {
    u8 pad_0x0[0x398];
    UnkAIGroupSrc_g* field_0x398;
};
struct UnkAIMemberActor_g {
    u8 pad_0x0[0x30];
    struct {
        u8 pad_0x0[0x310];
        bool field_0x310;
    }* field_0x30;
    u8 pad_0x38[0x1114 - 0x38];
    u32 field_0x1114;
};
struct UnkAIMemberInner_g {
    u8 pad_0x0[0x240];
    UnkAIMemberActor_g* field_0x240;
};
struct UnkAIMemberHolder_g {
    u8 pad_0x0[0x10];
    UnkAIMemberInner_g* field_0x10;
};
struct UnkAIStruct_g {
    UnkAIVtbl_g* vtbl;
    u8 pad_0x8[0x28];
    UnkActor* actor;
    u8 pad_0x38[0x1610 - 0x38];
    UnkAICfg_g* field_0x1610;
};

UnkAIGroup_g* fn_71011277a4(void* src);

void fn_7101102da8(UnkAIStruct_g* self) {
    if (self->field_0x1610) {
        if (self->field_0x1610->field_0x369) {
            UnkAIActorB_g* a = reinterpret_cast<UnkAIActorB_g*>(self->actor);
            if (a->field_0x398 && a->field_0x398->field_0x10) {
                if (UnkAIGroup_g* g = fn_71011277a4(a->field_0x398->field_0x10)) {
                    if (g->count) {
                        for (void **it = g->items, **end = it + g->count; it != end; ++it) {
                            UnkAIMemberActor_g* m = static_cast<UnkAIMemberHolder_g*>(*it)->field_0x10->field_0x240;
                            if (!m || !m->field_0x30->field_0x310)
                                continue;
                            fn_7100d7add4(reinterpret_cast<UnkActor_7101105e44*>(m->field_0x30), false);
                            reinterpret_cast<u8*>(m)[0x11e0] = 0;
                            m->field_0x1114 = 0;
                        }
                    }
                }
            }
        }
    }
    if (self->field_0x1610 && self->field_0x1610->field_0x22c) {
        if (fn_7100d7c990(self->actor) && fn_7100d7c990(self->actor)->field_0x1f8 &&
            !fn_7100979ef0(fn_7100d7c990(self->actor)->field_0x1f8)) {
            u8* st = reinterpret_cast<u8*>(self) + 0x1220;
            if (st[0]) {
                UnkActor* a = fn_7100d7c990(self->actor);
                u32 x = *(u32*)&a->field_0x44;
                u32 y = *(u32*)&a->field_0x54;
                u32 z = *(u32*)&a->field_0x64;
                UnkVec3 v = {x, y, z};
                fn_710007b444(reinterpret_cast<u8*>(self) + 0x1604, &v);
                st[0x3e2] = 0xff;
                *(u16*)(reinterpret_cast<u8*>(self) + 0x1600) = 0xffff;
            }
        }
    }
    self->vtbl->vf_0x438(self);
    self->vtbl->vf_0x458(self);
}

// ---- h ----
using base::global::CStrId;

struct UnkTimelineId {
    u64 value;
};
struct UnkNameHolder;

UnkTimelineId fn_7101177be0(UnkAnimationComponent* self, u64 key);
u32 fn_710050816c(UnkNameHolder* self);

struct UnkSpeedNode_h {
    u32 key;
    f32 value;
    u8 pad_0x8[0x10];
    UnkSpeedNode_h* next;  // 0x18
};
struct UnkSpeedMap_h {
    UnkSpeedNode_h** buckets;
    u32 count;
    u32 bucket_count;
};
struct UnkSpeedOwner_h {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};
struct UnkSpeedSelf_h {
    u8 pad_0x0[0x30];
    UnkSpeedOwner_h* owner;
    u8 pad_0x38[0x144c8 - 0x38];
    UnkSpeedMap_h map;
};
struct UnkSpeedRecord_h {
    u8 pad_0x0[0x18];
    f32 field_0x18;
};

f32 fn_71010ff140(UnkSpeedSelf_h* self) {
    UnkAnimationComponent* anim = self->owner->field_0x220;
    if (!anim)
        return -1.0f;
    CStrId id(*fn_7101177ae8(anim, 0xf4061ff25a2163d6));
    u32 hash;
    {
        UnkTimelineId tl = fn_7101177be0(anim, 0xf4061ff25a2163d6);
        hash = fn_710050816c(reinterpret_cast<UnkNameHolder*>(&tl));
    }
    f32 speed = -1.0f;
    if (self->map.count) {
        u32 idx = hash % self->map.bucket_count;
        UnkSpeedNode_h** slot = &self->map.buckets[idx];
        if (slot && *slot) {
            UnkSpeedNode_h* head = *slot;
            bool found = false;
            for (UnkSpeedNode_h* p = head; p; p = p->next) {
                if (p->key == hash) {
                    found = true;
                    break;
                }
            }
            if (found) {
                UnkSpeedNode_h* n = head;
                while (n->key != hash)
                    n = n->next;
                speed = n->value;
                if (speed > 0.0f) {
                    UnkTrackQuery q = {fn_7100ceb8ec(), nullptr, -1.0f};
                    fn_710117b08c(anim, &q);
                    if (q.result)
                        speed *= reinterpret_cast<UnkSpeedRecord_h*>(q.result)->field_0x18;
                }
            }
        }
    }
    return speed;
}

// ---- i ----
struct UnkEntityState;
struct UnkActorTarget;
extern UnkDesc_SpitSend g_unk_710164b968;  // reached through the GOT

struct UnkStartOwner_i {
    void** vtable;
    u8 pad_0x8[0x2c];
    u8 pad_0x34[4];
    u8 pad_0x38[0xc];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
    u8 pad_0x68[0x1b8];
    void* field_0x220;
    void vf_0xd0(const UnkVec3f* v, bool flag) {
        reinterpret_cast<void (*)(UnkStartOwner_i*, const UnkVec3f*, bool)>(vtable[0xd0 / 8])(this, v, flag);
    }
};
struct UnkStartSelf_i {
    u8 pad_0x0[0x30];
    UnkStartOwner_i* owner;
    u8 pad_0x38[0x15d0 - 0x38];
    bool field_0x15d0;
};

void fn_710110e554(UnkStartSelf_i* self);

void fn_710110e3dc(UnkStartSelf_i* self) {
    UnkStartOwner_i* owner = self->owner;
    f32 x = owner->x;
    f32 y = owner->y;
    f32 z = owner->z;
    UnkVec3f pos(x, y, z);
    base::reflection::UnkReflTypeIdArg value;
    base::reflection::UnkReflTypeIdArg arg;
    owner->vf_0xd0(&pos, true);
    if (void* target = self->owner->field_0x220) {
        value.id = 2;
        arg.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
        fn_7101176690(target, 0x3951592af38cb532, 0xf4061ff25a2163d6, &value, &g_unk_710164b968,
                      &g_unk_7172769740, &arg);
        fn_7101176a88(reinterpret_cast<UnkActorTarget*>(target), 1);
    }
    fn_710110e554(self);
    fn_7100232864(reinterpret_cast<UnkEntityState*>(self->owner), false);
    self->field_0x15d0 = true;
}

// ---- l ----
using base::global::CStrId;

struct UnkEnergyWaveTargetComp_b49;
struct CAttack;
const u64* fn_7100360890(CAttack* attack);

struct UnkLinkOwner_l {
    u8 pad_0x0[0x1d8];
    void* field_0x1d8;
};
struct UnkLinkTarget_l {
    u8 pad_0x0[0x30];
    UnkLinkOwner_l* field_0x30;
};
struct UnkLinkActor_l {
    u8 pad_0x0[0x240];
    UnkLinkTarget_l* field_0x240;
};
struct UnkLinkSelf_l {
    u8 pad_0x0[0x30];
    UnkLinkActor_l* owner;
    u8 pad_0x38[0x1618 - 0x38];
    u32 count;  // 0x1618
    u8 entries[8];  // 0x161c
    u8 pad_0x1624[0x1780 - 0x1624];
    CStrId id;  // 0x1780
    u8 pad_0x1788[0x1798 - 0x1788];
    u8 field_0x17a1;
};

void fn_7101104f9c(u8* raw) {
    UnkLinkSelf_l* self = reinterpret_cast<UnkLinkSelf_l*>(raw);
    raw[0x17a1] = 1;
    u8* link = raw + 0x17a8;
    UnkLinkTarget_l* next = self->owner->field_0x240;
    UnkLinkTarget_l*& cur = *reinterpret_cast<UnkLinkTarget_l**>(raw + 0x17f0);
    if (cur) {
        if (UnkLinkOwner_l* o = cur->field_0x30) {
            if (o->field_0x1d8)
                fn_7100872908(o->field_0x1d8, link);
        }
        cur = nullptr;
    }
    if (next) {
        if (UnkLinkOwner_l* o = next->field_0x30) {
            if (o->field_0x1d8)
                fn_7100872860(reinterpret_cast<UnkEnergyWaveTargetComp_b49*>(o->field_0x1d8), link);
        }
        cur = next;
    }
    if (self->id.data == &g_unk_7101d47108->empty && self->count) {
        CStrId tmp(*reinterpret_cast<const CStrId*>(fn_7100360890(*reinterpret_cast<CAttack**>(self->entries))));
        self->id = tmp;
    }
}

// ---- n ----
f32 fn_71002b27d4(void* e, u32 a, u32 b, f32* value);
void fn_71002b2c80(void* e, u32 a, u32 b, f32* value);

struct UnkSlot_n {
    u8 pad_0x0[2];
    s16 id2;  // 0x2
    f32 field_0x4;
    f32 field_0x8;
    u8 pad_0xc[0x12];
    s16 id1e;  // 0x1e
};
struct UnkOther_n {
    u8 pad_0x0[0x10c];
    f32 x;  // 0x10c
    f32 y;  // 0x110
};
struct UnkOtherH_n {
    u8 pad_0x0[0x120];
    void* field_0x120;
};
struct UnkOwnerN_n {
    u8 pad_0x0[0x38];
    u8 xform[0x30];
    u8 pad_0x68[0x1b0];
    void* field_0x218;
};
struct UnkSelf_n {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkOwnerN_n* owner;
};

f32 fn_710110f4f4(UnkSelf_n* self, UnkOther_n* other, u32 a, u32 b, f32* pair, UnkSlot_n* slot, f32 value) {
    u8* raw = reinterpret_cast<u8*>(self);
    base::UnkVector<void*>* v1 = reinterpret_cast<base::UnkVector<void*>*>(raw + 0x14520);
    base::UnkVector<void*>* v2 = reinterpret_cast<base::UnkVector<void*>*>(raw + 0x14548);
    for (void **it = v1->data, **end = it + v1->count; it != end; ++it)
        fn_71002b27d4(*it, a, b, &value);
    for (void **it = v2->data, **end = it + v2->count; it != end; ++it)
        fn_71002b2c80(*it, a, b, &value);
    if (other && !reinterpret_cast<UnkOtherH_n*>(other)->field_0x120) {
        u32 px = reinterpret_cast<u32*>(pair)[0];
        u32 py = reinterpret_cast<u32*>(pair)[1];
        if (slot && slot->id2 != -1 && slot->id1e != -1) {
            px = reinterpret_cast<u32*>(&slot->field_0x4)[0];
            py = reinterpret_cast<u32*>(&slot->field_0x4)[1];
        }
        base::math::CVector2D fwd;
        UnkVec3 v;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->xform));
        u32 mx = reinterpret_cast<const u32*>(&m)[2];
        u32 my = reinterpret_cast<const u32*>(&m)[6];
        u32 mz = reinterpret_cast<const u32*>(&m)[10];
        v.a = mx;
        v.b = my;
        v.c = mz;
        fn_710007b430(&fwd, reinterpret_cast<const base::math::CVector3D*>(&v));
        if (self->owner->field_0x218 && fn_71010cab0c(self->owner->field_0x218)) {
            f32 nx = -*reinterpret_cast<f32*>(raw + 0x14520 + 0x108);
            f32 ny = -*reinterpret_cast<f32*>(raw + 0x14520 + 0x10c);
            f32 nz = -*reinterpret_cast<f32*>(raw + 0x14520 + 0x110);
            reinterpret_cast<f32&>(v.a) = nx;
            reinterpret_cast<f32&>(v.b) = ny;
            reinterpret_cast<f32&>(v.c) = nz;
            fn_710007b444(&fwd, &v);
        }
        f32 dx = reinterpret_cast<f32&>(px) - other->x;
        f32 dy = reinterpret_cast<f32&>(py) - other->y;
        f32 len = __builtin_sqrtf(dx * dx + dy * dy);
        if (len != 0.0f) {
            dx /= len;
            dy /= len;
        }
        f32 dot = dx * fwd.x + dy * fwd.y;
        if (dot < 0.0f)
            value += reinterpret_cast<f32 (*)(UnkSelf_n*)>(self->vtable[0x6f0 / 8])(self);
    }
    return value;
}

// ---- w ----
struct UnkAttack_A2_b68;
struct UnkCancelAttack_ovb0128;
void* fn_7100362324(UnkCancelAttack_ovb0128* self, u32 value);
struct UnkLocalNode_w {
    void* first;
    UnkLocalNode_w* next;
};
extern char g_unk_7101a21540[];
struct UnkLocal_w {
    UnkRefObject* handle;  // 0x0
    f32 x;  // 0x8
    f32 y;  // 0xc
    u8 pad_0x10[8];
    bool flag;  // 0x18
    u8 pad_0x19[0xf];
    void** vtable;  // 0x28
    u32 count;  // 0x30
    u8 pad_0x34[4];
    UnkLocalNode_w* head;  // 0x38
    UnkLocalNode_w* tail;  // 0x40
    base::UnkAllocator* allocator;  // 0x48
    u64 pad_0x50;
    UnkLocal_w(void* actor, void* arg);  // fn_71002c6f80
    __attribute__((always_inline)) ~UnkLocal_w() {
        vtable = reinterpret_cast<void**>(g_unk_7101a21540 + 0x10);
        for (UnkLocalNode_w* n = head; n;) {
            UnkLocalNode_w* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
        if (UnkRefObject* h = handle) {
            if (--h->ref_count == 0)
                h->vf_0x10();
        }
    }
};
struct UnkAttackQuery_ovb0115;
inline UnkLocal_w::UnkLocal_w(void* actor, void* arg) {
    fn_71002c6f80(reinterpret_cast<UnkAttackQuery_ovb0115*>(this), static_cast<UnkActor*>(actor), arg);
}
struct UnkTargetSelf_w;
void fn_71010ffeb8(UnkTargetSelf_w* self, void* first, bool a, bool b);

struct UnkPresetObj_w {
    void** vtable;
    u8 pad_0x8[0x20];
    f32 field_0x28;
};
struct UnkActorW_w {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0x230];
    void* field_0x278;
};
struct UnkTargetVtbl_w {
    u8 pad_0x0[0x468];
    bool (*vf_0x468)(UnkTargetSelf_w* self, void* local, u32 id);
    void (*vf_0x470)(UnkTargetSelf_w* self, void* local, u32 id, bool flag);
};
struct UnkTargetSelf_w {
    UnkTargetVtbl_w* vtable;
    u8 pad_0x8[0x28];
    UnkActorW_w* actor;
    u8 pad_0x38[0x1140 - 0x38];
    u32 field_0x1140;
    u8 pad_0x1144[4];
    UnkAttack_A2_b68* field_0x1148;
    bool field_0x1150;
    u8 pad_0x1151[3];
    f32 field_0x1154;
    bool field_0x1158;
    u8 pad_0x1159[0xf];
    u8 field_0x1168[0x4a8];
    void* field_0x1610;
};

void fn_71010ffc38(UnkTargetSelf_w* self, f32 dt) {
    if (!self->field_0x1148) {
        self->field_0x1154 = 0.0f;
        return;
    }
    self->field_0x1154 = self->field_0x1154 + dt;
    void* busy = self->actor->field_0x278;
    if (busy && fn_7101136308(busy))
        self->field_0x1158 = true;
    if (!self->field_0x1610 || !self->field_0x1148)
        return;
    if (!*reinterpret_cast<void**>(reinterpret_cast<u8*>(self->field_0x1148) + 0x20) || self->field_0x1150)
        return;
    busy = self->actor->field_0x278;
    if (!busy || fn_7101136308(busy))
        return;
    if (!self->field_0x1148)
        return;
    UnkPresetObj_w* preset = static_cast<UnkPresetObj_w*>(fn_7100362324(reinterpret_cast<UnkCancelAttack_ovb0128*>(self->field_0x1148), self->field_0x1140));
    if (!preset)
        return;
    if (!fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(reinterpret_cast<void* (*)(void*)>(preset->vtable[0])(preset)), fn_710036593c()))
        return;
    f32 limit = preset->field_0x28;
    if (!(limit >= 0.0f) || !(self->field_0x1154 >= limit))
        return;
    UnkActor* other = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->actor));
    f32 dist = other ? __builtin_fabsf(other->field_0x44 - self->actor->x) : 0.0f;
    UnkLocal_w local(self->actor, self->field_0x1168);
    local.x = dist;
    local.y = dist;
    local.flag = true;
    void* first = nullptr;
    UnkTargetVtbl_w* vt = self->vtable;
    u32 id = self->field_0x1140;
    if (vt->vf_0x468(self, &local, id)) {
        vt = self->vtable;
        id = self->field_0x1140;
        vt->vf_0x470(self, &local, id, true);
        if (local.head)
            first = local.head->first;
    }
    fn_71010ffeb8(self, first, true, true);
}

// ---- fn_710110030c ----
struct UnkMovement_71010d08d4;
struct UnkMovementVec_71010d08d4;
extern f32 g_unk_7101cb1db8;

struct UnkRamp_j {
    f32 scale_x;  // 0x0
    f32 scale_y;  // 0x4
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 timer;  // 0x1c
};
struct UnkRampOwner_j {
    u8 pad_0x0[0x218];
    UnkMovement_71010d08d4* field_0x218;
};
struct UnkRampSelf_j {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkRampOwner_j* owner;
    u8 pad_0x38[0x15d0 - 0x38];
    bool field_0x15d0;
    u8 pad_0x15d1[0x1464c - 0x15d1];
    UnkRamp_j ramp;
    bool vf_0x420() { return reinterpret_cast<bool (*)(UnkRampSelf_j*)>(vtable[0x420 / 8])(this); }
    void vf_0x400() { reinterpret_cast<void (*)(UnkRampSelf_j*)>(vtable[0x400 / 8])(this); }
};

static inline f32 Clamp01_j(f32 v) {
    f32 m = !(v >= 1.0f) ? v : 1.0f;
    if (v <= 0.0f)
        m = 0.0f;
    return m;
}

static inline f32 Mix_j(f32 a, f32 b, f32 t) {
    if (t >= 1.0f)
        return b;
    if (t < 0.0f)
        return a;
    f32 p = a * (1.0f - t);
    f32 q = b * t;
    return q + p;
}

void fn_710110030c(UnkRampSelf_j* self, f32 dt) {
    UnkRamp_j* r = &self->ramp;
    if (!self->vf_0x420() && (!self->field_0x15d0 || r->timer > 0.0f)) {
        if (UnkMovement_71010d08d4* move = self->owner->field_0x218) {
            f32 t = r->timer;
            f32 k;
            if (!(t <= r->field_0x14)) {
                f32 u = Clamp01_j((t - r->field_0x14) / (g_unk_7101cb1db8 - r->field_0x14));
                k = Mix_j(r->field_0x10, r->field_0x18, u);
            } else {
                f32 u = Clamp01_j(t / r->field_0x14);
                f32 e = 1.0f - fn_7100076020(u, "iI");
                k = Mix_j(r->field_0xc, r->field_0x10, e);
            }
            f32 sx = r->scale_x;
            f32 sy = r->scale_y;
            struct {
                f32 x, y, z;
            } v = {k * sx, k * sy, 0.0f};
            fn_71010d08d4(move, reinterpret_cast<const UnkMovementVec_71010d08d4*>(&v), true);
        }
    }
    if (!self->field_0x15d0)
        self->vf_0x400();
    r->timer = r->timer + dt;
}

// ---- fn_7101107e94 ----
struct UnkFlyAI_z;
struct UnkAIBB_b0144;

struct UnkFlyMoveVtbl_z {
    u8 pad_0x0[0x358];
    void (*vf_0x358)(void* self, void* arg, u8 a, u32 b, u32 c);
};
struct UnkFlyMove_z {
    UnkFlyMoveVtbl_z* vtable;
};
struct UnkFlyCfg_z {
    u8 pad_0x0[0x141];
    bool field_0x141;
};
struct UnkFlyAiObj_z {
    void** vtable;
    u8 pad_0x8[0x190];
    u8 field_0x198;
    const base::reflection::CClass* vf_0x0() { return reinterpret_cast<const base::reflection::CClass* (*)(UnkFlyAiObj_z*)>(vtable[0])(this); }
};
struct UnkFlyActor_z {
    u8 pad_0x0[0x38];
    u8 xform[0x30];
    u8 pad_0x68[0x188];
    void* field_0x1f0;  // 0x1f0 (placeholder position)
};
struct UnkFlyAI_z {
    void** vtable;
    u8 pad_0x8[8];
    UnkFlyAiObj_z* field_0x10;
    u8 pad_0x18[0x18];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x13d4 - 0x38];
    u32 field_0x13d4;
    u8 pad_0x13d8[0x1610 - 0x13d8];
    UnkFlyCfg_z* field_0x1610;
};

void fn_7101107e94(UnkFlyAI_z* self, void* arg) {
    UnkFlyCfg_z* cfg = self->field_0x1610;
    self->field_0x13d4 = 0;
    if (cfg->field_0x141) {
        UnkAnimationComponent* comp = static_cast<UnkAnimationComponent*>(self->actor->field_0x220);
        if (comp && fn_710117b348(comp, fn_7100cf96a8())) {
            if (reinterpret_cast<UnkFlyActor_z*>(self->actor)->field_0x1f0)
                fn_7101108004(reinterpret_cast<UnkAIBB_b0144*>(self), arg);
        }
    }
    UnkFlyMove_z* move = static_cast<UnkFlyMove_z*>(self->actor->field_0x218);
    if (move) {
        UnkFlyAiObj_z* ai = self->field_0x10;
        const base::reflection::CClass* cls = ai->vf_0x0();
        bool is_ai = fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(cls),
                                   reinterpret_cast<const base::reflection::CType*>(fn_7100408b2c()));
        UnkFlyAiObj_z* typed = is_ai ? ai : nullptr;
        u8 flag = typed->field_0x198;
        // keeps the vtable load after the flag load
        asm volatile("" ::: "memory");
        move->vtable->vf_0x358(move, arg, flag, 1, 0);
    }
    if (self->actor->field_0x298) {
        UnkParams params;
        __builtin_memset(params.field_0x8, 0, 13);
        params.field_0x30 = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x34 = 5;
        params.field_0x18 = 0;
        params.field_0x1c = 1;
        const u32* m = reinterpret_cast<const u32*>(&fn_710002614c(reinterpret_cast<const UnkMatrix44*>(reinterpret_cast<u8*>(self->actor) + 0x38)));
        u32 a = m[2];
        u32 b = m[6];
        u32 c = m[10];
        __builtin_memcpy(params.field_0x8, &a, 4);
        __builtin_memcpy(params.field_0x8 + 4, &b, 4);
        __builtin_memcpy(params.field_0x8 + 8, &c, 4);
        params.field_0x8[12] = 1;
        fn_71006f4b0c(self->actor->field_0x298, &params, 1);
    }
    reinterpret_cast<void (*)(UnkFlyAI_z*, u32)>(self->vtable[0x778 / 8])(self, 1);
}
