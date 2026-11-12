// CMeleeComponent's unit (tools/tu_map.py unit 1421): the melee component's checks and effects. fn_71009298c4 sends
// the type id to the owner's (+0x30) timeline (+0x2e8) under the key g_unk_710164b270 (as the generated
// refl_SendTypeId does); fn_710092dfd4 returns one of two parts of the reflection singleton (+0x1788 or +0x1778);
// fn_7100929404 looks for a pending event of kind 1 with id 0x80 (0x1c-byte entries at +0x80, count at +0x7c),
// tells the owner's helper (+0x208) or, without one, needs the pending flag (+0xa4); when fn_71009299bc agrees it
// holds the owner's current track id, clears the flag and runs fn_7100929bbc and the game manager's hook, then
// clears the events. fn_710092e4e8 is whether a victim can be meleed: its AI component (+0x240, then +0x10; a
// CCharClassAIComponent) allows it outright (+0x199), else the owner's helper must accept (fn_71009e12d0) and not
// refuse (fn_71009e6434), then the AI's second flag (+0x19a) or the victim's movement (+0x1f0, fn_71010d0e38) decides.
// fn_7100931804 decides whether a hit message (kind at +0x1c, victim through +0x8 then +0x10) lands, from the
// owner's melee tracks and the victim's tracks or weapon movement. fn_7100931d54 spawns the hit effect
// (g_unk_71727385b0) at the message's position (+0x14) through the model manager. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAnimationComponent.h"
#include "game/CAutclastChargeAttack.h"
#include "game/CMovementComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkModelManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU134.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2739.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflTypeId;
using base::reflection::UnkReflTypeIdArg;
using base::reflection::UnkReflTypeIdTag;

extern const u64 g_unk_710164b270 __attribute__((visibility("hidden")));

struct UnkMeleeOwner_ovb0041 {
    u8 pad_0x0[0x2e8];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkMelee_ovb0041 {
    u8 pad_0x0[0x30];
    UnkMeleeOwner_ovb0041* owner;  // 0x30
};

void fn_71009298c4(UnkMelee_ovb0041* self) {
    if (UnkTimelineComponent* timeline = self->owner->timeline) {
        UnkReflTypeIdArg arg;
        arg.id = UnkReflTypeId<UnkReflTypeIdTag>::id();
        fn_7100a2cb7c(timeline, &g_unk_710164b270, &arg.id, false, -1.0f);
    }
}

static inline const void* SingletonPart(u32 offset) {
    return reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + offset;
}

const void* fn_710092dfd4(bool second) {
    if (second)
        return SingletonPart(0x1788);
    return SingletonPart(0x1778);
}

using base::global::CStrId;

struct UnkAnimationComponent;
void fn_71003246d8(void* p);

// A pending event: its id and kind.
struct UnkMeleeEvent_ovb0041 {
    u32 id;    // 0x0
    u32 kind;  // 0x4
    u8 pad_0x8[0x14];
};
static_assert(sizeof(UnkMeleeEvent_ovb0041) == 0x1c, "layout");

struct UnkMeleeOwner3_ovb0041 {
    u8 pad_0x0[0x208];
    void* helper;                     // 0x208
    u8 pad_0x210[0x10];
    UnkAnimationComponent* anim;      // 0x220
};

struct UnkMelee3_ovb0041 {
    u8 pad_0x0[0x30];
    UnkMeleeOwner3_ovb0041* owner;  // 0x30
    u8 pad_0x38[0x44];
    u32 event_count;                 // 0x7c
    UnkMeleeEvent_ovb0041* events;   // 0x80
    u8 pad_0x88[0x1c];
    bool pending;                    // 0xa4
};

bool fn_71009299bc(UnkMelee3_ovb0041* self);
void fn_7100929bbc(UnkMelee3_ovb0041* self);

void fn_7100929404(UnkMelee3_ovb0041* self) {
    bool hit = false;
    for (u32 i = 0; i < self->event_count; ++i) {
        UnkMeleeEvent_ovb0041& e = self->events[i];
        if (e.kind == 1)
            hit |= e.id == 0x80;
    }
    if (hit) {
        if (void* helper = self->owner->helper)
            fn_71009eaa88(helper);
    } else if (!self->pending) {
        self->event_count = 0;
        return;
    }
    if (fn_71009299bc(self)) {
        CStrId id(*fn_7101177ae8(self->owner->anim, 0xf4061ff25a2163d6));
        self->pending = false;
        fn_7100929bbc(self);
        if (void* p = fn_7100df57f4(g_unk_7101d47310))
            fn_71003246d8(p);
    }
    self->event_count = 0;
}

using base::reflection::UnkReflObject;

struct UnkHelper;
struct UnkMovement_71010d0e38;
bool fn_71009e12d0(void*);

// An AI component as seen through the reflected object (vtable slot 0 gives its class).
struct UnkMeleeAI_ovb0041 : UnkReflObject {
    u8 pad_0x8[0x191];
    bool field_0x199;  // always meleeable
    bool field_0x19a;  // meleeable while the owner's helper allows it
};

struct UnkMeleeActorComps_ovb0041 {
    u8 pad_0x0[0x10];
    UnkMeleeAI_ovb0041* ai;  // 0x10
};

struct UnkMeleeVictim_ovb0041 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x188];
    UnkMovement_71010d0e38* movement;  // 0x1f0
    u8 pad_0x1f8[0x28];
    UnkAnimationComponent* anim;  // 0x220
    u8 pad_0x228[0x18];
    UnkMeleeActorComps_ovb0041* comps;  // 0x240
};

struct UnkMeleeOwner2_ovb0041 {
    u8 pad_0x0[0x208];
    UnkHelper* helper;  // 0x208
    u8 pad_0x210[0x10];
    UnkAnimationComponent* anim;  // 0x220
};

struct UnkMelee2_ovb0041 {
    u8 pad_0x0[0x30];
    UnkMeleeOwner2_ovb0041* owner;  // 0x30
    u8 pad_0x38[0x60];
    bool blocks_all;  // 0x98
};

static inline UnkMeleeAI_ovb0041* GetAI(UnkMeleeVictim_ovb0041* victim) {
    if (!victim->comps)
        return nullptr;
    UnkMeleeAI_ovb0041* ai = victim->comps->ai;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_7100408b2c()))
        return ai;
    return nullptr;
}

bool fn_710092e4e8(UnkMelee2_ovb0041* self, UnkMeleeVictim_ovb0041* victim) {
    UnkMeleeAI_ovb0041* ai = GetAI(victim);
    if (ai && ai->field_0x199)
        return true;
    if (!self->owner->helper)
        return false;
    if (!fn_71009e12d0(self->owner->helper))
        return false;
    if (fn_71009e6434(self->owner->helper))
        return false;
    if (ai && ai->field_0x19a)
        return true;
    if (victim->movement && !fn_71010d0e38(victim->movement))
        return true;
    return false;
}

// The weapon movement a victim without animation component may have (its movement's +0x10).
struct UnkMeleeWeaponMovement_ovb0041 : UnkReflObject {
    u8 pad_0x8[0x5c];
    bool field_0x64;
};
struct UnkMeleeMovementView_ovb0041 {
    u8 pad_0x0[0x10];
    UnkMeleeWeaponMovement_ovb0041* weapon;  // 0x10
};

struct UnkMeleeHitSource_ovb0041 {
    u8 pad_0x0[0x10];
    UnkMeleeVictim_ovb0041* victim;  // 0x10
};

struct UnkMeleeHitMsg_ovb0041 {
    u8 pad_0x0[0x8];
    UnkMeleeHitSource_ovb0041* source;  // 0x8
    u8 pad_0x10[0xc];
    u32 kind;  // 0x1c
    u8 pad_0x20[0xa];
    bool active;  // 0x2a
    u8 pad_0x2b[0x41];
    u8 field_0x6c[4];  // 0x6c
};

struct UnkMeleePos_ovb0041 {
    f32 x, y, z;
};

bool fn_710092e0a8(UnkMelee2_ovb0041* self, const UnkMeleePos_ovb0041* pos, UnkMeleeVictim_ovb0041* victim, void* extra,
                   bool meleeable, f32 scale);

static inline bool OwnerHasTrack(UnkMelee2_ovb0041* self, const base::reflection::CClass* (*cls)()) {
    if (self->owner) {
        if (UnkAnimationComponent* anim = self->owner->anim)
            return fn_710117b348(anim, cls());
    }
    return false;
}

static inline bool HitLands(UnkMelee2_ovb0041* self, UnkMeleeHitMsg_ovb0041* msg) {
    if (!msg->source || !msg->source->victim)
        return false;
    bool damage = false;
    if (UnkAnimationComponent* anim = self->owner->anim)
        damage = fn_710117b348(anim, fn_7100d19258());
    bool melee = OwnerHasTrack(self, fn_7100d1ab58);
    if (!(damage | melee))
        return false;
    if (!msg->active)
        return false;
    UnkMeleeVictim_ovb0041* victim = msg->source ? msg->source->victim : nullptr;
    UnkAnimationComponent* own = self->owner->anim;
    if (!own || !fn_710117b348(own, fn_7100c13dd8())) {
        f32 x = victim->x, y = victim->y, z = victim->z;
        UnkMeleePos_ovb0041 pos = {x, y, z};
        if (!fn_710092e0a8(self, &pos, victim, msg->field_0x6c, fn_710092e4e8(self, victim), 1.0f))
            return false;
    }
    if (UnkAnimationComponent* anim = victim->anim) {
        if (melee & fn_710117b348(anim, fn_7100cf3508()))
            return true;
        if (!damage && !self->blocks_all)
            return false;
        return !fn_710117b348(anim, fn_7100cf0318());
    }
    UnkMeleeMovementView_ovb0041* movement = reinterpret_cast<UnkMeleeMovementView_ovb0041*>(victim->movement);
    if (!movement)
        return false;
    UnkMeleeWeaponMovement_ovb0041* weapon = movement->weapon;
    if (!weapon || !fn_710009d50c(weapon->vf_0x0(), fn_7100ab25ec()))
        return false;
    return weapon->field_0x64;
}

bool fn_7100931804(UnkMelee2_ovb0041* self, UnkMeleeHitMsg_ovb0041* msg) {
    if (msg->kind <= 1)
        return HitLands(self, msg);
    return false;
}

// The effect object the model manager hands out, as its virtual calls see it.
struct UnkFxObject_ovb0041;
struct UnkFxObjectVtable_ovb0041 {
    void* unk_0x0[2];
    void (*set_position)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x10
    void (*set_rotation)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x18
    void (*set_scale)(UnkFxObject_ovb0041* self, const UnkVec3f* v);     // 0x20
    void* unk_0x28[13];
    void (*set_loop)(UnkFxObject_ovb0041* self, bool loop);               // 0x90
    void (*set_speed)(UnkFxObject_ovb0041* self, f32 speed);              // 0x98
};
struct UnkFxObject_ovb0041 {
    UnkFxObjectVtable_ovb0041* vtable;
};

extern const u8 g_unk_71727385b0[] __attribute__((visibility("hidden")));  // the effect's path id

struct UnkMeleeHit_ovb0041 {
    u8 pad_0x0[0x14];
    UnkVec3f position;  // 0x14
};

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

void fn_7100931d54(void* self, const UnkMeleeHit_ovb0041* hit) {
    UnkVec3f at = hit->position;
    UnkModelManager* manager = ModelManager();
    UnkVec3f position = at;
    UnkVec3f rotation(0.0f, 0.0f, 0.0f);
    UnkVec3f scale(1.0f, 1.0f, 1.0f);
    if (UnkFxObject_ovb0041* fx = fn_71003b7db0(manager, g_unk_71727385b0, false, true)) {
        fx->vtable->set_position(fx, &position);
        fx->vtable->set_rotation(fx, &rotation);
        fx->vtable->set_scale(fx, &scale);
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(fx), true);
        fx->vtable->set_loop(fx, false);
        fx->vtable->set_speed(fx, 1.0f);
        fn_71011a9bb4(manager, fx);
    }
}
