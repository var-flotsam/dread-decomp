// Unit 2689 (hint CPlayerMovement; tools/tu_map.py): helpers of the player movement. A window test of two
// extents against a third (fn_71010d87a0), the speed ramp step (fn_71010d956c), the tile hit test
// (fn_71010d9638), the weak handle at +0x7e8 (fn_71010d98bc), the forward to the boss component
// (fn_71010db084), the released check (fn_71010dbcc0), the request dispatch (fn_71010dc968), the timed hold
// (fn_71010da7e8), the copy into the other movement (fn_71010da468) and the script method that hands a
// string id to the movement (fn_71010dd0d4). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CBreakableTileGroup.h"
#include "game/CGrabComponent.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2689.h"
#include "game/UnkTU365.h"
#include "game/UnkTU56.h"
#include "game/UnkTU785.h"
#include "game/UnkTU794.h"
#include "game/UnkTimelineCalls.h"
#include "game_s2/UnkChargeComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

struct UnkSizes_b0204 {
    u8 pad_0x0[0x14];
    f32 f14;
    f32 f18;
    u8 pad_0x1c[0x10];
    f32 f2c;
    f32 f30;
};

static inline bool Inside(f32 v) {
    return !(v <= -0.99999988f) && !(v >= 0.99999988f);
}

bool fn_71010d87a0(const f32* a, const f32* b, const UnkSizes_b0204* c, f32 t) {
    if (!Inside(c->f30)) {
        if (a[0] + b[0] - c->f14 < t)
            return true;
        return c->f14 - (a[0] - b[0]) < t;
    }
    if (!Inside(c->f2c)) {
        if (a[1] + b[1] - c->f18 < t)
            return true;
        return c->f18 - (a[1] - b[1]) < t;
    }
    return false;
}

struct UnkPlayerMovement_b0204 {
    void** vtable;
    u8 pad_0x8[0xf1];
    u8 field_0xf9;
    u8 pad_0xfa[0x12];
    f32 field_0x10c;
    u8 pad_0x110[0xc8];
    f32 field_0x1d8;
    f32 field_0x1dc;
    u8 pad_0x1e0[0x1c8];
    u8 field_0x3a8;
    u8 pad_0x3a9[3];
    f32 field_0x3ac;
    u8 pad_0x3b0[0x418];
    f32 field_0x7c8;
};

void fn_71010d956c(UnkPlayerMovement_b0204* self) {
    f32 speed = self->field_0x10c;
    self->field_0x7c8 = 0.6f;
    f32 sign = (f32)((speed > 0.0f) - (speed < 0.0f));
    self->field_0x3ac = 0.6f;
    f32 v = __builtin_fabsf(speed) * 0.6f;
    f32 w = (self->field_0x1dc + -500.0f) * 0.5f;
    v = !(v < 70.0f) ? v : 70.0f;
    self->field_0x1d8 = v * sign;
    self->field_0x1dc = !(w >= -500.0f) ? w : -500.0f;
    if (self->field_0x3a8) {
        self->field_0x3a8 = 0;
        reinterpret_cast<void (*)(UnkPlayerMovement_b0204*)>(self->vtable[0x2b8 / 8])(self);
    }
    if (self->field_0xf9 == 1)
        return;
    self->field_0xf9 = 1;
    reinterpret_cast<void (*)(UnkPlayerMovement_b0204*)>(self->vtable[0x260 / 8])(self);
}

struct UnkQueryObj;
struct UnkPlayer;

struct UnkTileQuery_A3_b64 {
    u8 pad_0x0[0x38];
    u32 kind;
};

struct UnkHitLink1_b0204 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* object;
};
struct UnkHitLink0_b0204 {
    u8 pad_0x0[0x10];
    UnkHitLink1_b0204* link;
};

struct UnkHit_b0204 {
    UnkHitLink0_b0204* link;
    u8 pad_0x8[0xc];
    f32 pos[3];  // 0x14
    union {
        u32 u[3];
        f32 f[3];
    } alt;  // 0x20
    f32 field_0x2c;
    f32 field_0x30;
    u8 pad_0x34[8];
    u8 flags;  // 0x3c
};

struct UnkOwnerXform_b0204 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
};

struct UnkHitOwner_b0204 {
    u8 pad_0x0[0x30];
    UnkOwnerXform_b0204* field_0x30;
};

bool fn_71010d9638(UnkHitOwner_b0204* self, UnkHit_b0204* hit, UnkTileQuery_A3_b64** out) {
    if (!hit->link)
        return false;
    UnkHitLink0_b0204* l0 = hit->link;
    if (!l0->link)
        return false;
    base::reflection::UnkReflObject* group = l0->link->object;
    if (!group)
        return false;
    if (!fn_710009d50c(group->vf_0x0(), fn_7100ca0484()))
        return false;
    union {
        f32 f[3];
        u32 u[3];
    } pos;
    if (!(hit->flags & 0x20)) {
        UnkOwnerXform_b0204* x = self->field_0x30;
        f32 px = hit->pos[0], py = hit->pos[1], pz = hit->pos[2];
        f32 tx = x->x, ty = x->y, tz = x->z;
        pos.f[0] = px - tx;
        pos.f[1] = py - ty;
        pos.f[2] = pz - tz;
    } else {
        pos.u[0] = hit->alt.u[0];
        pos.u[1] = hit->alt.u[1];
        pos.u[2] = hit->alt.u[2];
    }
    UnkTileQuery_A3_b64* tile = fn_7100ca1ecc(group, hit->pos, &pos, true);
    if (!tile)
        return false;
    *out = tile;
    if (fn_7100c9e960(reinterpret_cast<UnkQueryObj*>(tile)) != 7)
        return false;
    if (!(hit->field_0x30 >= -1.1920929e-7f))
        return false;
    if (!(hit->alt.f[1] <= 1.1920929e-7f))
        return false;
    return !fn_7100df5874(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310));
}

struct UnkPlayerMovementC_b0204 {
    u8 pad_0x0[0x7e8];
    UnkWeakHandle* handle;
};

void fn_71010d98bc(UnkPlayerMovementC_b0204* self, UnkWeakTarget* target) {
    if (target)
        AssignRef(&self->handle, fn_7100d78378(target));
    else
        AssignRef(&self->handle, static_cast<UnkWeakHandle*>(nullptr));
}

extern char* g_unk_71726b2398;       // the instance, reached through the GOT

struct UnkSceneLink_b0204 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkBossActor_b0204 {
    u8 pad_0x0[0x240];
    base::reflection::UnkReflObject* field_0x240;
};

struct UnkBossComponent_b0204 {
    void** vtable;
};

void fn_71010db084(void* unused, void* arg) {
    char* instance = g_unk_71726b2398;
    if (!instance) {
        instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(instance);
        g_unk_71726b2398 = instance;
    }
    auto* link = *reinterpret_cast<UnkSceneLink_b0204**>(instance + 0x100);
    if (!link)
        return;
    auto* actor = static_cast<UnkBossActor_b0204*>(link->field_0x10);
    if (!actor)
        return;
    base::reflection::UnkReflObject* c = actor->field_0x240;
    if (!c)
        return;
    if (fn_710009d50c(c->vf_0x0(), fn_71003ab524())) {
        auto* b = reinterpret_cast<UnkBossComponent_b0204*>(c);
        reinterpret_cast<void (*)(UnkBossComponent_b0204*, void*)>(b->vtable[0x7b8 / 8])(b, arg);
    }
}

struct UnkPlayerMovementB_b0204 {
    u8 pad_0x0[0x30];
    void* field_0x30;
    u8 pad_0x38[0x44];
    f32 field_0x7c;
    u8 pad_0x80[0x98];
    f32 field_0x118;
};

struct UnkTunable58_b0204 {
    u8 pad_0x0[0x58];
    f32 field_0x58;
};

bool fn_71010dbcc0(UnkPlayerMovementB_b0204* self) {
    if (!(self->field_0x7c <= 0.0f))
        return false;
    if (!(self->field_0x118 <= 1.0f))
        return false;
    if (!fn_7100302014(self->field_0x30))
        return true;
    return reinterpret_cast<UnkTunable58_b0204*>(fn_7100302014(self->field_0x30))->field_0x58 >= 0.1f;
}

struct UnkPlayerMovementD_b0204;
void fn_71010d80e8(UnkPlayerMovementD_b0204* self, u32 kind);
void fn_71010d9f88(UnkPlayerMovementD_b0204* self);
void fn_71010d7d0c(UnkPlayerMovementD_b0204* self, u32 kind);
void fn_71010da220(UnkPlayerMovementD_b0204* self, f32 a, f32 b, f32 c);
extern base::reflection::UnkTunable g_unk_7172a03d80;

struct UnkRequest_b0204 {
    u8 pad_0x0[0xb8];
    const void* field_0xb8;
};

bool fn_71010dc968(UnkPlayerMovementD_b0204* self, UnkRequest_b0204* req) {
    if (req->field_0xb8 != g_unk_71726dda50.data)
        fn_71010d80e8(self, 2);
    else
        fn_71010d9f88(self);
    if (req->field_0xb8 != g_unk_71726dbf68) {
        fn_71010d7d0c(self, 3);
    } else {
        const f32* charge = reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_71726e0aa8) + 0xd8);
        fn_71010da220(self, charge[0], charge[1],
                      *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172a03d80) + 0x60));
    }
    return false;
}

struct UnkMoveStaticZero_b0204 {
    u32 value;
    UnkMoveStaticZero_b0204() : value(0) {}
};

inline const UnkMoveStaticZero_b0204& UnkMoveStaticZero() {
    static UnkMoveStaticZero_b0204 s_value asm("g_unk_7101d57cc8");
    return s_value;
}

inline u32 UnkMoveDefaultMask() {
    static u32 s_mask asm("g_unk_7101d57cb8") = ~UnkMoveStaticZero().value & 0xff;
    return s_mask;
}

extern u64 g_unk_710164cf08;

struct UnkMoveOwner_b0204 {
    u8 pad_0x0[0x260];
    void* field_0x260;
    u8 pad_0x268[0x80];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkMoveHold_b0204 {
    u8 pad_0x0[0x30];
    UnkMoveOwner_b0204* owner;
    u8 pad_0x38[0x370];
    u8 field_0x3a8;
    u8 pad_0x3a9[0x14fb];
    u8 field_0x18a4;
    u8 pad_0x18a5[3];
    f32 field_0x18a8;
    f32 field_0x18ac;
};

void fn_71010da7e8(UnkMoveHold_b0204* self, f32 dt) {
    if (self->field_0x18a4) {
        if (self->owner->field_0x260 && fn_710113d83c(self->owner->field_0x260))
            fn_71010d80e8(reinterpret_cast<UnkPlayerMovementD_b0204*>(self), 3);
    }
    if (self->field_0x18a4) {
        if (self->field_0x3a8) {
            if (self->field_0x18a8 == 0.0f) {
                if (UnkTimelineComponent* timeline = self->owner->timeline) {
                    alignas(8) u32 value = UnkMoveDefaultMask();
                    fn_7100a2cb7c(timeline, &g_unk_710164cf08, &value, false, -1.0f);
                }
            }
            self->field_0x18a8 += dt;
            if (self->field_0x18a8 >=
                *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_71726e0aa8) + 0xa4))
                fn_71010d80e8(reinterpret_cast<UnkPlayerMovementD_b0204*>(self), 0);
        }
    } else {
        self->field_0x18ac += dt;
    }
}

extern const base::reflection::CClass g_unk_7172a03fd0 __attribute__((visibility("hidden")));
struct UnkAssignedSlot;

struct UnkMoveCopy_b0204 {
    void** vtable;
    u8 pad_0x8[0x74];
    u32 field_0x7c;
    u8 pad_0x80[0xf8];
    u8 slot_0x178[8];
    u8 slot_0x180[8];
    u32 field_0x188[13];
    u8 pad_0x1bc[0x44];
    u32 field_0x200;
    u32 field_0x204;
    u8 pad_0x208[0x5b0];
    u8 field_0x7b8;
    u8 field_0x7b9;
    u8 field_0x7ba;
    u8 pad_0x7bb[0x29];
    u8 field_0x7e4;
    u8 pad_0x7e5[3];
    UnkWeakHandle* field_0x7e8;
};
static_assert(__builtin_offsetof(UnkMoveCopy_b0204, field_0x7e8) == 0x7e8, "layout");
static_assert(__builtin_offsetof(UnkMoveCopy_b0204, field_0x200) == 0x200, "layout");

struct UnkMoveSource_b0204 {
    u8 pad_0x0[0x1f0];
    UnkMoveCopy_b0204* field_0x1f0;
};

void fn_71010da468(UnkMoveCopy_b0204* self, UnkMoveSource_b0204* other) {
    if (!other)
        return;
    UnkMoveCopy_b0204* dst = other->field_0x1f0;
    if (dst) {
        const base::reflection::CClass* cls = reinterpret_cast<base::reflection::UnkReflObject*>(dst)->vf_0x0();
        fn_71010d68bc();
        if (!fn_710009d50c(cls, &g_unk_7172a03fd0))
            dst = nullptr;
    }
    dst->field_0x200 = self->field_0x200;
    dst->field_0x204 = self->field_0x204;
    u8 mode = self->field_0x7e4;
    u8 old = dst->field_0x7e4;
    dst->field_0x7e4 = mode;
    if (old != mode)
        reinterpret_cast<void (*)(UnkMoveCopy_b0204*)>(dst->vtable[0x3f8 / 8])(dst);
    if (dst != self)
        CopyWeakRef(&dst->field_0x7e8, self->field_0x7e8);
    dst->field_0x7e4 = self->field_0x7e4;
    dst->field_0x7c = self->field_0x7c;
    dst->field_0x7b8 = self->field_0x7b8;
    dst->field_0x7b9 = self->field_0x7b9;
    dst->field_0x7ba = self->field_0x7ba;
    if (dst != self) {
        const UnkAssignedSlot* from0 = reinterpret_cast<const UnkAssignedSlot*>(self->slot_0x178);
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(dst->slot_0x178), from0);
        const UnkAssignedSlot* from1 = reinterpret_cast<const UnkAssignedSlot*>(self->slot_0x180);
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(dst->slot_0x180), from1);
        for (u32 i = 0; i < 13; i++)
            dst->field_0x188[i] = self->field_0x188[i];
    }
}

void fn_710006c76c(void* member, const CStrId* id);

struct UnkMovementTarget_b0204 {
    u8 pad_0x0[0x708];
    u8 member[1];  // 0x708
};

struct UnkMovementMethod_b0204 {
    u8 pad_0x0[8];
    UnkMovementTarget_b0204* target;
};

UnkReflValue fn_71010dd0d4(UnkMovementMethod_b0204* self, UnkReflValue* const* args) {
    UnkMovementTarget_b0204* target = self->target;
    UnkReflValue* arg = args[0];
    const char* text = arg->type ? base::reflection::refl_ArgValue<char>(arg) : nullptr;
    {
        char name[0x80];
        fn_710007cf30(const_cast<char*>(text), "s", name);
        CStrId id(name, true);
        fn_710006c76c(target->member, &id);
    }
    s32 zero = 0;
    const base::reflection::CType* type = fn_7100014768();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &zero, false);
    return r;
}
