// CShootActivatorComponent (unit of CCharClassShootActivatorComponent, tools/tu_map.py): fn_7100891c8c
// handles a hit after the base handler (fn_710024fbc8). While the timer at +0x4bc is below the duration at
// +0x4b0: a hit with any of the damage flags 0x08000400 (+0xb4) fills the timer up, any other adds the
// increment at +0x4b4 (clamped); then the slot-0x1f0 update runs. When the timer is full and the id at +0x4d8
// is set, the id at +0x4d0 is released, the owner (+0x30) gets the id's key (fn_7100d7c828) and the manager
// fn_7100df5440 finds is stopped (fn_71002fda50). The owner always gets the key g_unk_710164aed8. Returns
// false. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

bool fn_710024fbc8(void*);
struct UnkStruct_71002fda50;
void fn_71002fda50(UnkStruct_71002fda50* self, bool all, bool force_off);
void* fn_7100df5440(UnkGameManager* manager);
extern const u64 g_unk_710164aed8;  // reached through the GOT

struct UnkShootHit_ovb0126 {
    u8 pad_0x0[0xb4];
    u32 flags;
};

struct UnkShootActivatorHit_ovb0126;
struct UnkShootActivatorHitVtable_ovb0126 {
    void* unk_0x0[0x1f0 / 8];
    void (*vf_0x1f0)(UnkShootActivatorHit_ovb0126* self);
};

struct UnkShootActivatorHit_ovb0126 {
    UnkShootActivatorHitVtable_ovb0126* vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x4b0 - 0x38];
    f32 duration;   // 0x4b0
    f32 increment;  // 0x4b4
    u8 pad_0x4b8[4];
    f32 timer;  // 0x4bc
    u8 pad_0x4c0[0x10];
    CStrId field_0x4d0;
    CStrId field_0x4d8;
};

static inline f32 Clamp(f32 v, f32 min, f32 max) {
    f32 r = v >= max ? max : v;
    if (v <= min)
        return min;
    return r;
}

bool fn_7100891c8c(UnkShootActivatorHit_ovb0126* self, const UnkShootHit_ovb0126* hit) {
    if (!fn_710024fbc8(self))
        return false;
    f32 timer = self->timer;
    f32 duration = self->duration;
    if (!(timer < duration))
        return false;
    f32 t;
    if (hit->flags & 0x08000400)
        t = duration <= 0.0f ? 0.0f : duration;
    else
        t = Clamp(timer + self->increment, 0.0f, duration);
    self->timer = t;
    self->vtable->vf_0x1f0(self);
    if (self->field_0x4d8.data != &g_unk_7101d47108->empty && self->timer >= self->duration) {
        self->field_0x4d0.Reset();
        UnkActor* owner = self->owner;
        u64 key = *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(self->field_0x4d8.data) + 0x30);
        fn_7100d7c828(owner, &key, false);
        if (void* manager = fn_7100df5440(g_unk_7101d47310))
            fn_71002fda50(static_cast<UnkStruct_71002fda50*>(manager), true, false);
    }
    fn_7100d7c828(self->owner, &g_unk_710164aed8, false);
    return false;
}

// fn_71008919c4: places the scenario object of the actor at +0x18 (fn_7100774014) relative to the player: the
// player's position minus the actor's and the difference of their y angles (fn_710116c128), as
// fn_7100a7d594 (src/game/CElectricGeneratorComponent/CElectricGeneratorComponent_Arrays_ovb0120.cpp) does,
// here asking for the player again for the angle and without null tests.
struct UnkScenarioObject_ovb0032;
void* fn_7100774014(UnkScenarioObject_ovb0032* actor);
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkGeneratorTarget_ovb0120;
void fn_710116c128(UnkGeneratorTarget_ovb0120* self, const UnkVec3f* offset, const UnkVec3f* angle, bool flag);

struct UnkShootPlaceActor_ovb0126 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkShootPlace_ovb0126 {
    u8 pad_0x0[0x18];
    UnkShootPlaceActor_ovb0126* actor;
};

static inline UnkShootPlaceActor_ovb0126* Player() {
    return reinterpret_cast<UnkShootPlaceActor_ovb0126*>(fn_7100dec788(g_unk_7101d47310));
}

void fn_71008919c4(UnkShootPlace_ovb0126* self) {
    auto* target = static_cast<UnkGeneratorTarget_ovb0120*>(
        fn_7100774014(reinterpret_cast<UnkScenarioObject_ovb0032*>(self->actor)));
    if (!target)
        return;
    UnkVec3f from = Player()->xform.GetTranslation();
    UnkVec3f to = self->actor->xform.GetTranslation();
    UnkVec3f offset(from.x - to.x, from.y - to.y, from.z - to.z);
    f32 a = Player()->xform.GetAngle().y;
    f32 b = self->actor->xform.GetAngle().y;
    UnkVec3f angle(0.0f, a - b, 0.0f);
    fn_710116c128(target, &offset, &angle, true);
}

// fn_71008917c4: with the camera holder's (fn_7100defcec) current controller (fn_71010e2080): when it is a
// CAnimatedCameraCtrl (fn_7100023d00), the use count at +0x44 of its animation (+0x2b0) goes up, the id at
// +0x4d0 takes the animation's name (+0x10), and the player's input component (+0x1d8) is switched on with it
// (fn_7100872460), its +0x2a8 object off (fn_7100839b00) when that has the flag at +0x178; otherwise, with an
// id at +0x4d0, the input component is switched off with it, the id released, and the scenario object placed
// (fn_71008919c4).
struct UnkInputComponent;
void fn_7100872460(UnkInputComponent* self, bool enable, bool reset, void* arg);
bool fn_7100023d00(const UnkCameraCtrl* object, const base::reflection::CClass* cls);
struct UnkShootPlayerPart_ovb0126 {
    u8 pad_0x0[0x178];
    bool field_0x178;
};
void fn_7100839b00(char* self, bool value);

struct UnkShootCameraAnim_ovb0126 {
    u8 pad_0x0[0x10];
    base::global::CRntString name;
    u8 pad_0x10[0x44 - 0x10 - sizeof(base::global::CRntString)];
    union {
        s32 use_count;  // 0x44 (no type-based alias info: the original reloads the animation after it)
    };
};

struct UnkShootAnimatedCamera_ovb0126 {
    u8 pad_0x0[0x2b0];
    UnkShootCameraAnim_ovb0126* animation;
};

struct UnkShootPlayer_ovb0126 {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;  // 0x1d8
    u8 pad_0x1e0[0x2a8 - 0x1e0];
    UnkShootPlayerPart_ovb0126* part;  // 0x2a8
};

// A string id built from a string out of line (fn_7100000850); moved from, its release folds away.
struct UnkShootBuiltId_ovb0126 {
    union {
        base::global::UnkStrId id;
    };

    explicit UnkShootBuiltId_ovb0126(const base::global::CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkShootBuiltId_ovb0126() { id.~UnkStrId(); }
};

struct UnkShootCamera_ovb0126 {
    u8 pad_0x0[0x4d0];
    base::global::UnkStrId id;
};

// The id's move assignment: the old string released (count tested in two steps, the pool reloaded after a
// release), then the other's taken, the other left the empty string of a freshly loaded pool.
static inline void MoveId(base::global::UnkStrId* self, base::global::UnkStrId* other) {
    using base::global::UnkStringPool;
    UnkStringPool* pool = g_unk_7101d47108;
    if (pool) {
        if (self->data && self->data != &pool->empty) {
            --self->data->ref_count;
            if (self->data->ref_count == 0) {
                fn_71000803a4(pool, self->data);
                pool = g_unk_7101d47108;
            }
        }
        self->data = &pool->empty;
    }
    self->data = other->data;
    other->data = &g_unk_7101d47108->empty;
}

void fn_71008917c4(UnkShootCamera_ovb0126* self) {
    void* holder = fn_7100defcec(g_unk_7101d47310);
    if (!holder)
        return;
    UnkCameraCtrl* ctrl = fn_71010e2080(holder);
    if (!ctrl)
        return;
    if (fn_7100023d00(ctrl, fn_71010e7f90())) {
        auto* camera = reinterpret_cast<UnkShootAnimatedCamera_ovb0126*>(
            fn_710009d50c(ctrl->vf_0x0(), fn_71010e7f90()) ? ctrl : nullptr);
        ++camera->animation->use_count;
        {
            UnkShootBuiltId_ovb0126 name(camera->animation->name);
            MoveId(&self->id, &name.id);
        }
        auto* player = reinterpret_cast<UnkShootPlayer_ovb0126*>(fn_7100dec788(g_unk_7101d47310));
        if (!player)
            return;
        fn_7100872460(player->input, true, false, &self->id);
        if (UnkShootPlayerPart_ovb0126* part = player->part) {
            if (part->field_0x178)
                fn_7100839b00(reinterpret_cast<char*>(part), false);
        }
        return;
    }
    if (self->id.data == &g_unk_7101d47108->empty)
        return;
    if (auto* player = reinterpret_cast<UnkShootPlayer_ovb0126*>(fn_7100dec788(g_unk_7101d47310)))
        fn_7100872460(player->input, false, false, &self->id);
    reinterpret_cast<CStrId*>(&self->id)->Reset();
    fn_71008919c4(reinterpret_cast<UnkShootPlace_ovb0126*>(self));
}
