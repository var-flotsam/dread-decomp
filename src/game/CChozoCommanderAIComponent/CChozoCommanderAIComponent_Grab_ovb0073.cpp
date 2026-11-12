// CChozoCommanderAIComponent's unit (tools/tu_map.py): the commander's reaction to a hit through the
// lazily created manager g_unk_71726af468 (fn_71004530fc), its bounds grown around the listed
// entries (fn_7100455554), the end of its grab (fn_7100454fc8) and the push of its target along its
// facing (fn_7100450ef0). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CCharClassLifeComponent.h"
#include "game/CComponent.h"
#include "game/CShotManager/CTunableShotManager.h"
#include "game/UnkActor.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkBoundsComponent;
struct UnkAabb;

static inline UnkManager_71726af468* Manager_ovb0073() {
    UnkManager_71726af468* m = g_unk_71726af468;
    if (!m) {
        m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return m;
}

struct UnkCommanderHit_ovb0073 {
    u8 pad_0x0[0xb4];
    u16 flags;  // 0xb4
};

struct UnkCommander_ovb0073 {
    u8 pad_0x0[0x15d71];
    bool field_0x15d71;
};

// Unless the manager says otherwise (fn_7100da9254), passes it a value from the hit's flags
// (fn_7100da929c) and marks the commander.
void fn_71004530fc(UnkCommander_ovb0073* self, const UnkCommanderHit_ovb0073* hit) {
    if (fn_7100da9254(Manager_ovb0073()))
        return;
    f32 value = (hit->flags & 0x504) ? 0.09f : 0.0f;
    fn_7100da929c(Manager_ovb0073(), value);
    self->field_0x15d71 = true;
}

struct UnkVec3_ovb0073 {
    f32 x, y, z;
};

struct UnkAabb_ovb0073 {
    UnkVec3_ovb0073 min;
    UnkVec3_ovb0073 max;
};

struct UnkCommanderTransform_ovb0073 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkCommanderEntryOwner_ovb0073 {
    u8 pad_0x0[0x18];
    UnkCommanderTransform_ovb0073* transform;  // 0x18
};

struct UnkCommanderEntry_ovb0073 {
    u8 pad_0x0[0x10];
    UnkCommanderEntryOwner_ovb0073* owner;  // 0x10
};

struct UnkCommanderList_ovb0073 {
    UnkCommanderEntry_ovb0073** data;
    u32 count;
};

UnkCommanderList_ovb0073* fn_7100459f94();

static inline f32 Min_ovb0073(f32 a, f32 b) { return a < b ? a : b; }
static inline f32 Max_ovb0073(f32 a, f32 b) { return a > b ? a : b; }

static inline void Grow_ovb0073(UnkAabb_ovb0073* box, const UnkVec3_ovb0073& p) {
    box->min = UnkVec3_ovb0073{Min_ovb0073(box->min.x, p.x), Min_ovb0073(box->min.y, p.y), Min_ovb0073(box->min.z, p.z)};
    box->max = UnkVec3_ovb0073{Max_ovb0073(box->max.x, p.x), Max_ovb0073(box->max.y, p.y), Max_ovb0073(box->max.z, p.z)};
}

// The base bounds, grown to take in a 600-unit square around each listed entry.
void fn_7100455554(UnkBoundsComponent* self, UnkAabb_ovb0073* out) {
    fn_71011061d0(self, reinterpret_cast<UnkAabb*>(out));
    if (fn_7100459f94()->count == 0)
        return;
    UnkCommanderList_ovb0073* list = fn_7100459f94();
    for (UnkCommanderEntry_ovb0073 **it = list->data, **end = it + list->count; it != end; ++it) {
        UnkCommanderTransform_ovb0073* t = (*it)->owner->transform;
        if (!t)
            continue;
        f32 x = t->x;
        f32 y = t->y;
        f32 z = t->z;
        UnkVec3_ovb0073 lo = {x - 300.0f, y - 300.0f, z};
        UnkVec3_ovb0073 hi = {x + 300.0f, y + 300.0f, z};
        Grow_ovb0073(out, lo);
        Grow_ovb0073(out, hi);
    }
}

static inline void Zero_ovb0073(UnkVec3_ovb0073* v) {
    v->x = 0.0f;
    v->y = 0.0f;
    v->z = 0.0f;
}

template <u32 Offset>
struct UnkSmGrab_A1_b64;
void fn_7100440f5c(UnkSmGrab_A1_b64<0x88>*);
void fn_71004330cc(void* attack);
struct UnkGroupIds;

class UnkCommanderAttack_ovb0073 {
public:
    virtual const base::reflection::CType* vf_0x0();
};

struct UnkCommanderAttacks_ovb0073 {
    u8 pad_0x0[0x1a0];
    UnkCommanderAttack_ovb0073* current;  // 0x1a0
};

class UnkCommanderTimer_ovb0073 {
public:
    virtual void vf_0x0();
    u8 pad_0x8[0x48];
    f32 time;  // 0x50
    f32 limit;  // 0x54

    void Fire(bool on) { reinterpret_cast<void (***)(UnkCommanderTimer_ovb0073*, bool)>(this)[0][0x168 / 8](this, on); }
};

struct UnkCommanderActor_ovb0073 {
    u8 pad_0x0[0x1b8];
    UnkCommanderTimer_ovb0073* timer;  // 0x1b8
    u8 pad_0x1c0[0xb8];
    UnkCommanderAttacks_ovb0073* attacks;  // 0x278
};

struct UnkCommanderAI_ovb0073 {
    u8 pad_0x0[0x30];
    UnkCommanderActor_ovb0073* actor;  // 0x30
    u8 pad_0x38[0x1140 - 0x38];
    s32 field_0x1140;
    u8 pad_0x1144[0x15d71 - 0x1144];
    bool field_0x15d71;
    u8 pad_0x15d72[0x15ebc - 0x15d72];
    UnkVec3_ovb0073 field_0x15ebc;
};

template <const base::reflection::CClass* (*GetClass)()>
static inline bool IsA_ovb0073(UnkCommanderAttack_ovb0073* obj) {
    const base::reflection::CType* type = obj->vf_0x0();
    return fn_710009d50c(type, reinterpret_cast<const base::reflection::CType*>(GetClass()));
}

// Ends the commander's grab: the current grab attack (ultimate or ki) is released, the sound
// player stopped, and unless +0x1140 is set the actor's timer fires once it has run out.
void fn_7100454fc8(UnkCommanderAI_ovb0073* self) {
    Zero_ovb0073(&self->field_0x15ebc);
    if (UnkCommanderAttacks_ovb0073* attacks = self->actor->attacks) {
        UnkCommanderAttack_ovb0073* attack = attacks->current;
        if (attack && IsA_ovb0073<fn_710043d5e8>(attack)) {
            fn_7100440f5c(reinterpret_cast<UnkSmGrab_A1_b64<0x88>*>(attack));
        } else {
            attack = attacks->current;
            if (attack && IsA_ovb0073<fn_710042fea8>(attack))
                fn_71004330cc(attack);
        }
    }
    fn_7100ed78c0(g_unk_71726af0b0);
    if (self->field_0x1140 == 0 && self->actor) {
        if (UnkCommanderTimer_ovb0073* timer = self->actor->timer) {
            f32 delay = *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_717272fd38) + 0x94);
            if (timer->time - delay >= timer->limit)
                timer->Fire(true);
        }
    }
    self->field_0x15d71 = false;
}

struct UnkParent;

struct UnkCommanderPush_ovb0073 {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x15d07 - 0x38];
    bool push_pending;  // 0x15d07
};
void fn_7100451000(UnkCommanderPush_ovb0073* self, f32 dt);

// Sets the actor's position through its vf_0xd0 (with the flag set).
static inline void SetPosition_ovb0073(UnkActor* actor, UnkVec3_ovb0073* position) {
    reinterpret_cast<void (***)(UnkActor*, UnkVec3_ovb0073*, bool)>(actor)[0][0xd0 / 8](actor, position, true);
}

// The base update (fn_71010c424c); then, once after a push was asked for, moves the target 250
// units ahead of the commander along its facing, at most 150 units at a time; then fn_7100451000.
void fn_7100450ef0(UnkCommanderPush_ovb0073* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    if (self->push_pending) {
        self->push_pending = false;
        if (UnkActor* target = fn_7100d7c990(self->actor)) {
            f32 y = target->field_0x54;
            f32 z = target->field_0x64;
            UnkVec3_ovb0073 pos;
            pos.y = y;
            pos.z = z;
            UnkActor* actor = self->actor;
            f32 x = actor->field_0x44;
            f32 facing = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38)).m[2];
            pos.x = x + (f32)((facing > 0.0f) - (facing < 0.0f)) * 250.0f;
            f32 current = target->field_0x44;
            f32 step = pos.x - current;
            if (__builtin_fabsf(pos.x - current) > 150.0f) {
                if (step <= -150.0f)
                    step = -150.0f;
                else
                    step = step > 150.0f ? 150.0f : step;
            }
            pos.x = current + step;
            SetPosition_ovb0073(target, &pos);
        }
    }
    fn_7100451000(self, dt);
}
