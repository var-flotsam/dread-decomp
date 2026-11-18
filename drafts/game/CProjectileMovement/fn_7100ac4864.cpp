// CProjectileMovement's unit (tools/tu_map.py, unit 1623): reacts to a hit. Unless the hit actor is held
// (state kind 2), classifies the hit (fn_7100ac82c0) and either deflects (re-aims through slot 0x348) or stops
// (slot 0x350), starting the owner's hit timeline once, copying the position back to the owner and telling the
// listener for some classes; then runs the owner's "OnCollision" script. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CTimelineComponent.h"
#include "game/CWeaponMovement.h"
#include "game/UnkTU54.h"
#include "types.h"

extern const u8 g_unk_710164b89c[] __attribute__((visibility("hidden")));
extern u64 g_unk_71727556c8 __attribute__((visibility("hidden")));  // the stop timeline's name

struct UnkHitState_lac4864 {
    u8 pad_0x0[0x70];
    s32 kind;  // 0x70
};

struct UnkHitActor_lac4864 {
    u8 pad_0x0[0x1b8];
    UnkHitState_lac4864* state;  // 0x1b8
    u8 pad_0x1c0[0x80];
    void* field_0x240;

    bool IsHeld() const { return field_0x240 && state && state->kind == 2; }
};

struct UnkHitOwner_lac4864 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38
    u8 pad_0x68[0x178];
    UnkScriptCaller* script;  // 0x1e0
    u8 pad_0x1e8[0x58];
    void* field_0x240;
    u8 pad_0x248[0xa0];
    void* timeline;  // 0x2e8
    u8 pad_0x2f0[0xc0];
    bool moved;  // 0x3b0

    UnkVec3f Position() const { return UnkVec3f(transform[0][3], transform[1][3], transform[2][3]); }
};

struct UnkHitListener_lac4864 {
    void** vtable;

    void vf_0x160(u32 id, const UnkVec3f& pos, UnkHitActor_lac4864* other, void* extra) {
        reinterpret_cast<void (*)(UnkHitListener_lac4864*, u32, const UnkVec3f*, UnkHitActor_lac4864*, void*)>(
            vtable[0x160 / 8])(this, id, &pos, other, extra);
    }
    void vf_0x168(u32 id, const UnkVec3f& pos, UnkHitActor_lac4864* other, void* extra) {
        reinterpret_cast<void (*)(UnkHitListener_lac4864*, u32, const UnkVec3f*, UnkHitActor_lac4864*, void*)>(
            vtable[0x168 / 8])(this, id, &pos, other, extra);
    }
};

struct UnkHitLink_lac4864 {
    u8 pad_0x0[0x10];
    UnkHitListener_lac4864* listener;  // 0x10
};

struct UnkHitDef_lac4864 {
    u8 pad_0x0[0xec];
    bool sync_owner;  // 0xec
};

static inline UnkVec3f* Ptr(const UnkVec3f& v) { return const_cast<UnkVec3f*>(&v); }

struct UnkProjectileHit_lac4864 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkHitOwner_lac4864* owner;  // 0x30
    u8 pad_0x38[0x37c];
    UnkVec3f position;   // 0x3b4
    UnkVec3f direction;  // 0x3c0
    u8 pad_0x3cc[0x30];
    u32 flags;  // 0x3fc
    u8 pad_0x400[0xd0];
    UnkHitLink_lac4864* link;  // 0x4d0
    u8 pad_0x4d8[0x4c];
    bool timeline_started;  // 0x524
    bool stopped;           // 0x525
    u8 pad_0x526[0x72];
    base::global::CStrId timeline_id;  // 0x598
    u8 pad_0x5a0[0x28];
    UnkHitDef_lac4864* def;  // 0x5c8

    void* vf_0x2d8() { return reinterpret_cast<void* (*)(UnkProjectileHit_lac4864*)>(vtable[0x2d8 / 8])(this); }
    void vf_0x348(UnkVec3f* at, UnkVec3f* angle, const void* a, const void* b) {
        reinterpret_cast<void (*)(UnkProjectileHit_lac4864*, UnkVec3f*, UnkVec3f*, const void*, const void*)>(
            vtable[0x348 / 8])(this, at, angle, a, b);
    }
    void vf_0x350(UnkVec3f* at, UnkVec3f* angle) {
        reinterpret_cast<void (*)(UnkProjectileHit_lac4864*, UnkVec3f*, UnkVec3f*)>(vtable[0x350 / 8])(this, at,
                                                                                                       angle);
    }

    UnkVec3f Angles() { return ((UnkVec3f(*)(const UnkVec3f&))fn_710007a7f0)(direction); }

    void SyncOwner() {
        if (!def->sync_owner)
            return;
        UnkHitOwner_lac4864* o = owner;
        o->transform[0][3] = position.x;
        o->transform[1][3] = position.y;
        o->transform[2][3] = position.z;
        o->moved = true;
    }

    void Deflect(void* hit_timeline) {
        vf_0x348(&position, Ptr(Angles()), g_unk_710164b89c, &g_unk_7101d470e8);
        if (hit_timeline && !timeline_started) {
            fn_7100a2cddc(hit_timeline, vf_0x2d8(), &timeline_id, 0.0f, false);
            timeline_started = true;
        }
        SyncOwner();
    }

    void Stop(void* hit_timeline) {
        stopped = true;
        vf_0x350(&position, Ptr(Angles()));
        if (hit_timeline && !timeline_started) {
            fn_7100a2cddc(hit_timeline, &g_unk_71727556c8, &timeline_id, 0.0f, false);
            timeline_started = true;
        }
        SyncOwner();
    }

    void React(s32 result, void* hit_timeline) {
        switch (result) {
        case 0:
        case 6:
        case 8:
            Stop(hit_timeline);
            break;
        case 11:
            break;
        default:
            Deflect(hit_timeline);
            break;
        }
    }

    void NotifyDeflected(UnkHitActor_lac4864* other) {
        if (link && link->listener)
            link->listener->vf_0x160(flags, owner->Position(), other, nullptr);
    }
    void NotifyStopped(UnkHitActor_lac4864* other) {
        if (link && link->listener)
            link->listener->vf_0x168(flags, owner->Position(), other, nullptr);
    }
};
static_assert(__builtin_offsetof(UnkProjectileHit_lac4864, def) == 0x5c8, "layout");

s32 fn_7100ac4864(UnkProjectileHit_lac4864* self, UnkHitActor_lac4864* other) {
    s32 result = fn_7100ac82c0(reinterpret_cast<UnkSplash_b59a4*>(self), other);
    u32 flags = self->flags;
    bool held = (flags & 0xc) == 0 && other != nullptr && (flags & 2) == 0 && other->IsHeld();
    if (!held) {
        UnkHitOwner_lac4864* owner = self->owner;
        void* hit_timeline = owner->timeline;
        switch (result) {
        case 0:
            if (owner->field_0x240) {
                self->React(result, hit_timeline);
                break;
            }
            // A plain hit on an owner without a target is a deflection.
            [[fallthrough]];
        case 1:
        case 7:
            self->Deflect(hit_timeline);
            self->NotifyDeflected(other);
            break;
        case 2:
            self->Stop(hit_timeline);
            self->NotifyStopped(other);
            break;
        default:
            self->React(result, hit_timeline);
            break;
        }
    }
    if (UnkScriptCaller* script = self->owner->script)
        fn_7100305088(script, "OnCollision", g_unk_710162e2ed);
    return result;
}
