// CAttackComponent unit (tools/tu_map.py unit 2706): handles a hit on an actor. Copies the hit's
// data, lets a speed booster or shinespark (or the Samus gun) absorb it, asks the actor's life
// component, then walks the listeners and runs the hit callback. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::UnkReflObject;

struct UnkAbility;
struct UnkAbilitySet;
UnkAbility* fn_71003012f8(UnkAbilitySet* set, u64 id);
u64 fn_7100327d14();
u64 fn_710033b6a4();

struct UnkAttackTargets_ovb0050;
struct UnkAttackTargetsLock_ovb0050;
void fn_7101138188(UnkAttackTargets_ovb0050* self, void* target);
void fn_710113831c(UnkAttackTargetsLock_ovb0050* self);

struct UnkHitVec_l136bb4 {
    f32 x, y, z;
};

// The hit's data block (0x30 bytes), copied word by word.
struct UnkHitData_l136bb4 {
    u32 field_0x0;
    UnkHitVec_l136bb4 pos;  // 0x4
    UnkHitVec_l136bb4 dir;  // 0x10
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 flags;  // 0x2c

    // Copied as twelve words.
    void Assign(const UnkHitData_l136bb4& o) {
        u32* dst = reinterpret_cast<u32*>(this);
        const u32* src = reinterpret_cast<const u32*>(&o);
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        dst[4] = src[4];
        dst[5] = src[5];
        dst[6] = src[6];
        dst[7] = src[7];
        dst[8] = src[8];
        dst[9] = src[9];
        dst[10] = src[10];
        dst[11] = src[11];
    }
};

struct UnkHitInfo_l136bb4 {
    u8 pad_0x0[0x60];
    void* field_0x60;
    UnkHitData_l136bb4 data;  // 0x68
};

struct UnkHitAbility_l136bb4;
bool fn_710033c4cc(UnkHitAbility_l136bb4* self);
s32 fn_710033c500(UnkHitAbility_l136bb4* self, UnkHitInfo_l136bb4* info);
bool fn_710032d650(UnkHitAbility_l136bb4* self);
s32 fn_710032d684(UnkHitAbility_l136bb4* self, UnkHitInfo_l136bb4* info);

// A reference-counted object: count at 8, destroyed through vtable slot 2.
struct UnkHitRef_l136bb4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 count;
};

struct UnkHitResult_l136bb4 {
    UnkHitRef_l136bb4* a;
    UnkHitRef_l136bb4* b;
    u8 pad_0x10[0x38];

    static void Release(UnkHitRef_l136bb4* p) {
        if (p && --p->count == 0)
            p->vf_0x10();
    }
    ~UnkHitResult_l136bb4() {
        Release(b);
        Release(a);
    }
};

struct UnkHitShape_l136bb4 {
    s32 id;
    UnkHitVec_l136bb4 pos;      // 0x4
    UnkHitVec_l136bb4 neg_dir;  // 0x10
    UnkHitVec_l136bb4 dir;      // 0x1c
    u32 field_0x28;
    u32 field_0x2c;
};

void fn_71006d4d4c(UnkHitResult_l136bb4* out, void* param, UnkHitShape_l136bb4* shape, void* source);

struct UnkHitGun_l136bb4 {
    virtual const CClass* vf_0x0();
    u8 pad_0x8[0x328];
    bool field_0x330;
    u8 pad_0x331[0xf];
    bool field_0x340;
};
s32 fn_710085b110(UnkHitGun_l136bb4* self, UnkHitInfo_l136bb4* info, UnkHitResult_l136bb4* result);

struct UnkHitLife_l136bb4;
struct UnkHitLifeVtable_l136bb4 {
    void* slots[0x31];
    s32 (*vf_0x188)(UnkHitLife_l136bb4* self, UnkHitInfo_l136bb4* info);
};
struct UnkHitLife_l136bb4 {
    UnkHitLifeVtable_l136bb4* vtable;
    u8 pad_0x8[0x4c];
    f32 field_0x54;
    u8 pad_0x58[0x18];
    s32 state;  // 0x70
};

struct UnkHitActor_l136bb4 {
    u8 pad_0x0[0x1b8];
    UnkHitLife_l136bb4* life;  // 0x1b8
    u8 pad_0x1c0[0xe8];
    UnkHitGun_l136bb4* gun;  // 0x2a8
    u8 pad_0x2b0[0x30];
    UnkAbilitySet* abilities;  // 0x2e0
};

struct UnkHitListener_l136bb4 {
    u8 pad_0x0[0x8];
    void* ctx;
    bool (*fn)(void* ctx, bool arg);
};

struct UnkHitTargets_l136bb4 {
    u8 pad_0x0[0x50];
    UnkHitListener_l136bb4** data;  // 0x50
    u32 count;                      // 0x58
    u8 pad_0x5c[0x1c];
    // A union member: the original reloads the count after the increment.
    union {
        s32 lock;  // 0x78
    };
};

struct UnkAttackComponent_l136bb4;
struct UnkAttackComponentVtable_l136bb4 {
    void* slots[0x34];
    void (*vf_0x1a0)(UnkAttackComponent_l136bb4* self);
};
struct UnkAttackComponent_l136bb4 {
    UnkAttackComponentVtable_l136bb4* vtable;
    u8 pad_0x8[0x48];
    UnkHitData_l136bb4 hit;  // 0x50
    u8 pad_0x80[0x20];
    UnkHitTargets_l136bb4 targets;  // 0xa0
    void* callback_ctx;  // 0x120
    void (*callback)(void* ctx, UnkHitActor_l136bb4* actor, UnkHitInfo_l136bb4* info, f32 delta);  // 0x128
};
static_assert(__builtin_offsetof(UnkAttackComponent_l136bb4, callback) == 0x128, "layout");

struct UnkHitLock_l136bb4 {
    UnkHitTargets_l136bb4* targets;
};

template <const CClass* (*Getter)()>
static inline UnkHitAbility_l136bb4* CastAbility(UnkAbility* a) {
    UnkReflObject* obj = reinterpret_cast<UnkReflObject*>(a);
    if (!obj)
        return nullptr;
    return fn_710009d50c(obj->vf_0x0(), Getter()) ? reinterpret_cast<UnkHitAbility_l136bb4*>(obj) : nullptr;
}

s32 fn_7101136bb4(UnkAttackComponent_l136bb4* self, UnkHitActor_l136bb4* actor, UnkHitInfo_l136bb4* info,
                  void* unused, void* param) {
    self->hit.Assign(info->data);
    if (!actor)
        return 0;
    UnkHitLife_l136bb4* life = actor->life;
    if (!life || life->state == 2)
        return 0;
    f32 before = life->field_0x54;

    if (UnkAbilitySet* abilities = actor->abilities) {
        UnkHitAbility_l136bb4* booster =
            CastAbility<fn_710033a31c>(fn_71003012f8(abilities, fn_710033b6a4()));
        UnkHitAbility_l136bb4* spark =
            CastAbility<fn_71003255a4>(fn_71003012f8(abilities, fn_7100327d14()));
        if (booster && fn_710033c4cc(booster) && fn_710033c500(booster, info) == 1)
            return 6;
        if (spark && fn_710032d650(spark) && fn_710032d684(spark, info) == 1)
            return 6;
    }

    UnkHitGun_l136bb4* gun = actor->gun;
    if (gun && fn_710009d50c(gun->vf_0x0(), fn_7100857564()) && (info->data.flags & 8) &&
        (gun->field_0x330 || gun->field_0x340)) {
        UnkHitVec_l136bb4 dir = info->data.dir;
        UnkHitShape_l136bb4 shape;
        shape.pos = info->data.pos;
        shape.id = -1;
        shape.neg_dir.x = -dir.x;
        shape.neg_dir.y = -dir.y;
        shape.neg_dir.z = -dir.z;
        shape.dir = dir;
        shape.field_0x28 = 0;
        shape.field_0x2c = 0x38;
        s32 r;
        {
            UnkHitResult_l136bb4 result;
            fn_71006d4d4c(&result, param, &shape, info->field_0x60);
            r = fn_710085b110(gun, info, &result);
        }
        if (r == 6)
            return 6;
    }

    s32 r = life->vtable->vf_0x188(life, info);
    if (r != 1)
        return r;
    self->vtable->vf_0x1a0(self);
    UnkHitLock_l136bb4 lock{&self->targets};
    ++self->targets.lock;
    for (UnkHitListener_l136bb4** it = self->targets.data, **end = it + self->targets.count; it != end; ++it) {
        UnkHitListener_l136bb4* l = *it;
        if (!l || !l->fn || !l->fn(l->ctx, true))
            fn_7101138188(reinterpret_cast<UnkAttackTargets_ovb0050*>(&self->targets), l);
    }
    fn_710113831c(reinterpret_cast<UnkAttackTargetsLock_ovb0050*>(&lock));
    if (self->callback)
        self->callback(self->callback_ctx, actor, info, before - life->field_0x54);
    return 1;
}
