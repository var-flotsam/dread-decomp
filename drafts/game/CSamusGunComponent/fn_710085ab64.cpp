// CSamusGunComponent's unit (tools/tu_map.py unit 1315): handles a shot reaching an actor. While holding,
// a hit on another actor's collision is refused (6), and a hit repeated within 0.2 time units is too;
// otherwise it is listed. The actor's damage target may then deflect it before it receives a damage request
// built from the hit. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CGunComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2523.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

#include <new>

using base::reflection::UnkReflObject;

struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;
struct UnkRequest;

extern u64 g_unk_710164ad70;
extern u64 g_unk_710164ad78;

struct UnkShotTuning_l85ab64 {
    u8 pad_0x0[0x24];
    f32 held_damage;  // 0x24
};
extern UnkShotTuning_l85ab64 g_unk_7172728150;

struct UnkVec3_l85ab64 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkHitRequest_l85ab64 {
    u8 pad_0x0[0x8];
    UnkWeakHandle* source;  // 0x8
    u8 pad_0x10[0x50];
    void* collider;  // 0x60
    s32 field_0x68;
    UnkVec3_l85ab64 point;  // 0x6c
    f32 field_0x78;
    f32 field_0x7c;
    u32 field_0x80;
    UnkVec3_l85ab64 normal;  // 0x84
    u32 field_0x90;
    u32 field_0x94;
    void* other;  // 0x98
    u8 pad_0xa0[0x14];
    u32 flags;  // 0xb4
    u8 pad_0xb8[0x48];

    UnkHitRequest_l85ab64() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkHitRequest_l85ab64() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(sizeof(UnkHitRequest_l85ab64) == 0x100, "request layout");

struct UnkShotPart_l85ab64 {
    u8 pad_0x0[0x10];
    void* object;  // 0x10
};

struct UnkCollider_l85ab64;
struct UnkColliderVtable_l85ab64 {
    u8 pad_0x0[0x58];
    u32 (*vf_0x58)(UnkCollider_l85ab64* self);
};
struct UnkCollider_l85ab64 {
    UnkColliderVtable_l85ab64* vtable;
    u8 pad_0x8[0x8];
    UnkReflObject* component;  // 0x10
};

struct UnkShotRef_l85ab64 {
    u8 pad_0x0[0x10];
    UnkCollider_l85ab64* collider;  // 0x10
};

struct UnkShotHit_l85ab64 {
    UnkShotRef_l85ab64* ref;  // 0x0
    UnkShotPart_l85ab64* part;  // 0x8
    u8 pad_0x10[0x4];
    UnkVec3_l85ab64 point;  // 0x14
    u8 pad_0x20[0xc];
    UnkVec3_l85ab64 normal;  // 0x2c
};

struct UnkActor_l85ab64;
struct UnkDamageTarget_l85ab64;
struct UnkDamageTargetVtable_l85ab64 {
    u8 pad_0x0[0x188];
    s32 (*vf_0x188)(UnkDamageTarget_l85ab64* self, UnkHitRequest_l85ab64* request);
    s32 (*vf_0x190)(UnkDamageTarget_l85ab64* self, UnkActor_l85ab64* by, UnkHitRequest_l85ab64* request,
                    UnkShotHit_l85ab64* hit);
};
struct UnkDamageTarget_l85ab64 {
    UnkDamageTargetVtable_l85ab64* vtable;
};

struct UnkActor_l85ab64 {
    u8 pad_0x0[0x1b8];
    UnkDamageTarget_l85ab64* damage_target;  // 0x1b8
    u8 pad_0x1c0[0x2a0 - 0x1c0];
    UnkReflObject* collision;  // 0x2a0
};

struct UnkCollisionComponent_l85ab64 : UnkReflObject {
    u8 pad_0x8[0x28];
    UnkActor_l85ab64* owner;  // 0x30
};

struct UnkGun_l85ab64;
struct UnkGunVtable_l85ab64 {
    u8 pad_0x0[0x180];
    f32 (*vf_0x180)(UnkGun_l85ab64* self);
    f32 (*vf_0x188)(UnkGun_l85ab64* self);
    u8 pad_0x190[0x8];
    u32 (*vf_0x198)(UnkGun_l85ab64* self);
};
struct UnkGun_l85ab64 {
    UnkGunVtable_l85ab64* vtable;
};

// One listed hit: whom it reached, an unset cell and when.
struct UnkShotRecord_l85ab64 {
    UnkWeakHandle* handle;
    s32 cell_x;
    s32 cell_y;
    f32 time;

    UnkShotRecord_l85ab64(UnkWeakHandle* target, f32 now) : handle(nullptr) {
        AssignRef(&handle, target);
        cell_x = (s32)0x80000000;
        cell_y = (s32)0x80000000;
        time = now;
    }
};

void CSamusGunComponent_ReserveRefEntryVector(base::global::CRntVector<UnkShotRecord_l85ab64>* self, u32 capacity);

struct UnkSamusGun_l85ab64 {
    u8 pad_0x0[0x30];
    UnkActor_l85ab64* owner;  // 0x30
    u8 pad_0x38[0x174 - 0x38];
    f32 charge_needed;  // 0x174
    bool charging;  // 0x178
    u8 pad_0x179[3];
    f32 charge;  // 0x17c
    u8 pad_0x180[0x330 - 0x180];
    bool field_0x330;
    u8 pad_0x331[0xf];
    bool holding;  // 0x340
    u8 pad_0x341[7];
    base::global::CRntVector<UnkShotRecord_l85ab64> records;  // 0x348
};
static_assert(__builtin_offsetof(UnkSamusGun_l85ab64, records) == 0x348, "layout");

// A counted reference held for a scope.
struct UnkHandleRef_l85ab64 {
    UnkWeakHandle* handle;

    explicit UnkHandleRef_l85ab64(UnkWeakHandle* h) : handle(h) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkHandleRef_l85ab64() {
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
};

static inline UnkGun_l85ab64* Gun(UnkSamusGun_l85ab64* self) {
    return static_cast<UnkGun_l85ab64*>(fn_710083cd30(reinterpret_cast<UnkComponent*>(self)));
}

static inline f32 GunDamage(UnkSamusGun_l85ab64* self) {
    UnkGun_l85ab64* gun = Gun(self);
    return gun->vtable->vf_0x180(gun);
}

static inline f32 GunMultiplier(UnkSamusGun_l85ab64* self) {
    UnkGun_l85ab64* gun = Gun(self);
    return gun->vtable->vf_0x188(gun);
}

static inline u32 GunFlags(UnkSamusGun_l85ab64* self) {
    UnkGun_l85ab64* gun = Gun(self);
    return gun->vtable->vf_0x198(gun);
}

static inline bool IsCharged(UnkSamusGun_l85ab64* self) {
    return self->charging && self->charge >= self->charge_needed;
}

static inline UnkWeakTarget* TargetOf(UnkWeakHandle* handle) { return handle ? handle->target : nullptr; }

// Whether this hit on the actor was listed less than 0.2 time units ago (always, when not holding).
static inline bool IsRepeat(UnkSamusGun_l85ab64* self, const UnkShotRecord_l85ab64& record, UnkWeakHandle* handle,
                            f32 now) {
    return TargetOf(record.handle) == TargetOf(handle) && record.cell_x == (s32)0x80000000 &&
           record.cell_y == (s32)0x80000000 && (!self->holding || now - record.time < 0.2f);
}

static inline void AddRecord(UnkSamusGun_l85ab64* self, UnkWeakHandle* handle, f32 now) {
    base::global::CRntVector<UnkShotRecord_l85ab64>& records = self->records;
    u32 n = records.count;
    u32 capacity = records.capacity;
    if (capacity <= n && records.step != 0)
        capacity = records.GrownCapacity(n);
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    CSamusGunComponent_ReserveRefEntryVector(&records, capacity);
    new (&records.data[records.count]) UnkShotRecord_l85ab64(handle, now);
    ++records.count;
}

// The damage target's first look at the hit; 2 stops it.
static inline s32 PreviewHit(UnkSamusGun_l85ab64* self, UnkDamageTarget_l85ab64* target, UnkShotHit_l85ab64* hit) {
    UnkHitRequest_l85ab64 request;
    request.flags = GunFlags(self);
    return target->vtable->vf_0x190(target, self->owner, &request, hit);
}

// While holding: a hit on another actor's collision is refused (6), and so is one listed less than 0.2 time
// units ago; any other hit on an actor without a billboard collision is listed (0).
static inline s32 FilterHeldHit(UnkSamusGun_l85ab64* self, UnkActor_l85ab64* actor, UnkCollider_l85ab64* collider) {
    // The cast is read through even when it fails, as in the original.
    UnkActor_l85ab64* hitOwner = nullptr;
    if (collider) {
        UnkReflObject* component = collider->component;
        hitOwner = (fn_710009d50c(component->vf_0x0(), fn_710085fc44())
                        ? static_cast<UnkCollisionComponent_l85ab64*>(component)
                        : nullptr)
                       ->owner;
    }
    if (hitOwner != actor)
        return 6;

    UnkHandleRef_l85ab64 handle(actor ? fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(actor)) : nullptr);
    f32 now = g_unk_7101d47310->field_0x2444;
    if (actor && actor->collision && fn_710009d50c(actor->collision->vf_0x0(), fn_7100723914()))
        return 0;
    for (UnkShotRecord_l85ab64& record : self->records) {
        if (IsRepeat(self, record, handle.handle, now))
            return 6;
    }
    AddRecord(self, handle.handle, now);
    return 0;
}

s32 fn_710085ab64(UnkSamusGun_l85ab64* self, UnkActor_l85ab64* actor, UnkShotHit_l85ab64* hit) {
    UnkCollider_l85ab64* collider = hit->ref ? hit->ref->collider : nullptr;
    bool onCollision = false;
    if (collider) {
        if (collider->vtable->vf_0x58(collider) & 0x80)
            onCollision = true;
        else if (collider->vtable->vf_0x58(collider) & 0x100)
            return 2;
        else
            return 6;
    }

    if (self->holding && FilterHeldHit(self, actor, onCollision ? collider : nullptr) == 6)
        return 6;
    UnkDamageTarget_l85ab64* target;
    if (actor && (target = actor->damage_target) && PreviewHit(self, target, hit) == 2)
        return 0;

    UnkHitRequest_l85ab64 request;
    request.field_0x68 = -1;
    request.point = hit->point;
    request.field_0x78 = 0.0f;
    request.field_0x7c = 1.0f;
    request.field_0x80 = 0;
    request.normal = hit->normal;
    request.field_0x90 = 0;
    request.field_0x94 = 0x38;
    request.collider = hit->ref ? hit->ref->collider : nullptr;
    request.other = hit->part ? hit->part->object : nullptr;
    AssignRef(&request.source, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(self->owner)));
    if (self->holding) {
        fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), g_unk_7172728150.held_damage);
        request.flags = 0x40;
    } else {
        f32 damage = GunDamage(self);
        damage *= GunMultiplier(self);
        fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), damage);
        request.flags = GunFlags(self);
        if (IsCharged(self))
            request.flags |= 0x400;
        if (self->field_0x330)
            request.flags |= 0x800000;
    }

    s32 result = 0;
    if (actor && (target = actor->damage_target)) {
        result = target->vtable->vf_0x188(target, &request);
        if (result == 1) {
            bool holding = self->holding;
            if (holding || IsCharged(self))
                fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), holding ? &g_unk_710164ad70 : &g_unk_710164ad78,
                              false);
            result = 1;
        }
    }
    return result;
}
